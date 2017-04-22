#include "handler.h"
#include "meancurvature.hh"
#include "gausscurvature.hh"
#include <QSettings>

#include <QDateTime>

Handler::Handler(QString configFileName, QString logFileName)
{
    QSettings configIni(configFileName,QSettings::IniFormat);

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
//    if (!logFile.exists())
    if(logFileName == "")
    {
        // create the file as time.log
        QString tmpLogFileName = QDateTime::currentDateTime().toString();
        tmpLogFileName.replace(' ','_');
        tmpLogFileName.replace(':','_');
        logFile = QFileInfo(tmpLogFileName.append(".log"));
    }

    // create the log file
    flog.open(logFile.absoluteFilePath().toStdString(),std::fstream::out);
    assert(flog);

    // initial the render and the externalImporter
    render = NULL;
    exImporter = NULL;
    imgManager = NULL;
    // load the matrix file
    loadInMatrix();
}

Handler::~Handler()
{
    clear();
}


void Handler::rendering()
{
    QString fileName;
    // readin and render the models
    glm::mat4 identity;
    for(int i=0;i<config_modelList.size();i++)
    {
        MyMesh mesh;
        fileName = config_baseDir.absolutePath().append("/").append(config_modelList.at(i));
        std::cout << fileName.toStdString() << std::endl;
        // load the mesh into memory
        // and calculate the normals
        exImporter = new ExternalImporter<MyMesh>();
        loadInMesh(fileName,mesh);

        // set up the rendering environment
        render = new Render(mesh,identity,identity,identity);
        render->resize(QSize(800,800));

        // It is necessary to show the window first
        render->show();

        // at the same time, create a folder named as the model containing the rendered images.
        // the folder path is stored in imgManager->imgFolder
        imgManager->setImgFolder(this->config_baseDir, this->config_modelList[i]);

        exImporter->setUnformTransformation();
        float scale = exImporter->scale;
        glm::mat4 shiftTransform = exImporter->shiftTransform;
        glm::mat4 uniform = glm::scale(glm::mat4(1.f), glm::vec3(scale)) * shiftTransform;

        // render each viewpoints for the model
        for(int index=0;index<imgManager->len;index++)
        {
            glm::mat4 uniform_tmp = imgManager->getModelMatrix(index) * uniform;
            render->setMVP(uniform_tmp,
                           imgManager->getViewMatrix(index),
                           imgManager->getProjectiveMatrix(index));
//            render->setMVP(imgManager->getModelMatrix(index),
//                           imgManager->getViewMatrix(index),
//                           imgManager->getProjectiveMatrix(index));

            bool res = render->rendering();
            assert(res);

            render->storeRenderImage(imgManager->imgFolder,
                                     imgManager->getImageName(index),
                                     WIDTH_IMG,
                                     HEIGHT_IMG);

        }
        flog << config_modelList.at(i).toStdString() << std::endl;

        mesh.clear();
//        render->close();
        delete render;
        render = NULL;
        delete exImporter;
        exImporter = NULL;
    }




}

/**
  This function is used for initialize the features environment.
 * @brief Handler::setFeatrueInit
 * @param mode set mode to determine which kind of features should be extracted.
 */
void Handler::setFeatrueInit(int mode)
{
    std::vector< MeanCurvature< MyMesh >* > a;
    std::vector< GaussCurvature< MyMesh >* > b;
    if(mode  == 3 || mode  == 0)
    {
        render->setMeshSaliencyPara(exImporter);
        for(int i=0;i<render->p_vecMesh.size();i++)
        {
            MeanCurvature<MyMesh> *tmpMean = new MeanCurvature<MyMesh>(render->p_vecMesh[i]);
            GaussCurvature<MyMesh> *tmpGauss = new GaussCurvature<MyMesh>(render->p_vecMesh[i]);
            a.push_back(tmpMean);
            b.push_back(tmpGauss);
        }
    }
    render->setAreaAllFaces();

}

void Handler::loadInMatrix()
{
    imgManager = new ImageManager(this->config_matrixFile.absoluteFilePath());
}

void Handler::clear()
{
    if(exImporter)
    {
        delete exImporter;
        exImporter = NULL;
    }
    if(render)
    {
        delete render;
        render = NULL;
    }
    if(imgManager)
    {
        delete imgManager;
        imgManager = NULL;
    }
}


void Handler::loadInMesh(QString fileName,MyMesh &mesh)
{
    if(!exImporter->read_mesh(mesh,fileName.toStdString().c_str()))
    {
        std::cerr << "Error: Cannot read mesh from " << fileName.toStdString() << std::endl;
        return;
    }

    // if the file did not provide the vertex normals, then calculate them
    if(!mesh.has_vertex_normals())
    {
        std::cout << "we need to calculate the vertex normal first"<< std::endl;
        // allocate memory for normals storage
        // we need face normals to update the vertex normals
        mesh.request_face_normals();
        mesh.request_vertex_normals();

        // let the mesh update the normals
        mesh.update_normals();
        // dispose the face normals, as we don't need them anymore
        mesh.release_face_normals();
    }
    std::cout << fileName.toStdString() << " loaded in done!" << std::endl;
}
