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
     * @brief handlePlanetSelection
     */
    void handlePlanetSelection(GraphicsPlanetItem* planet);

    /**
     * @brief handleMineSelection
     */
    void handleMineSelection();

    /**
     * @brief handleFactorySelection
     */
    void handleFactorySelection();

    /**
     * @brief handleFighterSelection
     */
    void handleFighterSelection(GraphicsFighterItem* fighter);

    /**
     * @brief handleTransporterSelection
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

    QGraphicsItem* m_currentlySelected;
};

}
#endif // SCENE2DHANDLER_H
