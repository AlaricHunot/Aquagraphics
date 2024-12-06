QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    cubemap.cpp \
    main.cpp \
    mainwindow.cpp \
    meshloader.cpp \
    renderer.cpp \
    shaderloader.cpp \
    simulationwidget.cpp \
    water.cpp

HEADERS += \
    cubemap.h \
    mainwindow.h \
    meshloader.h \
    renderer.h \
    shaderloader.h \
    simulationwidget.h \
    water.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    ressources.qrc

# Compilation avec gestion des exceptions
QMAKE_CXXFLAGS += -std=c++11 -fexceptions
QMAKE_LFLAGS += -std=c++11 -fexceptions

# Forcer l'inclusion de certaines bibliothèques
LIBS += -lstdc++fs -lGL -lGLU -lpthread -ldl

# Drapeaux pour le débogage et les warnings
QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic

# Activation de l'optimisation et de la gestion des threads
CONFIG += thread release

# Nettoyage automatique des fichiers temporaires
clean.target = clean-all
clean.commands = rm -rf *.o *~ build-*
QMAKE_EXTRA_TARGETS += clean
