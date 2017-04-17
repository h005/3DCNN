#include "handler.h"
#include <QSettings>

#include <QDateTime>

Handler::Handler(QString configFileName, QString logFileName)
{
    QSettings configIni(configFileName,QSettings::IniFormat);

    // read in the baseDir
    config_baseDir = QDir(configIni.value("path/basePath").toString());

    // read in the model list
    QString modelsString = configIni.value("model/model").toString();
    config_modelList = modelsString.split(' ');

    // read in the matrix file
    config_matrixFile = QFileInfo(configIni.value("matrix/matrix").toString());

    QFileInfo logFile(logFileName);
    // if the file does not exist
    if (!logFile.exists())
    {
        // create the file as time.log
        QString tmpLogFileName = QDateTime::currentDateTime().toString();
        tmpLogFileName.replace(' ','_');
        tmpLogFileName.replace(':','_');
        logFile = QFileInfo(tmpLogFileName.append(".log"));
    }

    /* test for the config file read in
    qDebug() << config_baseDir.absolutePath() << endl;

    for(int i=0;i<config_modelList.size();i++)
        qDebug() << config_modelList.at(i) << endl;

    qDebug() << config_matrixFile.fileName() << endl;
    */
    // initial the render and the externalImporter
    render = NULL;
    exImporter = new ExternalImporter<MyMesh>();
}


Handler::rendering()
{
    QString fileName;
    // readin and render the models
    glm::mat4 identity;
    for(int i=0;i<config_modelList.size();i++)
    {
        // set up the rendering environment
        render = new Render(mesh,identity,identity,identity);
        render->resize(QSize(800,800));

        // I should check weather show the window is needed
        // render->show();
        fileName = config_baseDir.absolutePath().append(config_modelList.at(i));

        // load the mesh into memory
        // and calculate the normals
        loadInMesh(fileName);





    }




}


void Handler::loadInMesh(QString fileName)
{
    if(!exImporter->read_mesh(mesh,fileName.toStdString().c_str()));
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
}
