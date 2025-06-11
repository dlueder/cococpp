#ifndef COCOREADER_H
#define COCOREADER_H

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

struct Point
{
    float x;
    float y;
} typedef Point;

struct Segmentation
{
    std::vector<Point> points;
} typedef Segmentation;

struct BBox
{
    int xmin;
    int ymin;
    int xmax;
    int ymax;
} typedef BBox;

struct Annotation
{
    float area;
    BBox box;
    int iCategoryID;
    int iID;
    int iImageID;
    std::vector<Segmentation> segmentations;
    std::string caption;

} typedef Annotation;

struct Category
{
    int iID;
    std::string sName;
    std::string iSuperCategory;
} typedef Category;

struct Image
{
    std::string sCOCOUrl;
    // std::string dateCaptured
    std::string sFilename;
    // std::string sFlickrUrl = "";
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
    std::string sYear;
} typedef Info;

class COCOReader
{
public:
    COCOReader(std::string sFilename);
    ~COCOReader();
    void readAnnotations();
    void readCategories();
    void readImages();
    void readLicenses();
    void readInfo();
    void generateMasks(bool bBinaryMask = true);

    std::vector<Annotation> annotations;
    std::vector<Image> images;
    std::vector<Category> categories;
    std::vector<License> licenses;
    Info info;

private:
    nlohmann::json jf;
    Annotation getAnnotationByImageID(int iID);
};

#endif // COCOREADER_H
