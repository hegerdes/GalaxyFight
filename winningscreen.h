#ifndef WINNINGSCREEN_H
#define WINNINGSCREEN_H

#include <QWidget>

namespace Ui {
class winningscreen;
}

class winningscreen : public QWidget
{
    Q_OBJECT

public:
    explicit winningscreen(QWidget *parent = nullptr);
    ~winningscreen();

private:
    Ui::winningscreen *ui;
};

#endif // WINNINGSCREEN_H
