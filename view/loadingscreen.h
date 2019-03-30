#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <QWidget>
#include <QSettings>
#include "settings/init_file.h"

namespace Ui {
class LoadingScreen;
}

namespace asteroids {

class LoadingScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingScreen(QWidget *parent = nullptr);
    ~LoadingScreen();

signals:
    void goTo2D();

private:
    Ui::LoadingScreen *ui;
};

}

#endif // LOADINGSCREEN_H

