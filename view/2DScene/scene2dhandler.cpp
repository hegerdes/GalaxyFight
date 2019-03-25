#include "scene2dhandler.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "../../rendering/2D/MapFactory.hpp"
#include "itemtypes.h"
#include <QDebug>


namespace asteroids {

Scene2dHandler::Scene2dHandler(QObject* parent)
    : QGraphicsScene (parent)
{
    //config appearance
    QPixmap background("./models/box3.jpg");
    setBackgroundBrush(QBrush(background));

    MapFactory& fac = MapFactory::getinstance();

    //draw map
    auto map = fac.getMap("./models/01.map");
    auto planets = map->getPlanets();
    auto edges = map->getEdges();

    //draw connections
    QPen linePen(Qt::white, 3);
    for(const auto& edge : edges) {
        auto p1 = planets[edge[0]];
        auto p2 = planets[edge[1]];

        addLine(p1->getPos()[0] + 25, p1->getPos()[1] + 25,
                p2->getPos()[0] + 25, p2->getPos()[1] + 25, linePen);
    }

    //draw Planets
    for (const auto& planet : planets) {
        GraphicsPlanetItem* pitem = new GraphicsPlanetItem(planet->getID());
        pitem->setPos(QPointF(planet->getPos()[0], planet->getPos()[1]));

        addItem(pitem);
    }

    auto ship = new GraphicsFighterItem(PlayerType::PLAYER2, 1);
    auto tship = new GraphicsTransporterItem(1);
    tship->setPos(100, 100);
    addItem(ship);
    addItem(tship);

}

void Scene2dHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    //get item on click with left mouse button
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    //get item
    QGraphicsItem* item;
    item = itemAt(mouseEvent->scenePos(), QTransform());
    std::cout << mouseEvent->scenePos().x() << " : " << mouseEvent->scenePos().y() << std::endl;
    if(item == nullptr)
        return;

    unselectAll();

    //Performs a operation specific to each type
    switch (item->type()) {
    case ItemTypes::Planet:
        handlePlanetSelection((GraphicsPlanetItem*) item);
        break;
    case ItemTypes::Mine:

        break;
    case ItemTypes::Factory:

        break;
    case ItemTypes::Fighter:
            handleFighterSelection((GraphicsFighterItem*) item);
        break;
    case ItemTypes::Transporter:
            handleTransporterSelection((GraphicsTransporterItem*) item);
        break;
    default:
        //ignore all other items in the scene
        return;
    }

}

void Scene2dHandler::handlePlanetSelection(GraphicsPlanetItem* planet)
{
    planet->selected();
//    update(planet->pos().x(), planet->pos().x(), 75, 75);
    update(0, 0, width(), height());
    m_currentlySelected = planet;
    emit planetSelected(planet->getID());
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
    update(fighter->pos().x()-10, fighter->pos().x()-10, 60, 60);
    m_currentlySelected = fighter;
}

void Scene2dHandler::handleTransporterSelection(GraphicsTransporterItem* transporter)
{
    transporter->selected();
    update(transporter->pos().x()-10, transporter->pos().x()-10, 60, 60);
    m_currentlySelected = transporter;
}

void Scene2dHandler::unselectAll()
{
    if(m_currentlySelected == nullptr)
        return;

    switch (m_currentlySelected->type()) {
    case ItemTypes::Planet:
        ((GraphicsPlanetItem*) m_currentlySelected)->selected(false);
        break;
    case ItemTypes::Mine:

        break;
    case ItemTypes::Factory:

        break;
    case ItemTypes::Fighter:
            ((GraphicsFighterItem*) m_currentlySelected)->selected(false);
        break;
    case ItemTypes::Transporter:
            ((GraphicsTransporterItem*) m_currentlySelected)->selected(false);
        break;
    default:
        //ignore all other items in the scene
        break;
    }

    update(m_currentlySelected->pos().x()-10, m_currentlySelected->pos().x()-10, 75, 75);
    m_currentlySelected = nullptr;
}

void Scene2dHandler::placeFighter()
{
    auto map = MapFactory::getinstance().getMap("./models/01.map");
    auto planets = map->getPlanets();

    for (const auto& planet : planets) {

    }
}

void Scene2dHandler::placeTransporter()
{

}


void Scene2dHandler::updateRound() {

}
}
