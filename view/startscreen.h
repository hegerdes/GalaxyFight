#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>

#include "ui_startscreen.h"
#include "qtclient/client.h"

namespace asteroids {

class StartScreen : public QWidget
{
    Q_OBJECT

public:
    explicit StartScreen(QWidget *parent = nullptr);
    ~StartScreen();

signals:
    void closeProgramm();
    void goToLoading();
    void startClient();
    void goToSetting();

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
