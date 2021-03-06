/*
 *  MainWindow.hpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#pragma once

#include <string>

#include <QMainWindow>
#include <QStackedLayout>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "GLWidget.hpp"
#include "startscreen.h"
#include "loadingscreen.h"
#include "scene2d.h"
#include "../util/ManageGame.hpp"
#include "settingwindow.h"
#include "winningscreen.h"
#include "loosingscreen.h"
#include "io/SoundFactory.hpp"


namespace asteroids
{
/**
 * @brief   Represents the main window of the game. This
 *          class contains the main loop, handles all
 *          user input and renders all objects
 * @Maintainer lkreienbrink
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new Main Window object
     */
    MainWindow(QWidget* parent = nullptr);

public slots:
    /// Handle input
    void handleInput();

    ///Screen Navigation

    /**
     * @brief gotoStartScreen sets StartScreen as top screen
     */
    void gotoStartScreen();

    /**
     * @brief goto3DScene sets 3Dscene as top screen and starts loading process
     * TODO: add parameter for Scene init which are required to start a 3D fight
     */
    void goto3DScene();

    /**
     * @brief goToScene2D sets Scene2D as top screen
     */
    void gotoScene2D();

    /**
     * @brief gotoLoadingScreen sets LoadingScreen as top screen
     */
    void gotoLoadingScreen();

    /**
     * @brief gotoSettingsScreen sets the SettingsScreen as top screen
     */
    void gotoSettingsScreen();

    /**
     * @brief gotoWin sets the WinningScreen as the top screen
     */
    void gotoWin();
    /**
     * @brief gotoLoose sets the LoosingScreen as the top screen
     */
    void gotoLoose();

    /**
     * @brief closeWindow closes the MainWindow
     */
    void closeWindow();

protected:

    /// Called if a key was pressed
    virtual void keyPressEvent(QKeyEvent* event) override;

    /// Calles of a key was released
    virtual void keyReleaseEvent(QKeyEvent* event) override;

private:

    /**
     * @brief setupConnections setups all connections and it is called at the end of the constructor,
     * thus all member objects have to be init
     */
    void setupConnections();

    /// gl widget
    GLWidget*       m_3DScene;

    /// Layout stack used to navigate through the different Screens
    QStackedLayout* m_screenStack;

    /// StartScreen widget
    StartScreen* m_startscreen;

    /// LoadingScreen widget
    LoadingScreen* m_loadingscreen;

    /// WinningScreen widget
    winningscreen* m_winningscreen;

    /// LoosingScreen widget
    loosingscreen* m_loosingscreen;

    /// Scene2D widget
    Scene2D* m_scene2d;

    /// Settings Screen widget
    settingwindow* m_settingsScreen;

    /// map with the keys and their states
    map<Qt::Key, bool>          m_keyStates;

    /// 60 fps timer
    std::shared_ptr<QTimer>          m_timer;

};

}

#endif
