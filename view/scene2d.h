#ifndef SCENE2D_H
#define SCENE2D_H

#include <QWidget>

namespace Ui {
class Scene2D;
}

class Scene2D : public QWidget
{
    Q_OBJECT

public:
    explicit Scene2D(QWidget *parent = nullptr);
    ~Scene2D();

private:
    Ui::Scene2D *ui;
};

#endif // SCENE2D_H
