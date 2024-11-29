#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class MeshLoader : protected QOpenGLFunctions {
public:
    MeshLoader();
    ~MeshLoader();

    QOpenGLVertexArrayObject* loadPlane(int resolution);
    QOpenGLVertexArrayObject* loadSphere(int stacks, int slices);

private:
    QOpenGLBuffer* createBuffer(const std::vector<float>& data, QOpenGLBuffer::Type type);

    QOpenGLVertexArrayObject* createVAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
};

#endif // MESHLOADER_H
