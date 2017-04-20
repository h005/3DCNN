#include "imagemanager.h"

ImageManager::ImageManager()
{
    fileName = "";
}

ImageManager::~ImageManager()
{
    clear();
}

ImageManager::ImageManager(std::string fileName)
{
    this->fileName = fileName;
    loadInCameraParameters();
}

ImageManager::ImageManager(QString fileName)
{
    this->fileName = fileName.toStdString();
    loadInCameraParameters();
}

glm::mat4 &ImageManager::getModelMatrix(int index)
{
    return model_matrixList[index];
}

glm::mat4 &ImageManager::getViewMatrix(int index)
{
    return view_matrixList[index];
}

glm::mat4 &ImageManager::getProjectiveMatrix(int index)
{
    return projection_matrixList[index];
}

QString &ImageManager::getImageName(int index)
{
    return img_names[index];
}

void ImageManager::setImgFolder(QDir baseDir, QString model)
{
    QString tmpFile = baseDir.absolutePath().append("/").append(model);
    QFileInfo tmpFileInfo(tmpFile);
    imgFolder = tmpFileInfo.absolutePath();
    // is the folder does not exist, then create it
    QDir folder(imgFolder.append("/").append(tmpFileInfo.baseName()));
    if(!folder.exists())
        folder.mkpath(imgFolder);
}

void ImageManager::clear()
{
    img_names.clear();
    model_matrixList.clear();
    view_matrixList.clear();
    projection_matrixList.clear();
}

void ImageManager::loadInCameraParameters()
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
