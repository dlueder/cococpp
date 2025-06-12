#ifndef COCOREADER_H
#define COCOREADER_H

#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <ranges>
#include <type_traits>

using Segmentation = std::vector<cv::Point>;

struct BBox
{
    int iXmin;
    int iYmin;
    int iWidth;
    int iHeight;
} typedef BBox;

struct Annotation
{
    float fArea;
    BBox box;
    int iCategoryID;
    int iID;
    int iImageID;
    std::vector<Segmentation> segmentations;
    std::string sCaption;

} typedef Annotation;

struct Category
{
    int iID;
    std::string sName;
    std::string sSuperCategory;
} typedef Category;

struct Image
{
    std::string sCOCOUrl;
    std::string sDateCaptured;
    std::string sFilename;
    std::string sFlickrUrl;
    int iHeight;
    int iWidth;
    int iID;
    int iLicense;

} typedef Image;

struct License
{
    int iID;
    std::string sName;
    std::string sUrl;
} typedef License;

struct Info
{
    std::string sContributor;
    std::string sDateCreated;
    std::string sDescription;
    std::string sUrl;
    std::string sVersion;
    int iYear;
} typedef Info;

template<typename T>
concept Parseable = std::same_as<T, std::string> || std::same_as<T, int> || std::same_as<T, float>
                    || std::same_as<T, std::vector<int>>;

template<Parseable T>
T readField(nlohmann::json v, std::string sFieldname)
{
    T field;
    if constexpr (std::is_arithmetic_v<T>) {
        field = -1;
    } else if constexpr (std::same_as<T, std::vector<int>>) {
        field = {-1, -1, -1, -1};
    } else {
        field = "";
    }

    if (v.contains(sFieldname)) {
        v[sFieldname].get_to(field);
    }
    return field;
}

class COCOReader
{
public:
    COCOReader(std::string sFilename);
    ~COCOReader();
    Annotation *getAnnotationByID(int iID); // returns Annotation with ID iID
    std::vector<Annotation *> getAnnotationsByImageID(
        int iID);                // returns Annotation/Annotations that belong to image with iID
    Image *getImageByID(int iID); // returns Image with id iID
    cv::Mat generateMask(int iID, bool bUseBoxesAsMask = false);
    std::vector<BBox> generateBBoxes(int iID);
    std::vector<Annotation> annotations;
    std::vector<Image> images;
    std::vector<Category> categories;
    std::vector<License> licenses;
    Info info;

private:
    nlohmann::json jf;
    std::map<int, std::vector<Annotation *>> mImageToAnnotation;
    void readAnnotations();
    void readCategories();
    void readImages();
    void readLicenses();
    void readInfo();
    void generateMapping();
};

#endif // COCOREADER_H
