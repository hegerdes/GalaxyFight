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


int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    asteroids::MainWindow mainWindow;
    mainWindow.showFullScreen();

    return a.exec();
}
