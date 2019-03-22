#ifndef SCENE2DHANDLER_H
#define SCENE2DHANDLER_H

#include <QGraphicsScene>
#include "graphicsfighteritem.h"
#include "graphicsplanetitem.h"
#include "graphicsmineitem.h"
#include "graphicsfactoryitem.h"

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
    void handleTransporterSelection();

    /**
     * @brief unselectAll removes all selection
     */
    void unselectAll();

    QGraphicsItem* m_currentlySelected;
};

}
#endif // SCENE2DHANDLER_H
