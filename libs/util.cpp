#include "util.h"

QPixmap convertMatToPixmap(cv::Mat img, QImage::Format format){
    QImage image;
    if(img.channels() > 1){
        image = QImage((const uchar*)img.data, img.cols, img.rows, img.step, format);
    }
    else{
        image = QImage((const uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8);
    }
    return QPixmap::fromImage(image);
}
