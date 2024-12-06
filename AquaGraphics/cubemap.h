#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <QOpenGLFunctions>
#include <QString>
#include <vector>

class Cubemap : protected QOpenGLFunctions {
public:
    explicit Cubemap(const std::vector<QString>& faceTextures);
    ~Cubemap();

    void bind(unsigned int unit = 0) ;
    void unbind(unsigned int unit = 0) ;

private:
    GLuint textureID;

    void loadCubemapFace(GLenum face, const QString& imagePath);
};

#endif // CUBEMAP_H
