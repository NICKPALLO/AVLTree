#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "TreePainter.h"



int main(int argc, char **argv){

    QGuiApplication app(argc,argv);

    QQmlApplicationEngine engine;

    QQuickStyle::setStyle("Material");

    engine.loadFromModule("AVLTree", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;



    return app.exec();
}