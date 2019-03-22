#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>

#include "ui_startscreen.h"

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
    void startServer();
    void goToSetting();

private slots:
    void on_joinBut_clicked();

    void on_createBut_clicked();

    void on_quitBut_clicked();

    void on_settingBut_clicked();

private:
    Ui::StartScreen *ui;
};
}
#endif // STARTSCREEN_H
