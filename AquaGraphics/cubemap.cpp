#include "cubemap.h"
#include <SOIL/SOIL.h>
#include <stdexcept>

Cubemap::Cubemap(const std::vector<std::string>& faceTextures) {
    if (faceTextures.size() != 6) {
        throw std::runtime_error("A cubemap requires exactly 6 textures.");
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    const GLenum cubeFaces[6] = {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    };

    for (size_t i = 0; i < 6; ++i) {
        loadCubemapFace(cubeFaces[i], faceTextures[i]);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Cubemap::~Cubemap() {
    glDeleteTextures(1, &textureID);
}

void Cubemap::loadCubemapFace(GLenum face, const std::string& imagePath) {
    int width, height;
    unsigned char* image = SOIL_load_image(imagePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    if (!image) {
        throw std::runtime_error("Failed to load cubemap texture: " + imagePath);
    }

    glTexImage2D(face, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
}

void Cubemap::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void Cubemap::unbind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
