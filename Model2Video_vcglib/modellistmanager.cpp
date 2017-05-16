#include "modellistmanager.h"

#include <iostream>

ModelListManager::ModelListManager(QString modelListFile)
{
    std::cout << "Model list manager" << std::endl;
    std::cout << modelListFile.toStdString() << std::endl;
    fileHandlerFormlt.open(modelListFile.toStdString(), std::fstream::in);
    assert(fileHandlerFormlt);
}

void ModelListManager::getModelList(QStringList &modelList)
{
    // read in the model list from file
    std::string ss;
    while(fileHandlerFormlt >> ss)
    {
        QString modelFile(ss.c_str());
        modelList.push_back(modelFile);
    }
}
