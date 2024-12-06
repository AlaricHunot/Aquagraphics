#ifndef SIMULATIONWIDGET_H
#define SIMULATIONWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <glm/glm.hpp>
#include "renderer.h"
#include "water.h"
#include "cubemap.h"

class SimulationWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit SimulationWidget(QWidget* parent = nullptr);
    ~SimulationWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private slots:
    void updateSimulation();

private:
    Renderer* renderer;
    Water* water; // Changement pour utiliser un pointeur
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

#endif // SIMULATIONWIDGET_H
