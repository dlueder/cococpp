#include <QCoreApplication>
#include <QDebug>
#include "libs/cocoreader.h"

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    qDebug() << "Hello World";
    COCOReader reader = COCOReader("../../train.json");
    reader.readAnnotations();
    reader.readCategories();
    reader.readImages();
    reader.readLicenses();
    reader.readInfo();
    std::cout << reader.images[0].sFilename << std::endl;
    //return a.exec();
}
