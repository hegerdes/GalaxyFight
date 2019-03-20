#ifndef SCENE2D_H
#define SCENE2D_H

#include <QWidget>
#include <QGraphicsView>
#include "infobar.h"

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

    /**
     * @brief m_graphicsview
     */
    QGraphicsView *m_graphicsview;

    /**
     * @brief m_infobar
     */
    Infobar *m_infobar;

};

#endif // SCENE2D_H
