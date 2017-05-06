//#include <QCoreApplication>
#include <QApplication>
#include "meshmodelhandler.h"

#include "test.h"

int main(int argc, char *argv[])
{
    // to use opengl, you must use QApplication, but not the QCoreApplication
    QApplication a(argc, argv);

    QString fn1 = "/home/hejw005/Documents/3dcnn/code/test.ini";
    QString fn2 = "/home/hejw005/Documents/3dcnn/code/ModelNet40.log";

    MeshModelHandler *meshHandler = new MeshModelHandler(fn1,fn2);



    meshHandler->generateFrames();
//    meshHandler.generateFrames();
    exit(0);
    return a.exec();
}
