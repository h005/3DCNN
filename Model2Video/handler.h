#ifndef HANDLER_H
#define HANDLER_H

#include <QString>
#include <QDir>
#include <QStringList>
#include <opencv.hpp>
#include <GL/glew.h>
#include <QFileInfo>
#include "common.hh"
#include "render.hh"
#include "externalimporter.hh"
#include <iostream>
#include <QDebug>
#include "render.hh"
#include <string>
#include "imagemanager.h"
#include <fstream>
#include <modellistmanager.h>

class Handler
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
    Handler(QString configFileName, QString logFileName);

    ~Handler();

    void loadInMesh(QString fileName, MyMesh &mesh);

    void rendering();

private:

    void setFeatrueInit(int mode = 0);

    void loadInMatrix();

    void clear();

private:

    // Initial the mesh reader
    ExternalImporter<MyMesh> *exImporter;
    QStringList config_modelList;
    QDir config_baseDir;
    QFileInfo config_matrixFile;
    QFileInfo logFile;
    Render *render;
    std::fstream flog;

    int WIDTH_IMG = 800;
    int HEIGHT_IMG = 600;

    ModelListManager *modelListManager;
    ImageManager *imgManager;
};

#endif // HANDLER_H
