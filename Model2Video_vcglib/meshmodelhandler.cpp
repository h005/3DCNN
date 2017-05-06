#include "meshmodelhandler.h"
#include <QSettings>

#include <QDateTime>
#include <QtDebug>

MeshModelHandler::MeshModelHandler(QString configFileName, QString logFileName)
{
    QSettings configIni(configFileName, QSettings::IniFormat);

    // read in the baseDir
    config_baseDir = QDir(configIni.value("path/baseDir").toString());

    // read in the model list
    QString modelsString = configIni.value("model/model").toString();
    modelListManager = new ModelListManager(modelsString);
    config_modelList.clear();
    modelListManager->getModelList(config_modelList);

    // read in the matrix file
    config_matrixFile = QFileInfo(configIni.value("matrix/matrixFile").toString());

    QFileInfo logFile(logFileName);
    // if the file does not exist
    if ( logFileName == "" )
    {
        // create the file as time.log
        QString tmpLogFileName = QDateTime::currentDateTime().toString();
        tmpLogFileName.replace(' ','_');
        tmpLogFileName.replace(':','_');
        logFile = QFileInfo(tmpLogFileName.append(".log"));
    }

    flog.open(logFile.absoluteFilePath().toStdString(), std::fstream::out);

    assert(flog);

//    render = NULL;
    meshContainer = NULL;
    imgManager = NULL;
    // load the matrix file
    loadInMatrix();

}

MeshModelHandler::MeshModelHandler()
{
    meshContainer = NULL;
    modelListManager = NULL;
    imgManager = NULL;
}

MeshModelHandler::~MeshModelHandler()
{

}

void MeshModelHandler::generateFrames()
{
    QString fileName;
    // read in the mesh models
    glm::mat4 identity;
    for (int i=0;i<config_modelList.size(); i++)
    {
        // read in mesh
        fileName = config_baseDir.absolutePath().append("/").append(config_modelList.at(i));
        std::cout << fileName.toStdString() << std::endl;
        // load in the mesh and the mesh contains in the meshContainer
        meshContainer = new MeshModel(fileName,flog);

        // setup the rendering environment
        render = new Render(meshContainer,identity,identity,identity);
        render->resize(QSize(800,800));

        // It is necessary to show the window first
        render->show();

        // at the same time, create a folder named as the model containing the rendered images
        // the folder path is stored in imgManager->imgFolder
        imgManager->setImgFolder(this->config_baseDir,this->config_modelList[i]);

        // get the Uniformtransformation();
        auto campara = render->Uniformtransformation();
        float scale = campara.first;
        glm::mat4 shiftTransform = campara.second;
        glm::mat4 uniform = glm::scale(glm::mat4(1.f),glm::vec3(scale)) * shiftTransform;

        for(int index = 0;index < imgManager->len;index++)
        {
            glm::mat4 uniform_tmp = imgManager->getModelMatrix(index) * uniform;
            render->setMVP(uniform_tmp,
                           imgManager->getViewMatrix(index),
                           imgManager->getProjectiveMatrix(index));

            bool res = render->rendering();
            assert(res);

            render->storeRenderImage(imgManager->imgFolder,
                                     imgManager->getImageName(index),
                                     WIDTH_IMG,
                                     HEIGHT_IMG);
        }
//        flog << config_modelList.at(i).toStdString() << std::endl;

        delete meshContainer;
        meshContainer = NULL;
        delete render;
        render = NULL;

    }
    std::cout << "get frames done!" << std::endl;
}

void MeshModelHandler::loadInMatrix()
{
    imgManager = new ImageManager(this->config_matrixFile.absoluteFilePath());
}


