#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <qopengl.h>
#include <vector>
#include <string>

class Cubemap {
public:
    Cubemap(const std::vector<std::string>& faceTextures);
    ~Cubemap();
    void bind(unsigned int unit = 0) const;
    void unbind(unsigned int unit = 0) const;

private:
    GLuint textureID;
    void loadCubemapFace(GLenum face, const std::string& imagePath);
};

#endif // CUBEMAP_H
