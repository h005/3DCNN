#ifndef MODELLISTMANAGER_H
#define MODELLISTMANAGER_H

#include <QString>
#include <QStringList>
#include <fstream>
#include <assert.h>

class ModelListManager
{
public:
    ModelListManager(QString modelListFile);

    void getModelList(QStringList& modelList);

private:
    std::fstream fmlt;
};

#endif // MODELLISTMANAGER_H
