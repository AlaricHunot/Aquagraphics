#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include "renderer.h"
#include "water.h"
#include "cubemap.h"
#include <glm/glm.hpp>

class SimulationWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    SimulationWidget(QWidget* parent = nullptr)
        : QOpenGLWidget(parent), water(256), cubemap({
            "ressources/xneg.png", "ressources/xpos.png",
            "ressources/yneg.png", "ressources/ypos.png",
            "ressources/zneg.png", "ressources/zpos.png"}) {
        connect(&timer, &QTimer::timeout, this, &SimulationWidget::updateSimulation);
        timer.start(16); // ~60 FPS
    }

protected:
    void initializeGL() override {
        initializeOpenGLFunctions();
        glEnable(GL_DEPTH_TEST);
        renderer = new Renderer();
    }

    void resizeGL(int w, int h) override {
        glViewport(0, 0, w, h);
    }

    void paintGL() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Transformations de la caméra
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -4.0f);
        glRotatef(-angleX, 1.0f, 0.0f, 0.0f);
        glRotatef(-angleY, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.5f, 0.0f);

        // Rendu
        renderer->sphereCenter = sphereCenterGlobal;
        renderer->sphereRadius = sphereRadiusGlobal;
        renderer->renderCube();
        renderer->renderWater(water, cubemap);
        renderer->renderSphere();
    }

private slots:
    void updateSimulation() {
        if (!paused) {
            float deltaTime = 0.016f; // Fixed timestep ~60 FPS
            if (useSpherePhysics) {
                float percentUnderWater = glm::clamp((sphereRadiusGlobal - sphereCenterGlobal.y) / (2 * sphereRadiusGlobal), 0.0f, 1.0f);
                velocity += gravity * (deltaTime - 1.1f * deltaTime * percentUnderWater);
                velocity -= glm::normalize(velocity) * (percentUnderWater * deltaTime * glm::dot(velocity, velocity));
                sphereCenterGlobal += velocity * deltaTime;

                if (sphereCenterGlobal.y < sphereRadiusGlobal - 1.0f) {
                    sphereCenterGlobal.y = sphereRadiusGlobal - 1.0f;
                    velocity.y = fabs(velocity.y) * 0.7f;
                }
            }

            water.moveSphere(oldSphereCenter, sphereCenterGlobal, sphereRadiusGlobal);
            oldSphereCenter = sphereCenterGlobal;
            water.stepSimulation();
            water.updateNormals();
            renderer->updateCaustics(water);
        }
        update();
    }

private:
    Renderer* renderer;
    Water water;
    Cubemap cubemap;
    QTimer timer;

    glm::vec3 sphereCenterGlobal{-0.4f, -0.75f, 0.2f};
    glm::vec3 oldSphereCenter = sphereCenterGlobal;
    glm::vec3 velocity{0.0f};
    glm::vec3 gravity{0.0f, -4.0f, 0.0f};
    float sphereRadiusGlobal{0.25f};
    bool paused{false};
    bool useSpherePhysics{false};
    float angleX{-25.0f};
    float angleY{-200.5f};
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    SimulationWidget widget;
    widget.resize(1280, 720);
    widget.show();

    return app.exec();
}
