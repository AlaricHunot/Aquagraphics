#include "shaderloader.h"
#include <QFile>
#include <QTextStream>
#include <stdexcept>

QOpenGLShaderProgram* ShaderLoader::loadShader(const QString& vertexPath, const QString& fragmentPath) {
    QOpenGLShaderProgram* program = new QOpenGLShaderProgram();

    // Charger le vertex shader
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexPath)) {
        throw std::runtime_error("Erreur lors du chargement du vertex shader : " + vertexPath.toStdString());
    }

    // Charger le fragment shader
    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentPath)) {
        throw std::runtime_error("Erreur lors du chargement du fragment shader : " + fragmentPath.toStdString());
    }

    // Lier le programme
    if (!program->link()) {
        throw std::runtime_error("Erreur lors du lien du shader program");
    }

    return program;
}
