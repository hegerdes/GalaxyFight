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

private:
    Ui::StartScreen *ui;
};
}
#endif // STARTSCREEN_H
