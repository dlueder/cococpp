#include "preview.h"

Preview::Preview(QWidget *parent)
    : QMainWindow{parent}
    , ui(new Ui_Preview)
{
    this->ui->setupUi(this);
    QObject::connect(this->ui->nextImageButton, &QPushButton::clicked, this, &Preview::nextImage);
    QObject::connect(this->ui->previousImageButton,
                     &QPushButton::clicked,
                     this,
                     &Preview::previousImage);
    QObject::connect(this->ui->previewBoxes, &QCheckBox::toggled, this, [&](bool state) {
        this->bShowBoxes = state;
        this->showImage(this->iCurrentID);
    });
    QObject::connect(this->ui->generateMasksButton,
                     &QPushButton::clicked,
                     this,
                     &Preview::exportMasks);
    QObject::connect(this->ui->useBBoxAsMask, &QCheckBox::toggled, this, [&](bool state) {
        this->bUseBoxesAsMask = state;
        this->showImage(this->iCurrentID);
    });

    QString datasetFile = QFileDialog::getOpenFileName(this,
                                                       "Select COCO dataset file to read",
                                                       "./",
                                                       "*.json");

    this->sBaseImgPath = QFileDialog::getExistingDirectory(this,
                                                           "Select base folder of images",
                                                           "./",
                                                           QFileDialog::ShowDirsOnly);
    if (datasetFile.isEmpty() || this->sBaseImgPath.isEmpty()) {
        qDebug() << "Dataset or Image Path is empty";
        return;
    }
    qDebug() << "Dataset file: " << datasetFile;
    qDebug() << "Image folder: " << this->sBaseImgPath;
    this->reader = new COCOReader(datasetFile.toStdString());
    qDebug() << "Dataset has " << this->reader->images.size() << " images with "
             << this->reader->annotations.size() << " annotations";
    this->iMaxID
        = this->reader->images
              .size(); // assumes that images are index based 1 (as per CVAT COCO format) and that IDs are ascending
    this->showImage(this->iCurrentID);
}

void Preview::showImage(int iID)
{
    Image *img = this->reader->getImageByID(iID);
    if (img) {
        QString filename = this->sBaseImgPath + "/" + QString::fromStdString(img->sFilename);
        if (QFile::exists(filename)) {
            this->ui->filename->setText(filename);
            cv::Mat image = cv::imread(filename.toStdString(), cv::IMREAD_COLOR);
            if (this->bShowBoxes) {
                for (auto &a : this->reader->annotations) {
                    std::vector<BBox> boxes = this->reader->generateBBoxes(this->iCurrentID);
                    for (auto &b : boxes) {
                        cv::rectangle(image,
                                      cv::Rect(b.iXmin, b.iYmin, b.iWidth, b.iHeight),
                                      cv::Scalar(0, 0, 255),
                                      4);
                    }
                }
            }
            cv::Mat mask = this->reader->generateMask(iID, this->bUseBoxesAsMask);
            if (!mask.empty()) {
                QPixmap pxImage = convertMatToPixmap(image, QImage::Format_BGR888);
                pxImage = pxImage.scaledToHeight(this->ui->image->height());
                pxImage = pxImage.scaledToWidth(this->ui->image->width());
                QPixmap pxMask = convertMatToPixmap(mask, QImage::Format_Grayscale8);
                pxMask = pxMask.scaledToHeight(this->ui->mask->height());
                pxMask = pxMask.scaledToWidth(this->ui->mask->width());
                this->ui->image->setPixmap(pxImage);
                this->ui->mask->setPixmap(pxMask);
                this->ui->counter->setText(QString::number(this->iCurrentID) + "/"
                                           + QString::number(this->iMaxID));
            }
        } else {
            qDebug() << filename << " does not exist";
        }
    }
}

void Preview::nextImage()
{
    if (this->iCurrentID + 1 <= this->iMaxID) {
        this->iCurrentID++;
        this->showImage(this->iCurrentID);
    }
}

void Preview::previousImage()
{
    if (this->iCurrentID - 1 >= 1) {
        this->iCurrentID--;
        this->showImage(this->iCurrentID);
    }
}

void Preview::exportMasks()
{
    QString exportFolder = QFileDialog::getExistingDirectory(this,
                                                             "Select export folder of masks",
                                                             "./",
                                                             QFileDialog::ShowDirsOnly);
    for (int i = 1; i <= this->iMaxID; i++) {
        Image *img = this->reader->getImageByID(i);
        if (img) {
            cv::Mat mask = this->reader->generateMask(i, this->bUseBoxesAsMask);
            std::string filename = exportFolder.toStdString() + "/" + img->sFilename;
            cv::imwrite(filename, mask);
        } else {
            qDebug() << "Image with ID " << i << " does not exist";
        }
    }
    qDebug() << "Generating masks done";
}
