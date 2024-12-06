#include <QApplication>
#include "simulationwidget.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    SimulationWidget widget;
    widget.resize(1280, 720);
    widget.show();

    return app.exec();
}
