#ifndef SCENE2DHANDLER_H
#define SCENE2DHANDLER_H

#include <QGraphicsScene>
#include "graphicsfighteritem.h"
#include "graphicsplanetitem.h"
#include "graphicsmineitem.h"
#include "graphicsfactoryitem.h"
#include "graphicstransporteritem.h"

namespace asteroids {

/**
 * @brief The Scene2dHandler class it handels the drawing of the 2DSpace and its items like the
 * planets, pathes, ships, etc.. For this it loads the data from the gamelogic (primarily the map data)
 * @author lkreienbrink
 */
class Scene2dHandler : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene2dHandler(QObject* parent = nullptr);

signals:
    void planetSelected(int);

public slots:
    void updateRound();

protected:
    /**
     * @brief mouseReleaseEvent
     * @param mouseEvent
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    /**
     * @brief handlePlanetSelection notifys the GraphicsPlanetItem to draw a blue selection circle,
     * send a selection signal and store the pointer of it in the m_currently_selected pointer
     * @param planet is the selected GraphicsPlanetItem
     */
    void handlePlanetSelection(GraphicsPlanetItem* planet);

    /**
     * @brief handleMineSelection does nothing at the moment
     */
    void handleMineSelection();

    /**
     * @brief handleFactorySelection does nothing at the moment
     */
    void handleFactorySelection();

    /**
     * @brief handleFighterSelection notifies the GraphicsFighterItem to draw a blue slection circle
     * and store the pointer of it in the m_currently_selected pointer
     */
    void handleFighterSelection(GraphicsFighterItem* fighter);

    /**
     * @brief handleTransporterSelection notifies the GraphicsTransporterItem to draw a blue slection circle
     * and store the pointer of it in the m_currently_selected pointer
     */
    void handleTransporterSelection(GraphicsTransporterItem* transporter);

    /**
     * @brief unselectAll removes all selection
     */
    void unselectAll();

    /**
     * @brief placeFighter every time a new fighter spawns it is placed right beside the planet
     * where it has been created. If a fighter is moved from planet to another one,
     * an animation is started which moves it along the edge to its new pos.
     */
    void placeFighter();

    /**
     * @brief placeTransporter every time a new transporter spawns it is placed right beside the planet
     * where it has been created. If a transporter is moved from one planet to another one, an
     * animation is started which moves it along the edge to its new pos.
     */
    void placeTransporter();

    /**
     * @brief updateMap updates the owner of each planet if required and adds the mine and factory icons
     */
    void updateMap();

    /**
     * @brief getAngle returns an angle between two points
     * @param target point
     * @param source point
     * @return angle in degree
     */
    double getAngle(QPointF target, QPointF source);

    /**
     * @brief m_currentlySelected holds the pointer to the last selected GraphicsItem. Currently
     * this pointer should only point to GraphicsFighterItem, GraphicsTransporterItem and GraphicsPlanetItem
     */
    QGraphicsItem* m_currentlySelected;
};

}
#endif // SCENE2DHANDLER_H
