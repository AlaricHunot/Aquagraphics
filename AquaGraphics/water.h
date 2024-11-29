#ifndef WATER_H
#define WATER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>
#include <glm/glm.hpp>

class Water : protected QOpenGLFunctions {
public:
    Water(int resolution);
    ~Water();

    // Public methods for water simulation
    void addDrop(float x, float y, float radius, float strength);
    void moveSphere(const glm::vec3 &oldCenter, const glm::vec3 &newCenter, float radius);
    void stepSimulation();
    void updateNormals();

    // Binds the active texture for rendering
    void bindTexture();

private:
    int resolution; // Resolution of the water simulation grid
    QOpenGLFramebufferObject *textureA; // Active framebuffer texture
    QOpenGLFramebufferObject *textureB; // Secondary framebuffer texture

    // Shader programs for different operations
    QOpenGLShaderProgram dropShader;    // Shader for simulating water drops
    QOpenGLShaderProgram updateShader;  // Shader for updating water simulation
    QOpenGLShaderProgram normalShader;  // Shader for calculating water normals
    QOpenGLShaderProgram sphereShader;  // Shader for sphere interaction
};

#endif // WATER_H
