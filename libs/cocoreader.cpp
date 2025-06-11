#include "cocoreader.h"

COCOReader::COCOReader(std::string sFilename)
{
    std::ifstream ifs(sFilename);
    this->jf = nlohmann::json::parse(ifs);
    ifs.close();
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
        std::cout << a.size() << std::endl;
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
                            seg.points.push_back(Point({points[i], points[i + 1]}));
                        }
                        segmentations.push_back(seg);
                    }
                }
            }
            float area;
            int category_id, id, image_id;
            std::vector<int> bbox;
            if (v.contains("area")) {
                v["area"].get_to(area);
            }

            v["bbox"].get_to(bbox);
            v["category_id"].get_to(category_id);
            v["id"].get_to(id);
            v["image_id"].get_to(image_id);
            // caption
            Annotation e = {area,
                            BBox({bbox[0], bbox[1], bbox[2], bbox[3]}),
                            category_id,
                            id,
                            image_id,
                            segmentations};
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
            int id;
            std::string name, supercategory;
            v["id"].get_to(id);
            v["name"].get_to(name);
            v["supercategory"].get_to(supercategory);
            Category c = {id, name, supercategory};
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
            std::string coco_url;
            std::string filename;
            int height, width, id, license;
            v["coco_url"].get_to(coco_url);
            v["file_name"].get_to(filename);
            v["height"].get_to(height);
            v["width"].get_to(width);
            v["id"].get_to(id);
            v["license"].get_to(license);
            Image i = {coco_url, filename, height, width, id, license};
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
            int id;
            std::string name;
            std::string url;
            v["id"].get_to(id);
            v["name"].get_to(name);
            v["url"].get_to(url);
            License l = {id, name, url};
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
        std::string c, d, de, u, ver, y;
        v["contributor"].get_to(c);
        v["date_created"].get_to(d);
        v["description"].get_to(de);
        v["url"].get_to(u);
        v["version"].get_to(ver);
        v["year"].get_to(y);
        this->info = Info({c, d, de, u, ver, y});
    } catch (nlohmann::json::exception &e) {
        std::cout << "readInfo" << e.what() << std::endl;
    }
}

Annotation getAnnotationByImageID(int iID)
{
    return Annotation();
}

void COCOReader::generateMasks(bool bBinaryMask) {}
