#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <QOpenGLShaderProgram>
#include <QString>

class ShaderLoader {
public:
    static QOpenGLShaderProgram* loadShader(const QString& vertexPath, const QString& fragmentPath);
};

#endif // SHADERLOADER_H
