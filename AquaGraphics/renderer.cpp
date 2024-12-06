#include "renderer.h"
#include "shaderloader.h"
#include <QImage>
#include <stdexcept>
#include <memory> // Pour gérer les allocations modernes

Renderer::Renderer() : sphereCenter(0.0f), sphereRadius(0.25f) {
    initializeOpenGLFunctions();

    // Initialisation des shaders
    waterShaders[0] = ShaderLoader::loadShader(":/ressources/aboveWater.vert", ":/ressources/aboveWater.frag");
    waterShaders[1] = ShaderLoader::loadShader(":/ressources/underWater.vert", ":/ressources/underWater.frag");
    sphereShader = ShaderLoader::loadShader(":/ressources/sphere.vert", ":/ressources/sphere.frag");
    cubeShader = ShaderLoader::loadShader(":/ressources/cube.vert", ":/ressources/cube.frag");

    // Chargement des textures
    tileTexture = loadTexture(":/ressources/tiles.jpg");

    // Initialisation de la direction de la lumière
    lightDir = glm::normalize(glm::vec3(2.0f, 2.0f, -1.0f));

    // Initialisation des buffers
    initializeBuffers();
}

Renderer::~Renderer() {
    glDeleteTextures(1, &causticTexture);
    glDeleteTextures(1, &tileTexture);

    delete waterShaders[0];
    delete waterShaders[1];
    delete sphereShader;
    delete cubeShader;

    waterVAO.destroy();
    waterVBO.destroy();

    sphereVAO.destroy();
    sphereVBO.destroy();

    cubeVAO.destroy();
    cubeVBO.destroy();
}

GLuint Renderer::loadTexture(const QString& filePath) {
    QImage image(filePath);
    if (image.isNull()) {
        throw std::runtime_error("Échec du chargement de la texture : " + filePath.toStdString());
    }

    QImage formattedImage = image.convertToFormat(QImage::Format_RGB888);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, formattedImage.width(), formattedImage.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, formattedImage.bits());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

void Renderer::initializeBuffers() {
    // Initialisation du VAO et VBO pour l'eau
    waterVAO.create();
    waterVAO.bind();

    waterVBO.create();
    waterVBO.bind();

    std::vector<float> waterVertices = {
        -1.0f, 0.0f, -1.0f,
         1.0f, 0.0f, -1.0f,
         1.0f, 0.0f,  1.0f,
        -1.0f, 0.0f,  1.0f
    };

    waterVBO.allocate(waterVertices.data(), static_cast<int>(waterVertices.size() * sizeof(float)));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    waterVAO.release();
    waterVBO.release();
}

void Renderer::renderWater(Water& water, Cubemap& skybox) {
    glEnable(GL_CULL_FACE);

    for (int i = 0; i < 2; ++i) {
        glCullFace(i == 0 ? GL_BACK : GL_FRONT);

        waterShaders[i]->bind();
        water.bindTexture();

        waterShaders[i]->setUniformValue("lightDir", QVector3D(lightDir.x, lightDir.y, lightDir.z));
        waterShaders[i]->setUniformValue("sphereCenter", QVector3D(sphereCenter.x, sphereCenter.y, sphereCenter.z));
        waterShaders[i]->setUniformValue("sphereRadius", sphereRadius);

        skybox.bind();
        waterVAO.bind();
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        waterVAO.release();
    }

    glDisable(GL_CULL_FACE);
}

void Renderer::renderCube() {
    cubeShader->bind();

    cubeVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    cubeVAO.release();
}

// Ajout de la méthode renderSphere()
void Renderer::renderSphere() {
    sphereShader->bind();

    sphereShader->setUniformValue("lightDir", QVector3D(lightDir.x, lightDir.y, lightDir.z));
    sphereShader->setUniformValue("sphereCenter", QVector3D(sphereCenter.x, sphereCenter.y, sphereCenter.z));
    sphereShader->setUniformValue("sphereRadius", sphereRadius);

    sphereVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3); // Par défaut, 3 points pour représenter un triangle
    sphereVAO.release();
}

// Ajout de la méthode updateCaustics()
void Renderer::updateCaustics(Water& water) {
    glBindFramebuffer(GL_FRAMEBUFFER, causticTexture);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    waterShaders[0]->bind();
    water.bindTexture();

    waterShaders[0]->setUniformValue("lightDir", QVector3D(lightDir.x, lightDir.y, lightDir.z));
    waterShaders[0]->setUniformValue("sphereCenter", QVector3D(sphereCenter.x, sphereCenter.y, sphereCenter.z));
    waterShaders[0]->setUniformValue("sphereRadius", sphereRadius);

    waterVAO.bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    waterVAO.release();
}
