#ifndef SEAMTRAJECTORY_H
#define SEAMTRAJECTORY_H

#include <opencv2/opencv.hpp>
#include <QString>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

class SeamTrajectory
{
public:
    SeamTrajectory();

    void clear();

    cv::Mat_<cv::Vec3b> image;
    cv::Mat_<cv::Vec3b> duplicate;
    cv::Mat energy;

    // Compute the full energy matrix by scanning the whole image
    void computeFullEnergy();

    // Compute the energy function only for the pixels affected by seam-removal
    void computeEnergyAfterSeamRemoval(std::vector<uint> seam);

    // set the img as the full energy map
    void setEnergyMap();

    void generateTrajectoryMatrix(QString basePath,
                                  QString model,
                                  QString feaName,
                                  std::vector<uint> seam);

public:
    SeamTrajectory(cv::Mat_<cv::Vec3b> im)
    {
        image = im;
        duplicate = image;
        energy = cv::Mat(image.rows,
                         image.cols,
                         CV_32S,
                         cv::Scalar(195075));
    // in my program there is no need to compute the full energy
//        computeFullEnergy();
        setEnergyMap();
    }

    SeamTrajectory(QString basePath,
                   QString model,
                   QString feaName);

    ~SeamTrajectory(){
        image.release();
        duplicate.release();
        energy.release();
    }

    // Show the image
    void showImage()
    {
        cv::imshow("image", image);
        cv::waitKey(0);
    }

    // Show the energy matrix as an image
    void showEnergy(){
        cv::imshow("energy", energy);
    }

    // return the image
    cv::Mat_<cv::Vec3b> getImage()
    {
        return image;
    }

    // return  the value of the energy at a particular pixel
    unsigned int getEnergy(unsigned int row, unsigned int col)
    {
        return energy.at<uint32_t>(row, col);
    }

    // Find the optimal seams
    std::vector<uint> findVerticalSeam();
    std::vector<uint> findHorizontalSeam();

    // Find the optimal seams with the max energy
    std::vector<uint> findVerticalMaxSeam();
    std::vector<uint> findHorizontalMaxSeam();

    // Remove a given seam and update the image
    void removeVerticalSeam(std::vector<uint> seam);
    void removeHorizontalSeam(std::vector<uint> seam);

    // Display a given seam
    void showVerticalSeam(std::vector<uint> seam);
    void showHorizontalSeam(std::vector<uint> seam);

private:

    glm::mat4 normalizeModelView(const glm::mat4 &mvMatrix);

};

#endif // SEAMTRAJECTORY_H
