/*
 *  Main.cpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include <QSettings>
#include "view/MainWindow.hpp"
#include "rendering/2D/MapFactory.hpp"


int main(int argc, char** argv)
{
//    //Testing Map
//    asteroids::MapFactory& b = asteroids::MapFactory::getinstance();
//    asteroids::Map::Ptr map = b.getMap("models/01.map");

//    //map->print();
//    map->printEdges();
//    auto planets = map->getPlanets();
//    map->getPath(Vector3f(0,0,0),1,5);

    QApplication a(argc, argv);

    asteroids::MainWindow mainWindow;
    mainWindow.showFullScreen();

    return a.exec();
}
