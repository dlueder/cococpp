#ifndef COCOREADER_H
#define COCOREADER_H

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

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

} typedef Annotation;

struct Category
{
    int iID;
    std::string sName;
    int iSuperCategory;
} typedef Category;

struct Image
{
    std::string sCOCOUrl;
    // std::string dateCaptured
    std::string sFilename;
    std::string sFlickrUrl = "";
    int height;
    int width;
    int iID;
    int iLicense;

} typedef Image;

class COCOReader
{
public:
    COCOReader();
    ~COCOReader();
    void readCategory();
    void readAnnotations();
    void readImages();
    void readLicense();
    void generateMasks();

private:
    nlohmann::json jf;
};

#endif // COCOREADER_H
