#include "water.h"
#include <stdexcept>
#include <QOpenGLFramebufferObjectFormat>
#include <glm/gtc/type_ptr.hpp>
#include <QVector2D>
#include <QVector3D>

// Constructeur : Initialise les textures et shaders
Water::Water(int resolution)
    : resolution(resolution),
      textureA(nullptr),
      textureB(nullptr) {
    initializeOpenGLFunctions();

    // Crée des textures flottantes
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::NoAttachment);
    format.setInternalTextureFormat(GL_RGBA32F);

    textureA = new QOpenGLFramebufferObject(resolution, resolution, format);
    textureB = new QOpenGLFramebufferObject(resolution, resolution, format);

    if (!textureA || !textureB) {
        throw std::runtime_error("Échec de la création des framebuffer objects");
    }

    // Initialisation des shaders
    if (!dropShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/drop.vert") ||
        !dropShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/drop.frag") ||
        !dropShader.link()) {
        throw std::runtime_error("Échec de l'initialisation du shader dropShader");
    }

    if (!updateShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/update.vert") ||
        !updateShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/update.frag") ||
        !updateShader.link()) {
        throw std::runtime_error("Échec de l'initialisation du shader updateShader");
    }

    if (!normalShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/normal.vert") ||
        !normalShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/normal.frag") ||
        !normalShader.link()) {
        throw std::runtime_error("Échec de l'initialisation du shader normalShader");
    }

    if (!sphereShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/sphere.vert") ||
        !sphereShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/sphere.frag") ||
        !sphereShader.link()) {
        throw std::runtime_error("Échec de l'initialisation du shader sphereShader");
    }
}

// Destructeur : Libère les ressources
Water::~Water() {
    delete textureA;
    delete textureB;
}

// Ajoute une goutte d'eau
void Water::addDrop(float x, float y, float radius, float strength) {
    textureB->bind();
    dropShader.bind();

    dropShader.setUniformValue("center", QVector2D(x, y));
    dropShader.setUniformValue("radius", radius);
    dropShader.setUniformValue("strength", strength);

    glBindTexture(GL_TEXTURE_2D, textureA->texture());
    dropShader.setUniformValue("texture", 0);

    glViewport(0, 0, resolution, resolution);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    textureB->release();
    std::swap(textureA, textureB);
}

// Simule le mouvement de la sphère
void Water::moveSphere(const glm::vec3 &oldCenter, const glm::vec3 &newCenter, float radius) {
    textureB->bind();
    sphereShader.bind();

    sphereShader.setUniformValue("oldCenter", QVector3D(oldCenter.x, oldCenter.y, oldCenter.z));
    sphereShader.setUniformValue("newCenter", QVector3D(newCenter.x, newCenter.y, newCenter.z));
    sphereShader.setUniformValue("radius", radius);

    glBindTexture(GL_TEXTURE_2D, textureA->texture());
    sphereShader.setUniformValue("texture", 0);

    glViewport(0, 0, resolution, resolution);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    textureB->release();
    std::swap(textureA, textureB);
}

// Simule une étape de l'eau
void Water::stepSimulation() {
    textureB->bind();
    updateShader.bind();

    updateShader.setUniformValue("delta", QVector2D(1.0f / resolution, 1.0f / resolution));

    glBindTexture(GL_TEXTURE_2D, textureA->texture());
    updateShader.setUniformValue("texture", 0);

    glViewport(0, 0, resolution, resolution);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    textureB->release();
    std::swap(textureA, textureB);
}

// Met à jour les normales de l'eau
void Water::updateNormals() {
    textureB->bind();
    normalShader.bind();

    normalShader.setUniformValue("delta", QVector2D(1.0f / resolution, 1.0f / resolution));

    glBindTexture(GL_TEXTURE_2D, textureA->texture());
    normalShader.setUniformValue("texture", 0);

    glViewport(0, 0, resolution, resolution);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    textureB->release();
    std::swap(textureA, textureB);
}

// Lie la texture active pour le rendu
void Water::bindTexture() {
    glBindTexture(GL_TEXTURE_2D, textureA->texture());
}
