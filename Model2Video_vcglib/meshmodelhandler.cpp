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
    modelFromId = configIni.value("model/fromId").toInt();
    modelToId = configIni.value("model/toId").toInt();
    modelToId = modelToId < config_modelList.size() ? modelToId : config_modelList.size();

    // read in the matrix file
    config_matrixFile = QFileInfo(configIni.value("matrix/matrixFile").toString());

    // read in the FeaturelogFile
    QString featureLog = configIni.value("log/FeaturelogFile").toString();
    QString tmpFeatureLogFileName = QDateTime::currentDateTime().toString("yyyyMMdd");
    // check the folder and create
    folderExists(featureLog);
    // generate the feautre log file
    featureLogFile = featureLog + "/" + tmpFeatureLogFileName + ".log";
    // feature folder
    featureFolder = featureLog;

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

    modelLog.open(logFile.absoluteFilePath().toStdString(), std::fstream::out);

    assert(modelLog);

//    render = NULL;
    meshContainer = NULL;
    camParaManager = NULL;
    featureExtractor = NULL;
    // load the matrix file
    loadInMatrix();

}

MeshModelHandler::MeshModelHandler()
{
    meshContainer = NULL;
    modelListManager = NULL;
    camParaManager = NULL;
}

MeshModelHandler::~MeshModelHandler()
{

}

void MeshModelHandler::generateFrames()
{
    QString fileName;

    render_initialRender();

//    for (int i=0;i<config_modelList.size(); i++)
    for(int i = modelFromId; i < modelToId; i++)
    {
        // read in mesh
        fileName = config_baseDir.absolutePath().append("/").append(config_modelList.at(i));
        std::cout << fileName.toStdString() << std::endl;
        // load in the mesh and the mesh contains in the meshContainer
        meshContainer = new MeshModel(fileName,modelLog);

        render->setMeshModel(meshContainer);

        // at the same time, create a folder named as the model containing the rendered images
        // the folder path is stored in imgManager->imgFolder
        camParaManager->setImgFolder(this->config_baseDir,
                                     this->config_modelList[i]);

        // get the Uniformtransformation();
        auto campara = render->Uniformtransformation();
        float scale = campara.first;
        glm::mat4 shiftTransform = campara.second;
        glm::mat4 uniform = glm::scale(glm::mat4(1.f),glm::vec3(scale)) * shiftTransform;

        for(int index = 0;index < camParaManager->len;index++)
        {
            glm::mat4 uniform_tmp = camParaManager->getModelMatrix(index) * uniform;
            render->setMVP(uniform_tmp,
                           camParaManager->getViewMatrix(index),
                           camParaManager->getProjectiveMatrix(index));

            bool res = render->rendering();
            assert(res);

            render->storeRenderImage(camParaManager->imgFolder,
                                     camParaManager->getImageName(index),
                                     WIDTH_IMG,
                                     HEIGHT_IMG);
        }
//        flog << config_modelList.at(i).toStdString() << std::endl;

        delete meshContainer;
        meshContainer = NULL;
    }
    delete render;
    render = NULL;
    std::cout << "get frames done!" << std::endl;
}

void MeshModelHandler::generateFeatures()
{

    render_initialRender();

    featureExtractor = new FeatureExtractor(render);

//    for (int modelIndex=0; modelIndex < config_modelList.size(); modelIndex++)
    for (int modelIndex = modelFromId; modelIndex < modelToId; modelIndex++)
    {
        // load in mesh to the MeshContainer
        render_loadInMesh(modelIndex);

//        // at the same time, create a folder named as the model containing the rendered images
//        // the folder path is stored in imageManager->imgFolder
//        imgManager->setImgFolder(this->config_baseDir, this->config_modelList[modelIndex]);

        // create the .fea file for each model
        std::fstream feaOut;
        camParaManager->getFeafileHandler(this->featureFolder,
                                          this->config_modelList[modelIndex],
                                          feaOut);

        camParaManager->setImgFolder(this->config_baseDir,
                                     this->config_modelList[modelIndex]);

        featureExtractor->initFeatureContainer();

        for (int index = 0; index < camParaManager->len; index++)
        {
            render_setModelViewProjectionMatrix(camParaManager, index);

            // rendering
            bool res = render->rendering();
            assert(res);

            render->storeRenderImage(camParaManager->imgFolder,
                                     camParaManager->getImageName(index),
                                     WIDTH_IMG,
                                     HEIGHT_IMG);

            featureExtractor->initExtractor();
            featureExtractor->setProjectArea();
            featureExtractor->setVisSurfaceArea();
            featureExtractor->setViewpointEntropy();
            featureExtractor->setSilhouetteLength();
            featureExtractor->setSilhouetteCE();
            featureExtractor->setMaxDepth();
            featureExtractor->setDepthDistribute();
            featureExtractor->setMeanGaussianCurvature();
            featureExtractor->setAbovePreference();
        }

        featureExtractor->printFeatures(feaOut, featureExtractor->featureType::ProjectArea);
        featureExtractor->printFeatures(feaOut, featureExtractor->featureType::VisSurfaceArea);
        featureExtractor->printFeatures(feaOut, featureExtractor->featureType::ViewpointEntropy);
        featureExtractor->printFeatures(feaOut, featureExtractor->featureType::SilhouetteLength);
        featureExtractor->printFeatures(feaOut, featureExtractor->featureType::SilhouetteCurvature);
        featureExtractor->printFeatures(feaOut, featureExtractor->featureType::SilhouetteCurvatureExtreme);
        featureExtractor->printFeatures(feaOut, featureExtractor->featureType::MaxDepth);
        featureExtractor->printFeatures(feaOut, featureExtractor->featureType::DepthDistribute);
        featureExtractor->printFeatures(feaOut, featureExtractor->featureType::MeanCurvature);
        featureExtractor->printFeatures(feaOut, featureExtractor->featureType::GaussianCurvature);
        featureExtractor->printFeatures(feaOut, featureExtractor->featureType::AbovePreference);

        feaOut.close();

        clearMeshContainer();

    }

    clearRender();

    std::cout << "generate features done!" << std::endl;;
}

void MeshModelHandler::loadInMatrix()
{
    camParaManager = new CameraParaManager(this->config_matrixFile.absoluteFilePath());
}

void MeshModelHandler::render_loadInMesh(int modelIndex)
{
    QString fileName = config_baseDir.absolutePath().append("/").append(config_modelList.at(modelIndex));
    std::cout << fileName.toStdString() << std::endl;

    render_loadInMesh(fileName);

}

void MeshModelHandler::render_loadInMesh(QString fileName)
{
    // load in the mesh into the meshContainer
    meshContainer = new MeshModel(fileName, modelLog);

    render->setMeshModel( meshContainer );
}

void MeshModelHandler::render_setModelViewProjectionMatrix(CameraParaManager *imgManager,
                                                           int index)
{

    render_setModelViewProjectionMatrix(imgManager->getModelMatrix(index),
                                        imgManager->getViewMatrix(index),
                                        imgManager->getProjectiveMatrix(index));

}

void MeshModelHandler::render_setModelViewProjectionMatrix(glm::mat4 &modelMatrix,
                                                           glm::mat4 &viewMatrix,
                                                           glm::mat4 &projectionMatrix)
{
    // get the Uniformatransformation();
    auto campara = render->Uniformtransformation();
    float scale = campara.first;
    glm::mat4 shiftTransform = campara.second;
    glm::mat4 uniform = glm::scale(glm::mat4(1.f), glm::vec3(scale)) * shiftTransform;

    glm::mat4 newModelMatrix = modelMatrix * uniform;
    render->setMVP(newModelMatrix,
                   viewMatrix,
                   projectionMatrix);

}

void MeshModelHandler::folderExists(QString folder)
{
    int posOfFolder = folder.lastIndexOf('/');
    QString folderName = folder.right(folder.length() - 1 - posOfFolder);
    QDir dir(folder.left(posOfFolder));
    if(!dir.exists(folderName))
        assert(dir.mkdir(folderName));
}

void MeshModelHandler::clearMeshContainer()
{
    delete meshContainer;
    meshContainer = NULL;
}

void MeshModelHandler::clearRender()
{
    delete render;
    render = NULL;
}



void MeshModelHandler::render_initialRender()
{
    glm::mat4 identity;

    // setup the rendering environment
    render = new Render(identity,identity,identity);
    render->resize(QSize(800,800));

    // It is necessary to show the window first
    render->show();
}


