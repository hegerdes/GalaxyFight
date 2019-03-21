#ifndef SCENE2DHANDLER_H
#define SCENE2DHANDLER_H

#include <QGraphicsScene>

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
    void handlePlanetSelection();

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
    void handleFighterSelection();

    /**
     * @brief handleTransporterSelection
     */
    void handleTransporterSelection();
};

}
#endif // SCENE2DHANDLER_H
