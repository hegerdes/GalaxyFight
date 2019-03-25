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

#include "view/MainWindow.hpp"
#include "global_socket.h"

Client client_global;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cerr << "Usage: asteroids <level-file>" << endl;
        return 1;
    }

    QApplication a(argc, argv);
    client_global.connect("lennartkaiser.de", 38292);

    asteroids::MainWindow mainWindow(argv[1]);
    mainWindow.show();

    return a.exec();
}
