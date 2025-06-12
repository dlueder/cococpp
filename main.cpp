#include <QApplication>
#include <QDebug>
#include "libs/cocoreader.h"
#include "ui/preview.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Preview window = Preview();
    window.show();
    return app.exec();
}
