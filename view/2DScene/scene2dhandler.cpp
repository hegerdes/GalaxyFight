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

    QGraphicsRectItem rec(10, 10, 100, 100);
    rec.setBrush(QBrush(Qt::white));

    rec.setSelected(true);

    rec.show();

    addItem(&rec);

//    addRect(200, 200, 100, 100, QPen(), QBrush(Qt::white));



}

void Scene2dHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    //get item on click with left mouse button
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    //get item
    QGraphicsItem* item;
    item = itemAt(mouseEvent->pos(), QTransform());

    //Performs a operation specific to each type
    switch (item->type()) {
    case ItemTypes::Planet:

        break;
    case ItemTypes::Mine:

        break;
    case ItemTypes::Factory:

        break;
    case ItemTypes::Fighter:

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

void Scene2dHandler::handleFighterSelection()
{

}

void Scene2dHandler::handleTransporterSelection()
{

}
}
