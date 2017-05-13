#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QDir>
#include <iostream>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <QString>
#include <fstream>
/**
 * @brief The ImageManager class
 *
 * This class is created for handling the viewpoints to generate the video,
 * the viewpints is determined with the camear's parameters, i.e. the model-view and
 * the projection matrix
 */
class CameraParaManager
{
public:
    CameraParaManager();
    ~CameraParaManager();
    CameraParaManager(std::string fileName);
    CameraParaManager(QString fileName);
    int len;
    glm::mat4 &getModelMatrix(int index);
    glm::mat4 &getViewMatrix(int index);
    glm::mat4 &getProjectiveMatrix(int index);
    QString &getImageName(int index);

    void setImgFolder(QDir baseDir, QString model);
    void getFeafileHandler(QString basePath, QString model, std::fstream &feaout);
    QString imgFolder;

private:
    void clear();

    // matrix filenames
    std::string fileName;
    // the picture name of each pictures will be rendered
    std::vector< QString > img_names;
    std::vector< glm::mat4 > model_matrixList;
    std::vector< glm::mat4 > view_matrixList;
    std::vector< glm::mat4 > projection_matrixList;

    void loadInCameraParameters();
};

#endif // IMAGEMANAGER_H
