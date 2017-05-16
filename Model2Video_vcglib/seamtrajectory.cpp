#include "seamtrajectory.h"

SeamTrajectory::SeamTrajectory()
{

}

void SeamTrajectory::clear()
{
    image.release();
    duplicate.release();
    energy.release();
}

void SeamTrajectory::computeFullEnergy()
{
    //Ensure that the size of the energy matrix matches that of the image
    energy.create(image.rows, image.cols, CV_32S);

    // filling zeros into the rows bounds
    for(int i=0;i < image.cols; i++)
    {
        energy.at<uint32_t>(0,i) = 0;
        energy.at<uint32_t>(image.rows - 1, i) = 0;
    }
    // filling zeros into the cols bounds
    for(int i=0;i<image.rows;i++)
    {
        energy.at<uint32_t>(i,0) = 0;
        energy.at<uint32_t>(i,image.cols - 1) = 0;
    }


    //Scan through the image and update the energy values. Ignore boundary pixels.
    for (int i = 1; i < image.rows-1; ++i) {
        uchar* prev = image.ptr<uchar>(i-1);	//Pointer to previous row
        uchar* curr = image.ptr<uchar>(i);		//Pointer to current row
        uchar* next = image.ptr<uchar>(i+1);	//Pointer to next row

        for (int j = 1; j < image.cols-1; ++j) {
            int val = 0;
            //Energy along the x-axis
            val += (prev[3*j]-next[3*j]) * (prev[3*j]-next[3*j]);
            val += (prev[3*j+1]-next[3*j+1]) * (prev[3*j+1]-next[3*j+1]);
            val += (prev[3*j+2]-next[3*j+2]) * (prev[3*j+2]-next[3*j+2]);

            //Energy along the y-axis
            val += (curr[3*j+3]-curr[3*j-3]) * (curr[3*j+3]-curr[3*j-3]);
            val += (curr[3*j+4]-curr[3*j-2]) * (curr[3*j+4]-curr[3*j-2]);
            val += (curr[3*j+5]-curr[3*j-1]) * (curr[3*j+5]-curr[3*j-1]);

            energy.at<uint32_t>(i, j) = val;
        }
    }
}

void SeamTrajectory::computeEnergyAfterSeamRemoval(std::vector<uint> seam)
{
    cv::Mat tmp = cv::Mat(image.rows, image.cols, CV_32S, cv::Scalar(195075));
    for (unsigned int row = 0; row < (uint)image.rows; ++row) {
        for (unsigned int col = 0; col < (uint)image.cols; ++col) {
            if (col < seam[row]-1)	tmp.at<uint32_t>(row, col) = energy.at<uint32_t>(row, col);
            if (col > seam[row])	tmp.at<uint32_t>(row, col) = energy.at<uint32_t>(row, col+1);
            if (col == seam[row] || col == seam[row]-1) {
                cv::Vec3b l = image.at<cv::Vec3b>(row, col-1);
                cv::Vec3b r = image.at<cv::Vec3b>(row, col+1);
                cv::Vec3b u = image.at<cv::Vec3b>(row-1, col);
                cv::Vec3b d = image.at<cv::Vec3b>(row+1, col);
                int val = (l[0]-r[0])*(l[0]-r[0]) + (l[1]-r[1])*(l[1]-r[1]) + (l[2]-r[2])*(l[2]-r[2]) +
                        (u[0]-d[0])*(u[0]-d[0]) + (u[1]-d[1])*(u[1]-d[1]) + (u[2]-d[2])*(u[2]-d[2]);
                tmp.at<uint32_t>(row, col) = val;
            }
        }
    }
    energy = tmp;
}

void SeamTrajectory::setEnergyMap()
{
    cv::Mat tmp;
    cv::cvtColor(image,tmp,cv::COLOR_BGR2GRAY);
    for(int i=0;i<tmp.rows;i++)
        for(int j=0;j<tmp.cols;j++)
            energy.at<uint32_t>(i,j) = tmp.at<uchar>(i,j);
    tmp.release();
}

void SeamTrajectory::generateTrajectoryMatrix(QString basePath,
                                              QString model,
                                              QString feaName,
                                              std::vector<uint> seam)
{
    model.replace('/','_');
    int pos = model.lastIndexOf('.');
    model = model.left(pos);
    model = model + "_" + feaName;
    QString matrixFile = basePath + '/' + model + ".matrix";

    // matrix file output handler
    std::fstream fout;
    fout.open(matrixFile.toStdString(), std::fstream::out);

    glm::mat4 mv;

    glm::mat4 proj = glm::perspective(glm::pi<float>() / 3, 4.0f / 3.0f, 0.5f, 10.f);

    float angle_x = glm::pi<float>() / image.rows;
    float angle_z = 2.0 * glm::pi<float>() / image.cols;

    glm::mat4 m_camera = glm::lookAt(glm::vec3(0.f, 0.f, 1.5f),
                                     glm::vec3(0.f, 0.f, 0.f),
                                     glm::vec3(0.f, 1.f, 0.f));

    int ind = 1;
    for (int i=0; i < image.cols; i++)
    {

        float anglex = - angle_x * seam[i];
        float anglez = - angle_z * i + glm::pi<float>() / 2.0;
        glm::mat4 rotateX = glm::rotate(glm::mat4(1.f),
                                        anglex,
                                        glm::vec3(1.0,0.0,0.0));
        glm::mat4 rotateZ = glm::rotate(glm::mat4(1.f),
                                        anglez,
                                        glm::vec3(0.0,0.0,1.0));

        mv = m_camera * rotateX * rotateZ;
        mv = normalizeModelView(mv);

//      photo id
//      fout << seam[i] * 32 + i  + 1 << ".jpg" << std::endl;

        fout.width(6);
        fout.fill('0');
        fout << ind++ << ".jpg" << std::endl;

        // mv matrix
        for(int k1 = 0;k1 < 4;k1++)
        {
            for(int k2 = 0;k2 < 4;k2++)
                fout << mv[k2][k1] << " ";
            fout << std::endl;
        }
        // proj matrix
        for(int k1 = 0;k1 < 4;k1++)
        {
            for(int k2 = 0;k2 < 4;k2++)
                fout << proj[k2][k1] << " ";
            fout << std::endl;
        }
    }

    std::cout << "export to " << matrixFile.toStdString() << std::endl;
}

SeamTrajectory::SeamTrajectory(QString basePath, QString model, QString feaName)
{
    // get the energyMap and the heatMap
    model.replace('/','_');
    int pos = model.lastIndexOf('.');
    model = model.left(pos);
    model = model + "_" + feaName;
    QString energyMapName = basePath + '/' + model + "_e.jpg";

    image = cv::imread(energyMapName.toStdString());
    duplicate = image;
    energy = cv::Mat(image.rows,
                     image.cols,
                     CV_32S,
                     cv::Scalar(195075));
// in my program there is no need to compute the full energy
//        computeFullEnergy();
    setEnergyMap();
}

std::vector<uint> SeamTrajectory::findVerticalSeam()
{
    std::vector<uint> seam(image.rows);
    unsigned int distTo[image.rows][image.cols];	//Save the shortest distance from any of the top pixels
    short edgeTo[image.rows][image.cols];			//Which of the the three top pixels, the shortest path came from

    //Initialize the distance and edge matrices
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            if (i == 0)		distTo[i][j] = 0;
            else			distTo[i][j] = std::numeric_limits<unsigned int>::max();
            edgeTo[i][j] = 0;
        }
    }

    // Relax the edges in topological order
    for (int row = 0; row < image.rows-1; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            //Check the pixel to the bottom-left
            if (col != 0)
                if (distTo[row+1][col-1] > distTo[row][col] + getEnergy(row+1, col-1)) {
                    distTo[row+1][col-1] = distTo[row][col] + getEnergy(row+1, col-1);
                    edgeTo[row+1][col-1] = 1;
                }
            //Check the pixel right below
            if (distTo[row+1][col] > distTo[row][col] + getEnergy(row+1, col)) {
                distTo[row+1][col] = distTo[row][col] + getEnergy(row+1, col);
                edgeTo[row+1][col] = 0;
            }
            //Check the pixel to the bottom-right
            if (col != image.cols-1)
                if (distTo[row+1][col+1] > distTo[row][col] + getEnergy(row+1, col+1)) {
                    distTo[row+1][col+1] = distTo[row][col] + getEnergy(row+1, col+1);
                    edgeTo[row+1][col+1] = -1;
                }
        }
    }

    //Find the bottom of the min-path
    unsigned int min_index = 0, min = distTo[image.rows-1][0];
    for (int i = 1; i < image.cols; ++i)
        if (distTo[image.rows-1][i] < min) {
            min_index = i;
            min = distTo[image.rows-1][i];
        }

    //Retrace the min-path and update the 'seam' vector
    seam[image.rows-1] = min_index;
    for (int i = image.rows-1; i > 0; --i)
        seam[i-1] = seam[i] + edgeTo[i][seam[i]];

    return seam;
}

std::vector<uint> SeamTrajectory::findHorizontalSeam()
{
    std::vector<uint> seam(image.cols);
    //Transpose the matrices and find the vertical seam
    cv::transpose(image, image);
    cv::transpose(energy, energy);
    seam = findVerticalSeam();

    //Transpose back
    cv::transpose(image, image);
    cv::transpose(energy, energy);
    return seam;
}

std::vector<uint> SeamTrajectory::findVerticalMaxSeam()
{
    std::vector<uint> seam(image.rows);
    unsigned int distTo[image.rows][image.cols];
    short edgeTo[image.rows][image.cols];

    //Initialize the distance and edge matrices
    for (int i=0; i< image.rows; i++)
    {
        for (int j=0; j< image.cols; j++)
        {
            if( i==0 )
                distTo[i][j] = 0;
            else
                distTo[i][j] = std::numeric_limits<unsigned int>::min();
            edgeTo[i][j] = 0;
        }
    }

    // Relax the edges in topological order
    for ( int row = 0; row < image.rows - 1; ++row)
    {
        for (int col = 0; col < image.cols; ++col)
        {
            if(col != 0)
                if (distTo[row+1][col-1] <= distTo[row][col] + getEnergy(row+1, col-1))
                {
                    distTo[row+1][col-1] = distTo[row][col] + getEnergy(row+1, col-1);
                    edgeTo[row+1][col-1] = 1;
                }
            // Check the pixel right below
            if (distTo[row+1][col] <= distTo[row][col] + getEnergy(row+1,col))
            {
                distTo[row+1][col] = distTo[row][col] + getEnergy(row+1, col);
                edgeTo[row+1][col] = 0;
            }
            // Check the pixel to the bottom-right
            if (col != image.cols - 1)
            {
                if (distTo[row+1][col+1] <= distTo[row][col] + getEnergy(row+1,col+1))
                {
                    distTo[row+1][col+1] = distTo[row][col] + getEnergy(row+1,col+1);
                    edgeTo[row+1][col+1] = -1;
                }
            }

        }
    }

    // Find the bottom of the min-path
    unsigned int max_index = 0, max = distTo[image.rows - 1][0];
    for (int i=1; i< image.cols; ++i)
    {
        if(distTo[image.rows - 1][i] > max)
        {
            max_index = i;
            max = distTo[image.rows - 1][i];
        }
    }

    // Retrace the max-path and update the 'seam' vector
    seam[image.rows - 1] = max_index;
    for(int i=image.rows - 1; i>0;--i)
        seam[i-1] = seam[i] + edgeTo[i][seam[i]];

    return seam;
}

std::vector<uint> SeamTrajectory::findHorizontalMaxSeam()
{
    std::vector<uint> seam(image.cols);
    //Transpose the matrices and find the vertical seam
    cv::transpose(image, image);
    cv::transpose(energy, energy);
    seam = findVerticalMaxSeam();

    //Transpose back
    cv::transpose(image, image);
    cv::transpose(energy, energy);
    return seam;
}

void SeamTrajectory::removeVerticalSeam(std::vector<uint> seam)
{
    //Move all the pixels to the right of the seam, one pixel to the left
    for (int row = 0; row < image.rows; ++row) {
        for (int col = seam[row]; col < image.cols-1; ++col)
            image.at<cv::Vec3b>(row, col) = image.at<cv::Vec3b>(row, col+1);
    }

    //Resize the image to remove the last column
    image = image(cv::Rect(0, 0, image.cols-1, image.rows));

    //Re-compute the energy of the new image
    computeFullEnergy();
//	computeEnergyAfterSeamRemoval(seam);
}

void SeamTrajectory::removeHorizontalSeam(std::vector<uint> seam)
{
    //Transpose the matrices and remove the vertical seam
    cv::transpose(image, image);
    cv::transpose(energy, energy);
    removeVerticalSeam(seam);

    //Transpose back
    cv::transpose(image, image);
    cv::transpose(energy, energy);
}

void SeamTrajectory::showVerticalSeam(std::vector<uint> seam)
{
    cv::Mat tmp;
    image.copyTo(tmp);
    for (int i = 0; i < tmp.rows; ++i)
        tmp.at<cv::Vec3b>(i, seam[i]) = cv::Vec3b(0, 0, 255);	//Set the color of the seam to Red
    cv::imshow("Seam", tmp);
    cv::waitKey(0);
    tmp.release();
}

void SeamTrajectory::showHorizontalSeam(std::vector<uint> seam)
{
    cv::Mat tmp;
    image.copyTo(tmp);
    for (int i = 0; i < tmp.cols; ++i)
        tmp.at<cv::Vec3b>(seam[i], i) = cv::Vec3b(0, 0, 255);	//Set the color of the seam to Red
    cv::imshow("Seam", tmp);
    cv::waitKey(0);
    tmp.release();
}

glm::mat4 SeamTrajectory::normalizeModelView(const glm::mat4 &mvMatrix)
{
    glm::mat3 R = glm::mat3(mvMatrix);
    float c = glm::length(R[0]);
    glm::mat4 normal_mvMatrix = mvMatrix;
    normal_mvMatrix /= c;
    normal_mvMatrix[3][3] = 1.f;
    return normal_mvMatrix;
}
