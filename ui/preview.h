#ifndef PREVIEW_H
#define PREVIEW_H

#include <QWidget>
#include "../libs/cocoreader.h"
#include "../libs/util.h"
#include "ui_preview.h"

class Preview : public QMainWindow
{
    Q_OBJECT
public:
    explicit Preview(QWidget *parent = nullptr);

private:
    Ui_Preview *ui;
    std::string sBaseImgPath
        = "/Volumes/External II/Sicherungen/basil-backup/dataset-training/train/img/";
    COCOReader *reader;

signals:
};

#endif // PREVIEW_H
