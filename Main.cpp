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
#include "global_socket.h"
#include "settings/init_file.h"

asteroids::Client client_global;

int main(int argc, char** argv)
{

    QApplication a(argc, argv);
    client_global.connect("127.0.0.1", 38292);
    //client_global.connect(setting.value("Netzwerk/IP").toString().toStdString(), setting.value("Netzwerk/Port").toInt());

    asteroids::MainWindow mainWindow;
    mainWindow.showFullScreen();

    return a.exec();
}
