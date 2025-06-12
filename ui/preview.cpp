#include "preview.h"

Preview::Preview(QWidget *parent)
    : QMainWindow{parent}
    , ui(new Ui_Preview)
{
    this->ui->setupUi(this);
    this->reader = new COCOReader("../../train.json");
    Image *img = this->reader->getImageByID(1);
    cv::Mat image = cv::imread(this->sBaseImgPath + img->sFilename, cv::IMREAD_COLOR);
    cv::Mat mask = this->reader->generateMask(1);
    QPixmap pxImage = convertMatToPixmap(image, QImage::Format_BGR888);
    pxImage = pxImage.scaledToHeight(this->ui->image->height());
    pxImage = pxImage.scaledToWidth(this->ui->image->width());
    QPixmap pxMask = convertMatToPixmap(mask, QImage::Format_Grayscale8);
    pxMask = pxMask.scaledToHeight(this->ui->mask->height());
    pxMask = pxMask.scaledToWidth(this->ui->mask->width());
    this->ui->image->setPixmap(pxImage);
    this->ui->mask->setPixmap(pxMask);
}
