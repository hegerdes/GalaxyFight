#include "scene2dhandler.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "itemtypes.h"

namespace asteroids {

Scene2dHandler::Scene2dHandler(QObject* parent)
    : QGraphicsScene (parent)
{
    //config appearance
    QPixmap background("./models/box3.jpg");
    setBackgroundBrush(QBrush(background));
}

void Scene2dHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    //get item on click with left mouse button
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    //get item
    QGraphicsItem* item;
    item = itemAt(mouseEvent->pos(), QTransform());

    if(item == nullptr)
        return;

    //Performs a operation specific to each type
    switch (item->type()) {
    case ItemTypes::Planet:

        break;
    case ItemTypes::Mine:

        break;
    case ItemTypes::Factory:

        break;
    case ItemTypes::Fighter:
            handleFighterSelection((GraphicsFighterItem*) item);
        break;
    case ItemTypes::Transporter:

        break;
    default:
        //ignore all other items in the scene
        return;
    }

}

void Scene2dHandler::handlePlanetSelection()
{

}

void Scene2dHandler::handleMineSelection()
{

}

void Scene2dHandler::handleFactorySelection()
{

}

void Scene2dHandler::handleFighterSelection(GraphicsFighterItem* fighter)
{
    fighter->selected();
    update(fighter->pos().x(), fighter->pos().x(), 50, 50);
}

void Scene2dHandler::handleTransporterSelection()
{

}
}
