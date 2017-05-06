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
#include "imagemanager.h"

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

    void clear();

private:

    void loadInMatrix();

private:

    QStringList config_modelList;
    QDir config_baseDir;
    QFileInfo config_matrixFile;
    QFileInfo logFile;
    Render *render;
    MeshModel *meshContainer;
    std::fstream flog;

    int WIDTH_IMG = 800;
    int HEIGHT_IMG = 600;

    ModelListManager *modelListManager;
    ImageManager *imgManager;
};

#endif // MESHMODELHANDLER_H
