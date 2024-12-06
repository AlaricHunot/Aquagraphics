#include "cubemap.h"
#include <QImage>
#include <stdexcept>
#include <iostream> // Pour déboguer

Cubemap::Cubemap(const std::vector<QString>& faceTextures) {
    // Vérification que nous avons exactement 6 textures
    if (faceTextures.size() != 6) {
        throw std::runtime_error("A cubemap requires exactly 6 textures.");
    }

    // Initialisation des fonctions OpenGL
    initializeOpenGLFunctions();

    // Génération de la texture pour le cubemap
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // Tableau contenant les faces de cubemap dans l'ordre attendu par OpenGL
    const GLenum cubeFaces[6] = {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    };

    // Chargement des 6 textures dans les faces du cubemap
    try {
        for (size_t i = 0; i < 6; ++i) {
            loadCubemapFace(cubeFaces[i], faceTextures[i]);
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Error loading cubemap face: " << e.what() << std::endl;
        throw; // Relancer l'exception après avoir affiché l'erreur
    }

    // Configuration des paramètres de texture
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Désactivation de la texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Cubemap::~Cubemap() {
    // Libération de la texture OpenGL
    glDeleteTextures(1, &textureID);
}

void Cubemap::loadCubemapFace(GLenum face, const QString& imagePath) {
    // Chargement de l'image
    QImage image(imagePath);
    if (image.isNull()) {
        throw std::runtime_error("Failed to load cubemap texture: " + imagePath.toStdString());
    }

    // Conversion de l'image au format OpenGL compatible
    QImage formattedImage = image.convertToFormat(QImage::Format_RGB888).mirrored();

    // Chargement de la texture dans OpenGL
    glTexImage2D(face, 0, GL_RGB, formattedImage.width(), formattedImage.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, formattedImage.bits());
}

void Cubemap::bind(unsigned int unit) {
    if (unit >= 32) {
        std::cerr << "Texture unit out of range: " << unit << std::endl;
        return; // Protéger contre un dépassement d'indice
    }
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void Cubemap::unbind(unsigned int unit) {
    if (unit >= 32) {
        std::cerr << "Texture unit out of range: " << unit << std::endl;
        return; // Protéger contre un dépassement d'indice
    }
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
