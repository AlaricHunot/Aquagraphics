#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <glm/glm.hpp>
#include "water.h"
#include "cubemap.h"

class Renderer : protected QOpenGLFunctions {
public:
    Renderer();
    ~Renderer();

    // Définition des propriétés pour la sphère
    glm::vec3 sphereCenter;
    float sphereRadius;

    // Méthodes de rendu
    void renderWater(Water& water, Cubemap& skybox);
    void renderCube();
    void renderSphere(); // Ajout pour dessiner une sphère

    // Mise à jour des caustiques
    void updateCaustics(Water& water);

private:
    // Initialisation des buffers et textures
    void initializeBuffers();
    GLuint loadTexture(const QString& filePath);

    // Buffers et VAO
    QOpenGLVertexArrayObject waterVAO;
    QOpenGLBuffer waterVBO;

    QOpenGLVertexArrayObject sphereVAO;
    QOpenGLBuffer sphereVBO;

    QOpenGLVertexArrayObject cubeVAO;
    QOpenGLBuffer cubeVBO;

    // Shaders
    QOpenGLShaderProgram* waterShaders[2]; // Deux shaders pour au-dessus et en dessous de l'eau
    QOpenGLShaderProgram* sphereShader;    // Shader pour la sphère
    QOpenGLShaderProgram* cubeShader;      // Shader pour le cube

    // Textures
    GLuint tileTexture;
    GLuint causticTexture;

    // Lumière
    glm::vec3 lightDir;
};

#endif // RENDERER_H
