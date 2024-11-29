#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <glm/glm.hpp>
#include "water.h"
#include "cubemap.h"

class Renderer : protected QOpenGLFunctions {
public:
    Renderer();
    ~Renderer();

    void renderWater(Water& water, const Cubemap& skybox);
    void renderSphere();
    void renderCube();
    void updateCaustics(Water& water);

    glm::vec3 sphereCenter;
    float sphereRadius;

private:
    QOpenGLShaderProgram* waterShaders[2];
    QOpenGLShaderProgram* sphereShader;
    QOpenGLShaderProgram* cubeShader;

    GLuint causticTexture;
    GLuint tileTexture;

    QOpenGLVertexArrayObject waterVAO;
    QOpenGLBuffer waterVBO;

    QOpenGLVertexArrayObject sphereVAO;
    QOpenGLBuffer sphereVBO;

    QOpenGLVertexArrayObject cubeVAO;
    QOpenGLBuffer cubeVBO;

    glm::vec3 lightDir;

    GLuint loadTexture(const QString& filePath);
    void initializeBuffers();
};

#endif // RENDERER_H
