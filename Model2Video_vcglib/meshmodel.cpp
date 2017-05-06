#include "meshmodel.h"

MeshModel::MeshModel(QString modelPath, fstream &flog)
{
    // the open function returns 0 on success.
    if(vcg::tri::io::ImporterOFF<MyMesh>::Open(mesh,modelPath.toStdString().c_str())!=vcg::tri::io::ImporterOFF<MyMesh>::NoError)
    {
        std::cout << "Error: reading model file " + modelPath.toStdString() << std::endl;
        flog << "Error: reading model file " + modelPath.toStdString() << std::endl;
//        exit(0);
    }
    vcg::tri::Clean<MyMesh>::RemoveUnreferencedVertex(mesh);
    vcg::tri::Clean<MyMesh>::RemoveZeroAreaFace(mesh);
    vcg::tri::UpdateTopology<MyMesh>::FaceFace(mesh);
//    vcg::tri::Clean<MyMesh>::RemoveNonManifoldFace(mesh);
//    vcg::tri::UpdateTopology<MyMesh>::FaceFace(mesh);
    vcg::tri::RequirePerVertexNormal(mesh);
    vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalized(mesh);
    std::cout << "Input mesh vn: " << mesh.VN() << " fn: " << mesh.FN() << std::endl;
    std::cout << "Mesh has " << mesh.VN() << " verts and " << mesh.FN() << " faces" << std::endl;
// ref http://stackoverflow.com/questions/13041772/how-to-extract-vertex-coordinates-in-vcg-library
}

MeshModel::~MeshModel()
{
    clear();
}

void MeshModel::fbo_init(GLuint vertexPositionID, GLuint vertexNormalID)
{
    int index = 0;
    std::map<MyVertex*, int> dict;

    vertices.clear();
    vertexNormals.clear();
    faceIndices.clear();

    std::vector<MyVertex> &vs = mesh.vert;
    std::vector<MyEdge> &edge = mesh.edge;
    std::vector<MyFace> &fs = mesh.face;
    for(size_t i = 0; i < vs.size(); ++i)
    {
        if(vs[i].IsD())
            continue;
        vertices.push_back(vs[i].P().X());
        vertices.push_back(vs[i].P().Y());
        vertices.push_back(vs[i].P().Z());

        vertexNormals.push_back(vs[i].N().X());
        vertexNormals.push_back(vs[i].N().Y());
        vertexNormals.push_back(vs[i].N().Z());

        dict[&vs[i]] = index;
        index++;

//        std::cout << "vertex " << i << " " << &vs[i] << std::endl;
//        std::cout << "vertex x " << i << " " << vs[i].P().X() << std::endl;
//        std::cout << "vertex y " << i << " " << vs[i].P().Y() << std::endl;
//        std::cout << "vertex z " << i << " " << vs[i].P().Z() << std::endl;
//        std::cout << "vertex normal x " << i << " " << vs[i].N().X() << std::endl;
//        std::cout << "vertex normal y " << i << " " << vs[i].N().Y() << std::endl;
//        std::cout << "vertex normal z " << i << " " << vs[i].N().Z() << std::endl;
    }

    for(size_t i = 0; i < fs.size(); ++i)
    {
        if(fs[i].IsD())
            continue;
        faceIndices.push_back(dict[fs[i].V(0)]);
        faceIndices.push_back(dict[fs[i].V(1)]);
        faceIndices.push_back(dict[fs[i].V(2)]);
//        std::cout << "face " << i << " " << faceIndices[i*3] << " " << faceIndices[i * 3 + 1] << " " << faceIndices[i * 3 + 2] << std::endl;

//        std::cout << "face " << i << " " << fs[i].V(0) << " " << fs[i].V(1) << " " << fs[i].V(2) << std::endl;
//        std::cout << "face " << i << " " << fs[i].V(0)->P().X() << " "
//                  << fs[i].V(0)->P().Y() << " "
//                  << fs[i].V(0)->P().Z() << std::endl;
//        std::cout << "face " << i << " " << fs[i].V(1)->P().X() << " "
//                  << fs[i].V(1)->P().Y() << " "
//                  << fs[i].V(1)->P().Z() << std::endl;
//        std::cout << "face " << i << " " << fs[i].V(2)->P().X() << " "
//                  << fs[i].V(2)->P().Y() << " "
//                  << fs[i].V(2)->P().Z() << std::endl;
    }

    // 创建并绑定环境
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(GLfloat),
                 &vertices[0],
                 GL_STATIC_DRAW);

    glVertexAttribPointer(vertexPositionID,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          NULL);
    glEnableVertexAttribArray (vertexPositionID);

    if(vertexNormalID != -1){
        glGenBuffers(1, &m_vboVertexNormal);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboVertexNormal);
        glBufferData(GL_ARRAY_BUFFER, vertexNormals.size() * sizeof(GLfloat), &vertexNormals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(vertexNormalID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (vertexNormalID);
    }


    glGenBuffers(1, &m_vboIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 m_vboIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 faceIndices.size() * sizeof(GLuint),
                 &faceIndices[0],
                 GL_STATIC_DRAW);

    numsToDraw = faceIndices.size();
    std::cout<<"init "<<numsToDraw<<std::endl;
    m_isInited = true;

}

void MeshModel::clear()
{
    cleanup();
    mesh.Clear();
    vertices.clear();
    vertexNormals.clear();
    faceIndices.clear();
}

void MeshModel::draw()
{
    if (!m_isInited)
    {
        std::cout << "please call init() before draw()" << std::endl;
        assert(0);
    }

    // draw sphere
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
    glDrawElements(GL_TRIANGLES, numsToDraw, GL_UNSIGNED_INT, NULL);

}

void MeshModel::cleanup()
{
    if (!m_isInited)
    {
        return;
    }
    if (m_vboVertex){
        glDeleteBuffers(1, &m_vboVertex);
    }
    if (m_vboIndex)
    {
        glDeleteBuffers(1, &m_vboIndex);
    }
    if (m_vao){
        glDeleteVertexArrays(1, &m_vao);
    }

    m_isInited = false;
    m_vao = 0;
    m_vboVertex = 0;
    m_vboIndex = 0;
}

void MeshModel::getVerticesAndFaces_AddedByZwz(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
{
    /*
    int index = 0;
    std::map<typename MeshT::VertexHandle, int> dict;

    typename MeshT::VertexIter v_it, v_end(m_mesh.vertices_end());
    for (v_it = m_mesh.vertices_begin(); v_it != v_end; v_it++){
        typename MeshT::Point pos = m_mesh.point(*v_it);
        vertices.push_back(pos[0]);
        vertices.push_back(pos[1]);
        vertices.push_back(pos[2]);

        dict[*v_it] = index;
        index++;
    }

    typename MeshT::FaceIter f_it, f_end(m_mesh.faces_end());
    for(f_it = m_mesh.faces_begin(); f_it != f_end; f_it++)
    {
        typename MeshT::FaceVertexIter fv_it(m_mesh.fv_iter(*f_it));
        for (; fv_it; fv_it++)
            indices.push_back(dict[*fv_it]);
    }
    */
}

std::pair<float, glm::mat4> MeshModel::Uniformtransformation()
{
    glm::vec3 scene_min = glm::vec3(1e10, 1e10, 1e10);
    glm::vec3 scene_max = glm::vec3(-1e10, -1e10, -1e10);
    for(int i=0;i<vertices.size();i+=3)
    {
        if (vertices[i] < scene_min.x)
            scene_min.x = vertices[i];
        if (vertices[i] > scene_max.x)
            scene_max.x = vertices[i];

        if (vertices[i+1] < scene_min.y)
            scene_min.y = vertices[i+1];
        if (vertices[i+1] > scene_max.y)
            scene_max.y = vertices[i+1];

        if (vertices[i+2] < scene_min.z)
            scene_min.z = vertices[i+2];
        if (vertices[i+2] > scene_max.z)
            scene_max.z = vertices[i+2];
    }
    float tmp = -1e10;
    tmp = std::max<float>(scene_max.x - scene_min.x, tmp);
    tmp = std::max<float>(scene_max.y - scene_min.y, tmp);
    tmp = std::max<float>(scene_max.z - scene_min.z, tmp);
    float scale = 1.f / tmp;

    glm::vec3 scene_center;
    scene_center.x = (scene_min.x + scene_max.x) / 2.f;
    scene_center.y = (scene_min.y + scene_max.y) / 2.f;
    scene_center.z = (scene_min.z + scene_max.z) / 2.f;
    glm::mat4 shiftTransform = glm::translate(glm::mat4(1.f), glm::vec3(-scene_center.x, -scene_center.y, -scene_center.z));
    return std::make_pair(scale,shiftTransform);
}
