#include "simulationwidget.h"
#include <QMatrix4x4>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>

SimulationWidget::SimulationWidget(QWidget* parent)
    : QOpenGLWidget(parent),
      cubemap({"ressources/xneg.png", "ressources/xpos.png",
               "ressources/yneg.png", "ressources/ypos.png",
               "ressources/zneg.png", "ressources/zpos.png"}) {
    connect(&timer, &QTimer::timeout, this, &SimulationWidget::updateSimulation);
    timer.start(16); // ~60 FPS
}

SimulationWidget::~SimulationWidget() {
    delete renderer;
    delete water; // Libère Water
}

void SimulationWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    renderer = new Renderer(); // Initialisation après OpenGL
    water = new Water(256);    // Initialisation après OpenGL
}

void SimulationWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void SimulationWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 viewMatrix;
    viewMatrix.translate(0.0f, 0.0f, -4.0f);
    viewMatrix.rotate(-angleX, 1.0f, 0.0f, 0.0f);
    viewMatrix.rotate(-angleY, 0.0f, 1.0f, 0.0f);
    viewMatrix.translate(0.0f, 0.5f, 0.0f);

    renderer->sphereCenter = sphereCenterGlobal;
    renderer->sphereRadius = sphereRadiusGlobal;

    renderer->renderCube();
    renderer->renderWater(*water, cubemap);
    renderer->renderSphere();
}

void SimulationWidget::updateSimulation() {
    if (!paused) {
        const float deltaTime = 0.016f;

        if (useSpherePhysics) {
            float percentUnderWater = glm::clamp(
                (sphereRadiusGlobal - sphereCenterGlobal.y) / (2 * sphereRadiusGlobal),
                0.0f, 1.0f
            );

            velocity += gravity * (deltaTime - 1.1f * deltaTime * percentUnderWater);
            velocity -= glm::normalize(velocity) *
                        (percentUnderWater * deltaTime * glm::dot(velocity, velocity));
            sphereCenterGlobal += velocity * deltaTime;

            if (sphereCenterGlobal.y < sphereRadiusGlobal - 1.0f) {
                sphereCenterGlobal.y = sphereRadiusGlobal - 1.0f;
                velocity.y = std::abs(velocity.y) * 0.7f;
            }
        }

        water->moveSphere(oldSphereCenter, sphereCenterGlobal, sphereRadiusGlobal);
        oldSphereCenter = sphereCenterGlobal;
        water->stepSimulation();
        water->updateNormals();

        renderer->updateCaustics(*water);
    }

    update();
}
