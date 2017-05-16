#include "cameraparamanager.h"

CameraParaManager::CameraParaManager()
{
    fileName = "";
}

CameraParaManager::~CameraParaManager()
{
    clear();
}

CameraParaManager::CameraParaManager(std::string fileName)
{
    this->fileName = fileName;
    loadInCameraParameters();
}

CameraParaManager::CameraParaManager(QString fileName)
{
    this->fileName = fileName.toStdString();
    loadInCameraParameters();
}

glm::mat4 &CameraParaManager::getModelMatrix(int index)
{
    return model_matrixList[index];
}

glm::mat4 &CameraParaManager::getViewMatrix(int index)
{
    return view_matrixList[index];
}

glm::mat4 &CameraParaManager::getProjectiveMatrix(int index)
{
    return projection_matrixList[index];
}

QString &CameraParaManager::getImageName(int index)
{
    return img_names[index];
}

void CameraParaManager::setImgFolder(QDir baseDir, QString model)
{
    QString tmpFile = baseDir.absolutePath().append("/").append(model);
    QFileInfo tmpFileInfo(tmpFile);
    imgFolder = tmpFileInfo.absolutePath();
    // is the folder does not exist, then create it
    QDir folder(imgFolder.append("/").append(tmpFileInfo.baseName()));
    if(folder.exists())
    {
        folder.removeRecursively();
        folder.mkpath(imgFolder);
    }
    else
        folder.mkpath(imgFolder);
}

void CameraParaManager::getFeafileHandler(QString basePath, QString model, std::fstream &feaout)
{
    model.replace('/','_');
    std::cout << model.toStdString() << std::endl;
    // remove the suffix
    int pos = model.lastIndexOf('.');
    model = model.left(pos);
    model = model + ".fea";
    QString feaoutFile = basePath + '/' + model;
    feaout.open(feaoutFile.toStdString(), std::fstream::out);
    return;
}

/*
void CameraParaManager::getFeaImgPath(QString basePath,
                                      QString model,
                                      QString attr,
                                      QString &heatMapPath,
                                      QString &energyMapPath)
{
    model.replace('/','_');
    int pos = model.lastIndexOf('.');
    model = model.left(pos);
    model = model + "_" + attr;
    heatMapPath = basePath + '/' + model + ".jpg";
    energyMapPath = basePath + '/' + model + "_e.jpg";
}
*/


void CameraParaManager::clear()
{
    img_names.clear();
    model_matrixList.clear();
    view_matrixList.clear();
    projection_matrixList.clear();
}

void CameraParaManager::loadInCameraParameters()
{

    std::fstream fstream(fileName, std::fstream::in);

    assert(fstream);

    std::string tmpss;
    while(fstream >> tmpss)
    {
        img_names.push_back(QString(tmpss.c_str()));
        glm::mat4 m,v,p;
        float tmpNum;
        for(int i=0;i<16;i++)
        {
            fstream >> tmpNum;
            m[i%4][i/4] = tmpNum;
        }
        this->model_matrixList.push_back(m);
        this->view_matrixList.push_back(v);
        for(int i=0;i<16;i++)
        {
            fstream >> tmpNum;
            p[i%4][i/4] = tmpNum;
        }
        this->projection_matrixList.push_back(p);
    }
    fstream.close();
    len = this->model_matrixList.size();
    std::cout << "matrix file " << fileName << " load done!" << std::endl;
}
