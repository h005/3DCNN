//#include <QCoreApplication>
#include <QApplication>
#include "handler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QSurfaceFormat fmt;
//    fmt.setDepthBufferSize(24);

//    QSurfaceFormat::setDefaultFormat(fmt);

    Handler *handler = new Handler("/home/hejw005/Documents/3dcnn/code/test.ini",
                                   "/home/hejw005/Documents/3dcnn/code/model2videoAirplane.log");

    handler->rendering();

    std::cout << "rendering done!" << std::endl;

    delete handler;

    return a.exec();
}
