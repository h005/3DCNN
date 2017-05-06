#include "modellistmanager.h"

#include <iostream>

ModelListManager::ModelListManager(QString modelListFile)
{
    std::cout << "Model list manager" << std::endl;
    std::cout << modelListFile.toStdString() << std::endl;
    fmlt.open(modelListFile.toStdString(), std::fstream::in);
    assert(fmlt);
}

void ModelListManager::getModelList(QStringList &modelList)
{
    // read in the model list from file
    std::string ss;
    while(fmlt >> ss)
    {
        QString modelFile(ss.c_str());
        modelList.push_back(modelFile);
    }
}
