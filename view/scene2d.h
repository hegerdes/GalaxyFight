#ifndef SCENE2D_H
#define SCENE2D_H

#include "ui_scene2d.h"

#include <QWidget>
#include <QGraphicsView>
#include "infobar.h"
#include "2DScene/scene2dhandler.h"

namespace asteroids  {
/**
 * @brief The Scene2D class This widget is the screen which displays the whole 2dGame part of the game
 * @author lkreienbrink
 */
class Scene2D : public QWidget
{
    Q_OBJECT

public:
    explicit Scene2D(QWidget *parent = nullptr);
    ~Scene2D();

signals:
    void planetSelected(int);

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

    Scene2dHandler *m_scenehandler;

};
}

#endif // SCENE2D_H
