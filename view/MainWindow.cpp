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
#include "settings/init_file.h"

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
    m_loosingscreen = new loosingscreen(this);
    m_winningscreen = new winningscreen(this);

    //create and config layout
    m_screenStack = new QStackedLayout;

    m_screenStack->addWidget(m_startscreen);
    m_screenStack->addWidget(m_loadingscreen);
    m_screenStack->addWidget(m_scene2d);
    m_screenStack->addWidget(m_3DScene);
    m_screenStack->addWidget(m_settingsScreen);
    m_screenStack->addWidget(m_loosingscreen);
    m_screenStack->addWidget(m_winningscreen);


    m_3DScene->setMinimumSize(size());

    m_screenStack->setCurrentWidget(m_startscreen);

    m_3DScene->setMinimumSize(setting.value("Bildschirm/Breite").toInt(), setting.value("Bildschirm/Hoehe").toInt());

    // Create a timer object to trigger the main loop
    connect(m_timer.get(), SIGNAL(timeout()), this, SLOT(handleInput()));

    m_timer->start(1000 / 30.0);
    setupConnections();
    SoundFactory::instance().getSound("soundtrack")->setLoopCount(QSoundEffect::Infinite);
    SoundFactory::instance().getSound("soundtrack")->play();   
    
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
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
    m_screenStack->setCurrentWidget(m_3DScene);

    m_3DScene->setLevelFile(QSettings().value("levelXML", "./models/level.xml").toString().toStdString());
    std::cerr << __LINE__ << "\n";
    m_3DScene->active = true; // @ahaker muss noch?
    //m_3DScene->loadLevel();
}

void MainWindow::gotoScene2D()
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

void MainWindow::gotoWin()
{
    m_screenStack->setCurrentWidget(m_winningscreen);
}

void MainWindow::gotoLoose()
{
    m_screenStack->setCurrentWidget(m_loosingscreen);
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

    if(event->key() == Qt::Key_M)
    {
        SoundFactory::instance().muteAll();                
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
  connect(ManageGame::getinstance(), &ManageGame::goToScene2D,this,&MainWindow::gotoScene2D);
  connect(m_startscreen, &StartScreen::gotoLoadingScreen, this, &MainWindow::gotoLoadingScreen);
  connect(m_startscreen, &StartScreen::closeWindow , this, &MainWindow::closeWindow);
  connect(m_3DScene, &GLWidget::goToScene2D,this,&MainWindow::gotoScene2D);
  connect(m_startscreen, &StartScreen::goTo2D, this, &MainWindow::gotoScene2D);
  connect(m_startscreen, &StartScreen::goto3DScene, this, &MainWindow::goto3DScene);
  connect(m_startscreen, &StartScreen::goToSetting, this, &MainWindow::gotoSettingsScreen);
  connect(m_3DScene, &GLWidget::planet_apply_updates, ManageGame::getinstance(), &ManageGame::planet_apply_updates);
  connect(m_settingsScreen, &settingwindow::goToStart, this, &MainWindow::closeWindow);
  connect(ManageGame::getinstance(), &ManageGame::goToWin, this, &MainWindow::gotoWin);
  connect(ManageGame::getinstance(), &ManageGame::gotoLoose, this, &MainWindow::gotoLoose);

  //connect(m_startscreen, &StartScreen::gotoLoadingScreen, this, &MainWindow::goToLoadingScreen);
}
} // namespace asteroids
