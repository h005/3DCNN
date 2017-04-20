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
        /// model="yourmodel1 yourmodel2 yourmodel3 ... yourmodeln"; split with space ' '
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

    ImageManager *imgManager;
};

#endif // HANDLER_H
