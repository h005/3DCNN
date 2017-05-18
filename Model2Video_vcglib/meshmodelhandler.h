#ifndef MESHMODELHANDLER_H
#define MESHMODELHANDLER_H

#include <iostream>
#include <QString>
#include <QDir>
#include <QStringList>
#include <opencv.hpp>
#include <GL/glew.h>
#include <QFileInfo>
#include "meshmodel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include "modellistmanager.h"
#include "render.hh"
#include "cameraparamanager.h"
#include "featureextractor.h"
#include "seamtrajectory.h"

class MeshModelHandler
{
public:
    ///
    /// \brief Handler
    /// \param configFile
    /// \param logFile
    ///
    /// read in the config file and log file
    /// configFile .ini file
    ///
        /// [path]
        /// baseDir=%your path%
        ///
        /// [model]
        /// model=modelListFile.mlt
        /// // the model list file, contains the model relative path (corresponding to baseDir), each model occupy one line
        ///     .mlt file conatins the modellist should be converted
        ///
        ///     model1.off
        ///     model2.off
        ///     ...
        ///     modeln.off
        ///
        /// [matrix]
        /// matrixFile=%your matrix%.matrix
        ///
        ///     matrix file contains the viewpoints you want to render
        ///     the same format as in viewpoint recommendation (3DfeatureCheck)
        ///
        /// [log]
        /// FeaturelogFile=/home/h005/Documents/3dcnn/data/ModelNet40Feature
        ///     // the "FeaturelogFile" is a folder, and this program will create a log file in this folder
        ///     // named as the [date].log, such as
        ///     // 20170510.log
        ///     // the log file records the mesh model has been processed for extracting the features, correctly
        ///
        ///
    ///
    /// logFile .log file
    ///     log the viewpoints has been rendered, in case of the
    ///     program will be crashed.
    ///
    ///     path=%the base path%
    ///     model=%the model%
    ///     id=%the id in matrix file%
    ///
    MeshModelHandler(QString configFileName, QString logFileName);

    MeshModelHandler();

    ~MeshModelHandler();

    void loadInMesh(QString fileName, MeshModel &meshModel);

    void generateFrames();

    void generateFeatures();

    void generateTrajectoryWithSeam();

    void clear();

private:

    void loadInMatrix();

    void render_initialRender();

    bool render_loadInMesh(int modelIndex);

    bool render_loadInMesh(QString fileName);

    void render_setModelViewProjectionMatrix(CameraParaManager *camParaManager, int index);

    void render_setModelViewProjectionMatrix(glm::mat4 &modelMatrix,
                                             glm::mat4 &viewMatrix,
                                             glm::mat4 &projectionMatrix);

    void folderExists(QString folder);

    void clearMeshContainer();

    void clearRender();

private:

    QStringList config_modelList;
    QDir config_baseDir;
    QFileInfo config_matrixFile;
    QFileInfo logFile;
    Render *render;
    FeatureExtractor *featureExtractor;
    MeshModel *meshContainer;
    std::fstream modelLog;
    QFileInfo featureLogFile;
    QString featureFolder;
    int modelFromId;
    int modelToId;

    int WIDTH_IMG = 800;
    int HEIGHT_IMG = 600;

    ModelListManager *modelListManager;
    CameraParaManager *camParaManager;
};

#endif // MESHMODELHANDLER_H
