#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include "render.hh"
#include <fstream>
#include <opencv.hpp>
#include <map>

class FeatureExtractor
{
public:
    enum featureType{ProjectArea,
                    VisSurfaceArea,
                    ViewpointEntropy,
                    SilhouetteLength,
                    SilhouetteCurvature,
                    SilhouetteCurvatureExtreme,
                    MaxDepth,
                    DepthDistribute,
                    MeanCurvature,
                    GaussianCurvature,
                    AbovePreference,
                    TiltAngle,
                    BoundingBoxAbs,
                    BallCoord,
                    ENDenum};

public:
    FeatureExtractor();

    FeatureExtractor(Render *render);

    ~FeatureExtractor();

    QString getFeaName(featureType id);

    void printFeatures(std::fstream &feaOut, featureType id);

    void parseToImg(QString basePath, QString model, featureType id);

    void setFeatures();

    void printFeatures(std::fstream &feaOut);

    void initExtractor();

    void initFeatureContainer();

    void setProjectArea();

    void setVisSurfaceArea();

    void setViewpointEntropy();

    void setSilhouetteLength();

    void setSilhouetteCE();

    void setMaxDepth();

    void setDepthDistribute();

    void setMeanGaussianCurvature();

    void setAbovePreference();

    void setTiltAngle();

    void setBoundingBoxAbs();

    void setBallCoord();

private:

    void setMat(float *img,
                int width,
                int height,
                int dstWidth = 800,
                int dstHeight = 600);

    double getArea3D(CvPoint3D64f *a,
                     CvPoint3D64f *b,
                     CvPoint3D64f *c);

    void normalizeHist(double *hist,
                       double step,
                       int num);

    double getContourCurvature(const std::vector<cv::Point2d> &points,
                               int target);

    double getArea2D(CvPoint2D64f *a,
                     CvPoint2D64f *b,
                     CvPoint2D64f *c);

    double getDis2D(CvPoint2D64f *a,
                    CvPoint2D64f *b);

    float floatAbs(float num);

    double doubleAbs(double num);

    double setAbovePreference(double theta);

    void parseToImg(std::vector<double> fea,
                    QString heatMapName,
                    QString energyMapName);

    void initFeaNameList();

//    QString feaFileName;
//    QStringList feaNameList;
    std::vector<QString> feaNameList;
    Render *render;
    cv::Mat image;
    int imgWidth,imgHeight;
    std::vector< std::vector<cv::Point> > contour;
    std::vector< std::vector<double> > featureContainer;
};

#endif // FEATUREEXTRACTOR_H
