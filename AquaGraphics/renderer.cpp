#include "renderer.h"
#include "shaderloader.h"
#include <QImage>
#include <stdexcept>

Renderer::Renderer() : sphereCenter(0.0f), sphereRadius(0.25f) {
    initializeOpenGLFunctions();

    // Initialize shaders
    waterShaders[0] = ShaderLoader::loadShader(":/ressources/aboveWater.vert", ":/ressources/aboveWater.frag");
    waterShaders[1] = ShaderLoader::loadShader(":/ressources/underWater.vert", ":/ressources/underWater.frag");
    sphereShader = ShaderLoader::loadShader(":/ressources/sphere.vert", ":/ressources/sphere.frag");
    cubeShader = ShaderLoader::loadShader(":/ressources/cube.vert", ":/ressources/cube.frag");

    // Load textures
    tileTexture = loadTexture(":/ressources/tiles.jpg");

    // Initialize light direction
    lightDir = glm::normalize(glm::vec3(2.0f, 2.0f, -1.0f));

    // Initialize buffers
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
        throw std::runtime_error("Failed to load texture: " + filePath.toStdString());
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
    // Initialize water VAO and VBO
    waterVAO.create();
    waterVAO.bind();

    waterVBO.create();
    waterVBO.bind();

    std::vector<float> waterVertices = {
        // X, Y, Z
        -1.0f, 0.0f, -1.0f,  // Bottom-left
         1.0f, 0.0f, -1.0f,  // Bottom-right
         1.0f, 0.0f,  1.0f,  // Top-right
        -1.0f, 0.0f,  1.0f   // Top-left
    };

    waterVBO.allocate(waterVertices.data(), static_cast<int>(waterVertices.size() * sizeof(float)));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    waterVAO.release();
    waterVBO.release();

    // Initialize sphere VAO and VBO
    sphereVAO.create();
    sphereVAO.bind();

    sphereVBO.create();
    sphereVBO.bind();
    std::vector<float> sphereVertices = {
        // Sphere vertices (example data; replace with actual sphere vertices)
         0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.5f,
         0.5f, -0.5f, 0.5f,
    };
    sphereVBO.allocate(sphereVertices.data(), static_cast<int>(sphereVertices.size() * sizeof(float)));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    sphereVAO.release();
    sphereVBO.release();

    // Initialize cube VAO and VBO
    cubeVAO.create();
    cubeVAO.bind();

    cubeVBO.create();
    cubeVBO.bind();
    std::vector<float> cubeVertices = {
        // Cube vertices (example data; replace with actual cube vertices)
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    cubeVBO.allocate(cubeVertices.data(), static_cast<int>(cubeVertices.size() * sizeof(float)));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    cubeVAO.release();
    cubeVBO.release();
}

void Renderer::updateCaustics(Water& water) {
    glBindFramebuffer(GL_FRAMEBUFFER, causticTexture);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    waterShaders[0]->bind();
    water.bindTexture();

    waterShaders[0]->setUniformValue("lightDir", QVector3D(lightDir.x, lightDir.y, lightDir.z));
    waterShaders[0]->setUniformValue("sphereCenter", QVector3D(sphereCenter.x, sphereCenter.y, sphereCenter.z));
    waterShaders[0]->setUniformValue("sphereRadius", sphereRadius);

    waterVAO.bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);  // Draw water plane
    waterVAO.release();
}

void Renderer::renderWater(Water& water, const Cubemap& skybox) {
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

void Renderer::renderSphere() {
    sphereShader->bind();

    sphereShader->setUniformValue("lightDir", QVector3D(lightDir.x, lightDir.y, lightDir.z));
    sphereShader->setUniformValue("sphereCenter", QVector3D(sphereCenter.x, sphereCenter.y, sphereCenter.z));
    sphereShader->setUniformValue("sphereRadius", sphereRadius);

    sphereVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Use actual vertex count
    sphereVAO.release();
}

void Renderer::renderCube() {
    cubeShader->bind();

    cubeVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);  // Use actual vertex count
    cubeVAO.release();
}
