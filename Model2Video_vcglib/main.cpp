//#include <QCoreApplication>
#include <QApplication>
#include "meshmodelhandler.h"

#include "test.h"

int main(int argc, char *argv[])
{
    // to use opengl, you must use QApplication, but not the QCoreApplication
    QApplication a(argc, argv);

    QString fn1 = "/home/h005/Documents/3dcnn/code/3DCNN/configFile/miniTest.ini";
    QString fn2 = "/home/h005/Documents/3dcnn/code/3DCNN/configFile/ModelNetMini.log";

    MeshModelHandler *meshHandler = new MeshModelHandler(fn1,fn2);



//    meshHandler->generateFrames();
    meshHandler->generateFeatures();
    exit(0);
    return a.exec();
}
