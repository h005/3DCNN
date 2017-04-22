#include "modellistmanager.h"

ModelListManager::ModelListManager(QString modelListFile)
{
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
