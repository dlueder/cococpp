#include "cocoreader.h"

COCOReader::COCOReader()
{
    std::ifstream ifs("../../train.json");
    this->jf = nlohmann::json::parse(ifs);
}

COCOReader::~COCOReader() {}

void COCOReader::readAnnotations()
{
    std::cout << jf.size() << std::endl;
    std::cout << jf["licenses"] << std::endl;
}

void COCOReader::readCategory() {}
void COCOReader::readImages() {}
void COCOReader::readLicense() {}

void COCOReader::generateMasks() {}
