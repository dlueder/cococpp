#ifndef UTIL_H
#define UTIL_H

#include <QPixmap>
#include <opencv2/imgproc.hpp>

QPixmap convertMatToPixmap(cv::Mat, QImage::Format format = QImage::Format_BGR888);

#endif // UTIL_H
