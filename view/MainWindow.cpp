/*
 *  MainWindow.cpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "MainWindow.hpp"
#include "io/LevelParser.hpp"
#include "io/TextureFactory.hpp"

#include <iostream>
#include <QTimer>
#include <QKeyEvent>

namespace asteroids
{

MainWindow::MainWindow(const std::string& file, QWidget* parent) :
    QMainWindow(parent),
    m_timer(new QTimer())
{
    // Setup user interface
    m_startscreen = new StartScreen(this);
    m_3DScene = new GLWidget(this);

    //create and config layout
    m_screenStack = new QStackedLayout;

    m_screenStack->addWidget(m_startscreen);
    m_screenStack->addWidget(m_3DScene);

    setLayout(m_screenStack);

    // Create a timer object to trigger the main loop
    connect(m_timer.get(), &QTimer::timeout, this, &MainWindow::handleInput);
    m_timer->start(1000 / 60.0);

    setupConnections();
}

void MainWindow::handleInput()
{
    m_3DScene->step(m_keyStates);
}

void MainWindow::gotoStartScreen()
{
    m_screenStack->setCurrentWidget(m_startscreen);
}

void MainWindow::goto3DScene()
{
    m_screenStack->setCurrentWidget(m_3DScene);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // Save which key was pressed
    m_keyStates[(Qt::Key)event->key()] = true;
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    // Save which key was released
    m_keyStates[(Qt::Key)event->key()] = false;
}

void MainWindow::setupConnections()
{

}
} // namespace asteroids
