#include "scene2dhandler.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "../../util/ManageGame.hpp"
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

    //draw map
    auto map = MapFactory::getinstance().getMap("./models/01.map");
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
        pitem->setOwner(planet->getOwner());
        pitem->setPos(QPointF(planet->getPos()[0], planet->getPos()[1]));

        addItem(pitem);
    }

    //debug ships
    auto ship = new GraphicsFighterItem(PlayerType::PLAYER2, 1);
    auto tship = new GraphicsTransporterItem(1);
    tship->setPos(100, 100);
    addItem(ship);
    addItem(tship);

    //connections

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
    update(0, 0, 1920, 1080);
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
    update(0, 0, 1920, 1080);
    m_currentlySelected = fighter;
}

void Scene2dHandler::handleTransporterSelection(GraphicsTransporterItem* transporter)
{
    transporter->selected();
    update(0, 0, 1920, 1080);
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

    update(0, 0, 1920, 1080);
    m_currentlySelected = nullptr;
}

void Scene2dHandler::placeFighter()
{
    auto fighters = ManageGame::getinstance()->get_attackSpaceCraftList();
    auto planets = MapFactory::getinstance().getMap("./models/01.map")->getPlanets();
    auto itemList = items();

    //start animation and placement
    for (const auto& fighter : fighters) {

        //skip fighter if nothing changed
        if (!fighter->m_change_position)
            continue;

        bool foundFlag = false;
        //get current GraphicsItem
        for(auto& item : itemList) {
            if(item->type() == ItemTypes::Fighter && ((GraphicsFighterItem*)item)->getID() == fighter->m_id) {
                auto pos = planets[fighter->m_next_position]->getPos();
                item->setPos(pos[0], pos[1]);
                foundFlag = true;
            }
        }

        //if no fighter found, create a new one
        if(!foundFlag) {
            auto newFighter = new GraphicsFighterItem(PlayerType::PLAYER1, fighter->m_id);
            auto pos = planets[fighter->m_position]->getPos();
            newFighter->setPos(pos[0], pos[1]);
            addItem(newFighter);
        }
    }

    update(0, 0, 1920, 1080);
}

void Scene2dHandler::placeTransporter()
{
    auto transporters = ManageGame::getinstance()->get_transportSpaceCraftList();
    auto planets = MapFactory::getinstance().getMap("./models/01.map")->getPlanets();
    auto itemList = items();

    //start animation and placement
    for (const auto& transporter : transporters) {

        bool foundFlag = false;
        //get current GraphicsItem
        for(auto& item : itemList) {
            if(item->type() == ItemTypes::Transporter && ((GraphicsFighterItem*)item)->getID() == transporter->m_id) {
                auto pos = planets[transporter->m_next_position]->getPos();
                item->setPos(pos[0], pos[1]);
                foundFlag = true;
            }
        }

        //if no transporter found, create a new one
        if(!foundFlag) {
            auto newTransporter = new GraphicsFighterItem(PlayerType::PLAYER1, transporter->m_id);
            auto pos = planets[transporter->m_position]->getPos();
            newTransporter->setPos(pos[0], pos[1]);
            addItem(newTransporter);
        }
    }

    update(0, 0, 1920, 1080);
}

void Scene2dHandler::updateRound() {
    placeFighter();
    placeTransporter();
}
}
