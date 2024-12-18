#include "meshloader.h"
#include <cmath>
#include <stdexcept>
#include <vector>
#include <limits>

MeshLoader::MeshLoader() {
    initializeOpenGLFunctions();
}

MeshLoader::~MeshLoader() {}

QOpenGLVertexArrayObject* MeshLoader::loadPlane(int resolution) {
    if (resolution <= 0) {
        throw std::runtime_error("Resolution must be positive");
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float step = 2.0f / resolution;

    for (int y = 0; y <= resolution; ++y) {
        for (int x = 0; x <= resolution; ++x) {
            vertices.push_back(-1.0f + step * x); // X
            vertices.push_back(0.0f);            // Y
            vertices.push_back(-1.0f + step * y); // Z
        }
    }

    for (int y = 0; y < resolution; ++y) {
        for (int x = 0; x < resolution; ++x) {
            int topLeft = y * (resolution + 1) + x;
            int bottomLeft = (y + 1) * (resolution + 1) + x;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topLeft + 1);

            indices.push_back(topLeft + 1);
            indices.push_back(bottomLeft);
            indices.push_back(bottomLeft + 1);
        }
    }

    return createVAO(vertices, indices);
}

QOpenGLVertexArrayObject* MeshLoader::loadSphere(int stacks, int slices) {
    if (stacks <= 0 || slices <= 0) {
        throw std::runtime_error("Stacks and slices must be positive");
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= stacks; ++i) {
        float phi = M_PI * i / stacks;
        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * M_PI * j / slices;

            float x = sin(phi) * cos(theta);
            float y = cos(phi);
            float z = sin(phi) * sin(theta);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
        }
    }

    return createVAO(vertices, indices);
}

QOpenGLBuffer* MeshLoader::createBuffer(const void* data, int size, QOpenGLBuffer::Type type) {
    if (size <= 0 || size > std::numeric_limits<int>::max()) {
        throw std::runtime_error("Invalid buffer size");
    }

    QOpenGLBuffer* buffer = new QOpenGLBuffer(type);
    buffer->create();
    buffer->bind();
    buffer->allocate(data, size);
    return buffer;
}

QOpenGLVertexArrayObject* MeshLoader::createVAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    QOpenGLVertexArrayObject* vao = new QOpenGLVertexArrayObject();
    vao->create();
    vao->bind();

    // Vertex Buffer
    QOpenGLBuffer* vertexBuffer = createBuffer(vertices.data(), static_cast<int>(vertices.size() * sizeof(float)), QOpenGLBuffer::VertexBuffer);
    if (!vertexBuffer->isCreated()) {
        throw std::runtime_error("Failed to create vertex buffer");
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Index Buffer
    QOpenGLBuffer* indexBuffer = createBuffer(indices.data(), static_cast<int>(indices.size() * sizeof(unsigned int)), QOpenGLBuffer::IndexBuffer);
    if (!indexBuffer->isCreated()) {
        throw std::runtime_error("Failed to create index buffer");
    }

    vao->release();
    vertexBuffer->release();
    indexBuffer->release();

    return vao;
}
