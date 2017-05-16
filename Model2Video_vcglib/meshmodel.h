#ifndef MESHMODEL_H
#define MESHMODEL_H

#include <vcg/complex/complex.h>
#include <wrap/io_trimesh/import.h>
#include <vcg/complex/algorithms/update/curvature.h>
#include <QString>

#include <assert.h>
#include <map>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <QtDebug>
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include <iostream>


class MyVertex;
class MyEdge;
class MyFace;

struct MyUsedTypes: public vcg::UsedTypes<vcg::Use<MyVertex>::AsVertexType,
                                          vcg::Use<MyEdge>::AsEdgeType,
                                          vcg::Use<MyFace>::AsFaceType>{};

class MyVertex  : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags, vcg::vertex::Curvaturef>{};
class MyFace    : public vcg::Face<   MyUsedTypes, vcg::face::FFAdj,  vcg::face::VertexRef, vcg::face::BitFlags, vcg::face::Mark > {};
class MyEdge    : public vcg::Edge<   MyUsedTypes> {};

class MyMesh    : public vcg::tri::TriMesh< std::vector<MyVertex>, std::vector<MyFace> , std::vector<MyEdge>  > {};

class MyVertex0  : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::BitFlags  >{};
class MyVertex1  : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags  >{};
class MyVertex2  : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Color4b, vcg::vertex::CurvatureDirf,
                                                    vcg::vertex::Qualityf, vcg::vertex::Normal3f, vcg::vertex::BitFlags  >{};



class MeshModel
{
public:
    MeshModel(QString modelPath, std::fstream &flog);
    ~MeshModel();

    void fbo_init(GLuint vertexPositionId, GLuint vertexNormalID);

    void clear();

    void draw();

    // this function is used for clear the opengl buffers
    void cleanup();

    void getVerticesAndFaces_hejw005(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);

    std::pair<float, glm::mat4> Uniformtransformation();

    std::pair<double, double> getMeanGaussianCurvature(std::vector<bool> &isVertexVisiable);

    MyMesh mesh;

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> vertexNormals;
    std::vector<GLuint> faceIndices;

private:
    GLuint m_vao = 0, m_vboVertex = 0, m_vboIndex = 0, m_vboVertexNormal = 0;
    int numsToDraw;
    bool m_isInited;

    double clamp(double val, double min = 0.0, double max = 1.0);
    double abs(double val);
};

#endif // MESHMODEL_H
