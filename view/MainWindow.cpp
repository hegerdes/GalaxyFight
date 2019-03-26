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

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_timer(new QTimer())
{
    // Setup user interface
    m_startscreen = new StartScreen(this);
    m_3DScene = new GLWidget(this);
    m_loadingscreen = new LoadingScreen(this);
    m_scene2d = new Scene2D(this);
    m_settingsScreen = new settingwindow(this);

    //create and config layout
    m_screenStack = new QStackedLayout;

    m_screenStack->addWidget(m_startscreen);
    m_screenStack->addWidget(m_loadingscreen);
    m_screenStack->addWidget(m_scene2d);
    m_screenStack->addWidget(m_3DScene);
    m_screenStack->addWidget(m_settingsScreen);


    m_3DScene->setMinimumSize(size());

    m_screenStack->setCurrentWidget(m_startscreen);

//    setLayout(m_screenStack);
    m_3DScene->setMinimumSize(QSettings().value("minWidth",1920).toInt(), QSettings().value("minHeight",1080).toInt());
   // goto3DScene();

    // Create a timer object to trigger the main loop
    connect(m_timer.get(), SIGNAL(timeout()), this, SLOT(handleInput()));
    m_timer->start(1000 / 30.0);
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

    m_3DScene->setLevelFile(QSettings().value("levelXML", "./models/level.xml").toString().toStdString());
    //m_3DScene->loadLevel();
}

void MainWindow::goToScene2D()
{
    m_screenStack->setCurrentWidget(m_scene2d);
}

void MainWindow::gotoLoadingScreen()
{
    m_screenStack->setCurrentWidget(m_loadingscreen);
}

void MainWindow::gotoSettingsScreen()
{
    m_screenStack->setCurrentWidget(m_settingsScreen);
}

void MainWindow::closeWindow()
{
    this->close();
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
  connect(ManageGame::getinstance(), &ManageGame::goto3DScene, this, &MainWindow::goto3DScene);
  connect(ManageGame::getinstance(), &ManageGame::goToScene2D,this,&MainWindow::goToScene2D);
  connect(m_startscreen, &StartScreen::gotoLoadingScreen, this, &MainWindow::gotoLoadingScreen);
  connect(m_startscreen, &StartScreen::closeWindow , this, &MainWindow::closeWindow);
  connect(m_startscreen, &StartScreen::goTo2D, this, &MainWindow::goToScene2D);
  //connect(m_startscreen, &StartScreen::gotoLoadingScreen, this, &MainWindow::goToLoadingScreen);
}
} // namespace asteroids
