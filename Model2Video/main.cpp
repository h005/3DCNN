#include <QCoreApplication>
#include <QSurfaceFormat>
//#include <QDesktopWidget>
#include "handler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    QSurfaceFormat fmt;
//    fmt.setDepthBufferSize(24);

//    QSurfaceFormat::setDefaultFormat(fmt);

    Handler handler("/home/h005/Documents/3dcnn/data/config.ini","");


    return a.exec();
}
