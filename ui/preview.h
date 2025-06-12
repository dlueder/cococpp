#ifndef PREVIEW_H
#define PREVIEW_H

#include <QFile>
#include <QFileDialog>
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
    QString sBaseImgPath;
    COCOReader *reader;
    int iCurrentID = 1;
    int iMaxID = 1;
    bool bShowBoxes = false;
    void nextImage();
    void previousImage();
    void showImage(int iID);
    void exportMasks();

signals:
};

#endif // PREVIEW_H
