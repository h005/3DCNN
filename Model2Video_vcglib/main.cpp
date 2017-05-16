//#include <QCoreApplication>
#include <QApplication>
#include "meshmodelhandler.h"

#include "test.h"

void programStart();

void batchParaParse(QString &configFile, QString &logFile);

int main(int argc, char *argv[])
{
    // to use opengl, you must use QApplication, but not the QCoreApplication
    QApplication a(argc, argv);

    programStart();

    exit(0);
    return a.exec();
}

void programStart()
{
    MeshModelHandler *meshHandler;
    QString configFile;
    QString logFile;

    std::string operation;
    while(cin >> operation)
    {
        if( !operation.compare("-f") || !operation.compare("-feature"))
        {
            batchParaParse(configFile, logFile);
            meshHandler = new MeshModelHandler(configFile,logFile);
            meshHandler->generateFeatures();
            break;
        }
        else if( !operation.compare("-t") || !operation.compare("-track"))
        {
            batchParaParse(configFile, logFile);
            meshHandler = new MeshModelHandler(configFile,logFile);
            meshHandler->generateTrajectoryWithSeam();
            break;
        }
        else if( !operation.compare("-r") || !operation.compare("-render"))
        {
            batchParaParse(configFile, logFile);
            meshHandler = new MeshModelHandler(configFile,logFile);
            meshHandler->generateFrames();
            break;
        }
        else if( !operation.compare("-h") )
        {
            std::cout << "Usage: -operation -batchNum" << std::endl;
            std::cout << "operation is one of the \"f\" \"t\" \"r\" \"feature\" \"track\" \"render\"" << std::endl;
            std::cout << "batchNum is one of [0,1,2,3]" << std::endl;
            std::cout << "where 0 means compute all the models in one batch" << std::endl;
        }
        else if( !operation.compare("q"))
        {
            break;
        }
        else
        {
            std::cout << "parameter error!" << std::endl;
            std::cout << "type in -h for help" << std::endl;
        }
    }

}

void batchParaParse(QString &configFile, QString &logFile)
{
    int batch;
    std::cin >> batch;

    switch(batch)
    {
    case 0:
        configFile = "/home/h005/Documents/3dcnn/code/3DCNN/configFile/modelVideoBatch/miniBatch0.ini";
        logFile = "/home/h005/Documents/3dcnn/code/3DCNN/configFile/modelVideoBatch/miniBatch0.log";
        break;
    case 1:
        configFile = "/home/h005/Documents/3dcnn/code/3DCNN/configFile/modelVideoBatch/miniBatch1.ini";
        logFile = "/home/h005/Documents/3dcnn/code/3DCNN/configFile/modelVideoBatch/miniBatch1.log";
        break;
    case 2:
        configFile = "/home/h005/Documents/3dcnn/code/3DCNN/configFile/modelVideoBatch/miniBatch2.ini";
        logFile = "/home/h005/Documents/3dcnn/code/3DCNN/configFile/modelVideoBatch/miniBatch2.log";
        break;
    case 3:
        configFile = "/home/h005/Documents/3dcnn/code/3DCNN/configFile/modelVideoBatch/miniBatch3.ini";
        logFile = "/home/h005/Documents/3dcnn/code/3DCNN/configFile/modelVideoBatch/miniBatch3.log";
        break;
    default:
        std::cout << "err input" << std::endl;
        exit(0);
    }
}
