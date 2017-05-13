#include "featureextractor.h"

FeatureExtractor::FeatureExtractor(Render *render)
{
    this->render = render;
    this->imgHeight = 600;
    this->imgWidth = 800;

}

FeatureExtractor::~FeatureExtractor()
{
    image.release();
    contour.clear();
    for(int i=0;i<contour.size();i++)
        contour[i].clear();
    for(int i=0;i<featureContainer.size();i++)
        featureContainer[i].clear();
    contour.clear();
    featureContainer.clear();
}

void FeatureExtractor::printFeatures(std::fstream &feaOut, featureType id)
{
    switch(id)
    {
    case featureType::ProjectArea:
        feaOut << "ProjectArea" << std::endl;
        break;
    case featureType::VisSurfaceArea:
        feaOut << "VisSurfaceArea" << std::endl;
        break;
    case featureType::ViewpointEntropy:
        feaOut << "ViewpointEntropy" << std::endl;
        break;
    case featureType::SilhouetteLength:
        feaOut << "SilhouetteLength" << std::endl;
        break;
    case featureType::SilhouetteCurvature:
        feaOut << "SilhouetteCurvature" << std::endl;
        break;
    case featureType::SilhouetteCurvatureExtreme:
        feaOut << "SilhouetteCurvatureExtreme" << std::endl;
        break;
    case featureType::MaxDepth:
        feaOut << "MaxDepth" << std::endl;
        break;
    case featureType::DepthDistribute:
        feaOut << "DepthDistribute" << std::endl;
        break;
    case featureType::MeanCurvature:
        feaOut << "MeanCurvatue" << std::endl;
        break;
    case featureType::GaussianCurvature:
        feaOut << "GaussianCurvature" << std::endl;
        break;
    case featureType::AbovePreference:
        feaOut << "AbovePreference" << std::endl;
        break;
    }

    for( int i = 0; i < featureContainer[id].size(); i++)
        feaOut << featureContainer[id][i] << " ";
    feaOut << std::endl;

}

void FeatureExtractor::initExtractor()
{
    render->setAreaAllFaces();

    render->setParameters();

    this->setMat(render->p_img,
                 render->p_width,
                 render->p_height);
}

void FeatureExtractor::initFeatureContainer()
{
    // clear the container first
    for(int i=0;i<featureContainer.size();i++)
        featureContainer[i].clear();
    featureContainer.clear();
    // initial featureContainer
    for( int i = 0; i < featureType::ENDenum; i++)
        featureContainer.push_back( std::vector< double >());
}

///
/// \brief FeatureExtractor::setProjectArea
/// set the project area
///
void FeatureExtractor::setProjectArea()
{
    double res = 0.0;
    for (int i = 0 ; i < image.rows ; i++)
        for (int j = 0 ; j < image.cols ; j++)
            if(image.at<uchar>(i,j) != 255)
                res++;
    res /= double(imgHeight * imgWidth);
    featureContainer[featureType::ProjectArea].push_back(res);
//    std::cout << "set project area done" << std::endl;
    return;
}

///
/// \brief FeatureExtractor::setVisSurfaceArea
/// this function compute the visual surface area
///
void FeatureExtractor::setVisSurfaceArea()
{
    double res = 0.0;
    std::vector<GLfloat> &vertex = render->p_vertices;
    std::vector<GLuint> &face = render->p_VisibleFaces;
    double totalArea = render->areaAllFaces;

    for (int i=0; i<face.size(); i+=3)
    {
        CvPoint3D64f p1 = cvPoint3D64f(vertex[3*face[i]],vertex[3*face[i]+1],vertex[3*face[i]+2]);
        CvPoint3D64f p2 = cvPoint3D64f(vertex[3*face[i+1]],vertex[3*face[i+1]+1],vertex[3*face[i+1]+2]);
        CvPoint3D64f p3 = cvPoint3D64f(vertex[3*face[i+2]],vertex[3*face[i+2]+1],vertex[3*face[i+2]+2]);
        res += getArea3D(&p1, &p2, &p3);
    }
    assert(totalArea);
    res = res / totalArea;
    featureContainer[featureType::VisSurfaceArea].push_back(res);
//    std::cout << "set vis surface area done" << std::endl;
    return;
}

void FeatureExtractor::setViewpointEntropy()
{
    std::vector<GLfloat> &vertex = render->p_verticesMvp;
    std::vector<GLuint> &face = render->p_VisibleFaces;

    double res = 0.0;
    double area = 0.0;
    double totalArea = image.cols * image.rows;

    for ( int i = 0; i < face.size(); i += 3)
    {
        CvPoint2D64f a = cvPoint2D64f(vertex[face[i]*3],vertex[face[i]*3+1]);
        CvPoint2D64f b = cvPoint2D64f(vertex[face[i+1]*3],vertex[face[i+1]*3+1]);
        CvPoint2D64f c = cvPoint2D64f(vertex[face[i+2]*3],vertex[face[i+2]*3+1]);
        area = getArea2D(&a, &b, &c);

        if(area)
            res += area / totalArea * log2(area / totalArea);
    }

//    assert(featureContainer[featureType::ProjectArea]);

    if ( (totalArea - featureContainer[featureType::ProjectArea].back()) )
        res += ( totalArea - featureContainer[featureType::ProjectArea].back() ) / totalArea * log2( ( totalArea - featureContainer[featureType::ProjectArea].back() ) / totalArea );

    featureContainer[featureType::ViewpointEntropy].push_back(-res);

//    std::cout << "set viewpoint entropy done" << std::endl;

    return;
}

void FeatureExtractor::setSilhouetteLength()
{
    double res = 0.0;
    // ref http://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html
    cv::Mat gray;

    // ref http://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html?highlight=threshold#threshold
    // 这个一定要二值化，图像本身就基本都是白色，直接提取轮廓是拿不到结果的
    cv::threshold( image, gray, 254, 255.0, cv::THRESH_BINARY_INV);

    std::vector<cv::Vec4i> hierarchy;

    contour.clear();

    cv::findContours(gray, contour, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

    if( contour.size() )
    {
        for (int i=0; i< contour.size(); i++)
        {
            res += cv::arcLength(contour[i],true);
        }
    }
    else
        res = 0.0;

    // scale
    res = res / image.cols / image.rows;

    featureContainer[featureType::SilhouetteLength].push_back(res);
    // release the space
    std::vector<cv::Vec4i>().swap(hierarchy);

//    std::cout << "set SilhouetteLength done" << std::endl;

    return;
}

void FeatureExtractor::setSilhouetteCE()
{
    double resCurva = 0.0;
    double resCurvaExtreme = 0.0;
    double dis = 0.0;
    //    example
    //    abcdefghabcde
    //     ^  ->  ^
    //    abc -> bcd -> def
    for ( size_t k = 0; k < contour.size() ; k++)
    {
        for ( size_t i = 0; i < contour[k].size(); i++)
        {
            cv::Point a0 = contour[k][i];
            cv::Point b0 = contour[k][(i+1) % contour[k].size()];
            cv::Point c0 = contour[k][(i+2) % contour[k].size()];
            CvPoint2D64f a = cvPoint2D64f((double)a0.x, (double)a0.y);
            CvPoint2D64f b = cvPoint2D64f((double)b0.x, (double)b0.y);
            CvPoint2D64f c = cvPoint2D64f((double)c0.x, (double)c0.y);

            std::vector< cv::Point2d > points;
            points.push_back(cv::Point2d(a.x, a.y));
            points.push_back(cv::Point2d(b.x, b.y));
            points.push_back(cv::Point2d(c.x, c.y));

            dis = getDis2D(&a, &b) + getDis2D(&b, &c);

            double curvab = getContourCurvature(points,1);
            if(std::isnan(curvab))
            {
                std::cout << "curve apperas zero!" << std::endl;
            }
            else
            {
                resCurva += floatAbs(curvab);
                resCurvaExtreme += curvab * curvab;
            }
        }
    }

    featureContainer[featureType::SilhouetteCurvature].push_back(resCurva);
    featureContainer[featureType::SilhouetteCurvatureExtreme].push_back(resCurvaExtreme);

//    std::cout << "set SilhouetteCurvature done" << std::endl;
//    std::cout << "set SilhouetteCurvature Extreme done" << std::endl;

    return;
}

///
/// \brief FeatureExtractor::setMaxDepth
/// set the max depth feature the render->p_img is the depth array read from the depth buffer
///
void FeatureExtractor::setMaxDepth()
{
    float *array = render->p_img;
    int len = render->p_height*render->p_width;

    double res = -1.0;
    for(int i=0;i<len;i++)
        if(array[i] < 1.0)
            res = res > array[i] ? res : array[i];

    featureContainer[featureType::MaxDepth].push_back(res);

//    std::cout << "set Max depth done" << std::endl;

    return;
}

void FeatureExtractor::setDepthDistribute()
{
    float * zBuffer = render->p_img;
    int num = render->p_height * render->p_width;

    double res = 0.0;
    double min = 1.0;
    double max = -1.0;
    const int NumHistDepth = 20000;
    double *hist = new double[NumHistDepth];
    memset(hist, 0, sizeof(double)*NumHistDepth);
    for(int i=0;i<num;i++)
    {
        min = min > zBuffer[i] ? zBuffer[i] : min;
        if(zBuffer[i] < 1.0)
            max = max < zBuffer[i] ? zBuffer[i] : max;
    }

    double step = (max - min) / (double) NumHistDepth;

    if(step > 0.0)
    {
        // explan for if else below
        // such as min = 0 and  max = 15 then step = 1
        // so the hist is [0,1), [1,2), [2,3) ... [14,15)
        // max was omit!
        for(int i=0;i<num;i++)
        {
            if(zBuffer[i]==max)
                hist[NumHistDepth - 1]++;
            else if(zBuffer[i] < 1.0) // 数组越界错误
                hist[(int)((zBuffer[i]-min)/step)]++;
        }

        // normalizeHist
        normalizeHist(hist, step, NumHistDepth);

    }

    for(int i=0; i < NumHistDepth; i++)
        res += hist[i] * hist[i] * step;
    res = 1.0 - res;

    delete []hist;

    featureContainer[featureType::DepthDistribute].push_back(res);

//    std::cout << "set DepthDistribute done " << std::endl;

    return;

}

void FeatureExtractor::setMeanGaussianCurvature()
{
    std::pair<double,double> meanGaussianCurvature = \
            render->getMeanGaussianCurvature();
    featureContainer[featureType::MeanCurvature].push_back(meanGaussianCurvature.first / featureContainer[featureType::ProjectArea].back());
    featureContainer[featureType::GaussianCurvature].push_back(meanGaussianCurvature.second / featureContainer[featureType::ProjectArea].back());
//    std::cout << "set mean and Gaussian curvature done" << std::endl;
    return;
}

void FeatureExtractor::setAbovePreference()
{
    glm::vec4 z = glm::vec4(0.0, 0.0, 1.0, 0.0);

    glm::vec4 modelViewZ = render->m_view * render->m_model * z;

    // my camera position is 0,0,1,0
    // and the direction from the original point to my camera is 0,0,-1,0
    // the cameraDirection is determined by the matrix generate function in project of 3DfeatureCheck
    // in the file of fea.cc of the function of exportSBM()
    // and it is determined by the parameters of the glm::lookAt function of the camera's position
    glm::vec4 cameraDirection = glm::vec4(0.0, 1.0, 0.0, 0.0);
    double dotz = glm::dot(modelViewZ, cameraDirection);
    double cosThetaz = dotz / \
            glm::length(cameraDirection) / \
            glm::length(modelViewZ);
    double thetaz = acos(cosThetaz);
    double res = setAbovePreference(thetaz);
    featureContainer[featureType::AbovePreference].push_back(res);
//    std::cout << "set above preference done" << std::endl;
    return;
}

///
/// \brief FeatureExtractor::setTiltAngle
/// the camera's up direction tilt angles
///
void FeatureExtractor::setTiltAngle()
{

}




///
/// \brief FeatureExtractor::setMat set the Mask mat from the depth buffer of the OpenGL
/// \param img contains the depth info from the OpenGL depth buffer
/// \param width the width of the viewport
/// \param height the height of the viewport
/// \param dstWidth the dest width of the viewport
/// \param dstHeight the dest heigth of the viewport
///
void FeatureExtractor::setMat(float *img,
                              int width,
                              int height,
                              int dstWidth,
                              int dstHeight)
{
    imgWidth = dstWidth;
    imgHeight = dstHeight;
    float min = 1.0;
    for(int i=0;i<width * height; i++)
        min = min < img[i] ? min : img[i];

    cv::Mat imgtmp = cv::Mat(width, height, CV_32FC1, img);
    imgtmp.convertTo(image, CV_8UC1, 255.0 / (1.0 - min), 255.0 * min / (min - 1.0));

    // binary the image
    cv::threshold(image, image, 240, 255, CV_THRESH_BINARY);
    cv::flip(image, image, 0);
    cv::resize(image, image, cv::Size(dstWidth, dstHeight));

    // release the memory
    imgtmp.release();
}

double FeatureExtractor::getArea3D(CvPoint3D64f *a,
                                   CvPoint3D64f *b,
                                   CvPoint3D64f *c)
{
    CvPoint3D64f ab = cvPoint3D64f(b->x - a->x, b->y - a->y, b->z - a->z);
    CvPoint3D64f ac = cvPoint3D64f(c->x - a->x, c->y - a->y, c->z - a->z);
    double area = (ab.y*ac.z - ac.y*ab.z)*(ab.y*ac.z - ac.y*ab.z)
             + (ac.x*ab.z - ab.x*ac.z)*(ac.x*ab.z - ab.x*ac.z)
            + (ab.x*ac.y - ac.x*ab.y)*(ab.x*ac.y - ac.x*ab.y);
    area = sqrt(area);
    area /= 2.0;
    return area;
}

void FeatureExtractor::normalizeHist(double *hist, double step, int num)
{
    double area = 0.0;
    for(int i=0;i<num;i++)
        area += hist[i]*step;
    for(int i=0;i<num;i++)
        hist[i] /= area;
}

typedef long double LD;
double FeatureExtractor::getContourCurvature(const std::vector<cv::Point2d> &points,
                                             int target)
{
    assert(points.size() == 3);

    double T[3];
    for (int i = 0; i < 3; i++) {
        double t = cv::norm(points[target] - points[i]);
        T[i] = target < i ? t : -t;
    }
    cv::Mat M(3, 3, CV_64F);
    for (int i = 0; i < 3; i++) {
        M.at<double>(i, 0) = 1;
        M.at<double>(i, 1) = T[i];
        M.at<double>(i, 2) = T[i] * T[i];
    }
    cv::Mat invM = M.inv();

    cv::Mat X(3, 1, CV_64F), Y(3, 1, CV_64F);
    for (int i = 0; i < 3; i++) {
        X.at<double>(i, 0) = points[i].x;
        Y.at<double>(i, 0) = points[i].y;
    }

    cv::Mat a, b;
    a = invM * X;
    b = invM * Y;

    LD up = (LD)2 * (a.at<double>(1, 0) * b.at <double>(2, 0) - a.at<double>(2, 0) * b.at <double>(1, 0));
    LD down = pow((LD)a.at<double>(1, 0) * a.at<double>(1, 0) + (LD)b.at <double>(1, 0) * b.at <double>(1, 0), 1.5);
    LD frac = up / down;

    return (double)frac;
}

double FeatureExtractor::getArea2D(CvPoint2D64f *a,
                                   CvPoint2D64f *b,
                                   CvPoint2D64f *c)
{
    CvPoint2D64f ab = cvPoint2D64f(b->x - a->x, b->y - a->y);
    CvPoint2D64f ac = cvPoint2D64f(c->x - a->x, c->y - a->y);
    double area = ab.x * ac.y - ab.y * ac.x;
    area = area > 0 ? area : -area;
    area /= 2.0;
    return area;
}

double FeatureExtractor::getDis2D(CvPoint2D64f *a, CvPoint2D64f *b)
{
    double dx = (a->x - b->x);
    double dy = (a->y - b->y);
    return sqrt(dx * dx + dy * dy);
}

float FeatureExtractor::floatAbs(float num)
{
    return num < 0 ? -num : num;
}

double FeatureExtractor::doubleAbs(double num)
{
    return num < 0 ? -num : num;
}

double FeatureExtractor::setAbovePreference(double theta)
{
    double res = 0.0;
    double pi = asin(1.0)*2.0;
    res = exp(-(theta - pi/8.0*3.0)*(theta - pi/8.0*3.0)
                          / pi/4.0*pi/4.0);
    return res;
}
