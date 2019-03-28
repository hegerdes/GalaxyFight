#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>

#include "ui_startscreen.h"
#include "qtclient/client.h"
#include "init_file.h"

namespace asteroids {

class StartScreen : public QWidget
{
    Q_OBJECT

public:
    explicit StartScreen(QWidget *parent = nullptr);

    ~StartScreen();

signals:
    void closeWindow();
    void gotoLoadingScreen();
    void startClient();
    void goToSetting();
    void goTo2D();
    void goto3DScene();

private slots:
    void on_playBut_clicked();

    void on_quitBut_clicked();

    void on_settingBut_clicked();

private:
    Ui::StartScreen *ui;

    void setupConnections();

   // Client* m_client;

};
}
#endif // STARTSCREEN_H
