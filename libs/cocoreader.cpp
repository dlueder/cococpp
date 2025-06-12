#include "cocoreader.h"

COCOReader::COCOReader(std::string sFilename)
{
    std::ifstream ifs(sFilename);
    this->jf = nlohmann::json::parse(ifs);
    ifs.close();
    this->readAnnotations();
    this->readCategories();
    this->readImages();
    this->readLicenses();
    this->readInfo();
    this->generateMapping();
}

COCOReader::~COCOReader()
{
    this->jf.clear();
}

void COCOReader::readAnnotations()
{
    auto start = std::chrono::high_resolution_clock::now();
    try {
        nlohmann::json a = this->jf["annotations"];
        std::cout << a.size() << " annotations in dataset" << std::endl;
        for (nlohmann::json::iterator it = a.begin(); it != a.end(); ++it) {
            std::vector<Segmentation> segmentations;
            auto v = it.value();
            if (v.contains("segmentation")) {
                auto s = v["segmentation"];
                for (nlohmann::json::iterator sit = s.begin(); sit != s.end(); ++sit) {
                    if (sit->size() % 2 == 0) {
                        Segmentation seg;
                        auto points = sit.value();
                        for (int i = 0; i < sit->size(); i = i + 2) {
                            seg.push_back(cv::Point(points[i], points[i + 1]));
                        }
                        segmentations.push_back(seg);
                    }
                }
            }
            int category_id, id, image_id;
            std::vector<int> bbox = readField<std::vector<int>>(v, "bbox");
            Annotation e = {readField<float>(v, "area"),
                            BBox({bbox[0], bbox[1], bbox[2], bbox[3]}),
                            readField<int>(v, "category_id"),
                            readField<int>(v, "id"),
                            readField<int>(v, "image_id"),
                            segmentations,
                            readField<std::string>(v, "caption")};
            this->annotations.push_back(e);
        }
    } catch (nlohmann::json::exception &e) {
        std::cout << "readAnnotations" << e.what() << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Parsing annotations took: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms"
              << std::endl;
}

void COCOReader::readCategories()
{
    try {
        nlohmann::json a = this->jf["categories"];
        for (nlohmann::json::iterator it = a.begin(); it != a.end(); ++it) {
            auto v = it.value();
            Category c = {readField<int>(v, "id"),
                          readField<std::string>(v, "name"),
                          readField<std::string>(v, "supercategory")};
        }
    } catch (nlohmann::json::exception &e) {
        std::cout << "readCategories" << e.what() << std::endl;
    }
}

void COCOReader::readImages()
{
    try {
        auto start = std::chrono::high_resolution_clock::now();
        nlohmann::json a = this->jf["images"];
        for (nlohmann::json::iterator it = a.begin(); it != a.end(); ++it) {
            auto v = it.value();
            Image i = {readField<std::string>(v, "coco_url"),
                       readField<std::string>(v, "file_name"),
                       readField<int>(v, "height"),
                       readField<int>(v, "width"),
                       readField<int>(v, "id"),
                       readField<int>(v, "license")};
            this->images.push_back(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Parsing images took: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms" << std::endl;
    } catch (nlohmann::json::exception &e) {
        std::cout << "readImages" << e.what() << std::endl;
    }
}
void COCOReader::readLicenses()
{
    try {
        nlohmann::json a = this->jf["licenses"];
        for (nlohmann::json::iterator it = a.begin(); it != a.end(); ++it) {
            auto v = it.value();
            License l = {readField<int>(v, "id"),
                         readField<std::string>(v, "name"),
                         readField<std::string>(v, "url")};
            this->licenses.push_back(l);
        }
    } catch (nlohmann::json::exception &e) {
        std::cout << "readLicenses" << e.what() << std::endl;
    }
}

void COCOReader::readInfo()
{
    try {
        auto v = this->jf["info"];
        this->info = Info({readField<std::string>(v, "contributor"),
                           readField<std::string>(v, "date_created"),
                           readField<std::string>(v, "description"),
                           readField<std::string>(v, "url"),
                           readField<std::string>(v, "version"),
                           readField<int>(v, "year")});
    } catch (nlohmann::json::exception &e) {
        std::cout << "readInfo" << e.what() << std::endl;
    }
}

void COCOReader::generateMapping()
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << this->images.size() << " images in dataset" << std::endl;
    for (const Image &i : this->images) {
        auto annotationFilter = this->annotations | std::views::filter([&](const Annotation &a) {
                                    return a.iImageID == i.iID;
                                });
        for (Annotation &a : annotationFilter) {
            this->mImageToAnnotation[i.iID].push_back(&a);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Generate image to annotation mapping took: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms"
              << std::endl;
}

std::vector<Annotation *> COCOReader::getAnnotationsByImageID(int iID)
{
    return this->mImageToAnnotation[iID];
}

Image *COCOReader::getImageByID(int iID)
{
    auto imageFilter = this->images
                       | std::views::filter([&](const Image &i) { return i.iID == iID; });
    if (!imageFilter.empty()) {
        return &imageFilter.front();
    }
    return nullptr;
}

cv::Mat COCOReader::generateMask(int iID, bool bUseBoxesAsMask)
{
    try {
        Image *image = this->getImageByID(iID);
        if (image) {
            cv::Mat mask = cv::Mat(image->iHeight, image->iWidth, CV_8UC1, 1);
            for (auto &ann : this->getAnnotationsByImageID(iID)) {
                if (bUseBoxesAsMask) {
                    cv::rectangle(mask,
                                  cv::Rect(ann->box.xmin,
                                           ann->box.ymin,
                                           ann->box.width,
                                           ann->box.height),
                                  cv::Scalar(255, 255, 255),
                                  -1);
                } else {
                    for (auto &seg : ann->segmentations) {
                        if (!seg.empty()) {
                            cv::fillPoly(mask, seg, cv::Scalar(255, 255, 255));
                        }
                    }
                }
            }
            return mask;
        }
    } catch (cv::Exception &e) {
        std::cout << e.what() << std::endl;
    }
    return cv::Mat();
}

std::vector<BBox> COCOReader::generateBBoxes(int iID)
{
    auto annotationFilter = this->annotations | std::views::filter([&](const Annotation &a) {
                                return a.iImageID == iID;
                            });
    std::vector<BBox> boxes;
    for (Annotation &a : annotationFilter) {
        boxes.push_back(a.box);
    }
    return boxes;
}
