#include "scene2dhandler.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <memory>
#include <QDebug>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QtMath>
#include "../../util/ManageGame.hpp"
#include "../../rendering/2D/MapFactory.hpp"
#include "itemtypes.h"
#include "init_file.h"


namespace asteroids {

Scene2dHandler::Scene2dHandler(QObject* parent)
    : QGraphicsScene (parent)
{
    //config appearance
    QPixmap background(setting.value("Dateipfade/Hintergrund2D").toString());
    setBackgroundBrush(QBrush(background));

   //draw map
    auto map = MapFactory::getinstance().getMap(setting.value("Dateipfade/Map").toString().toStdString());
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

    //init pointer with nullptr to prevent crashes
    m_currentlySelected = nullptr;

    //draw Planets
    for (const auto& planet : planets) {
        GraphicsPlanetItem* pitem = new GraphicsPlanetItem(planet->getID());
        pitem->setOwner(planet->getOwner());
        pitem->setPos(QPointF(planet->getPos()[0], planet->getPos()[1]));

        //draw a HQ icon on the first and the last planet
        if(planet->getID() == 0 || planet->getID() == planets.size()-1)
            pitem->setIsHQ(true);

        addItem(pitem);
    }

    updateRound();

    //connections
    connect(ManageGame::getinstance(), &ManageGame::updateScene, this, &Scene2dHandler::updateRound);
}

void Scene2dHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    //get item on click with left mouse button
    if (mouseEvent->button() == Qt::LeftButton) {
        //get item
        QGraphicsItem* item;
        item = itemAt(mouseEvent->scenePos(), QTransform());
        if(item == nullptr)
            return;

        unselectAll();

        //Performs a operation specific to each type
        switch (item->type()) {
        case ItemTypes::Planet:
            handlePlanetSelection(static_cast<GraphicsPlanetItem*>(item));
            break;
        case ItemTypes::Mine:
            handleMineSelection();
            break;
        case ItemTypes::Factory:
            handleFactorySelection();
            break;
        case ItemTypes::Fighter:
                handleFighterSelection(static_cast<GraphicsFighterItem*>(item));
            break;
        case ItemTypes::Transporter:
                handleTransporterSelection(static_cast<GraphicsTransporterItem*>(item));
            break;
        default:
            //ignore all other items in the scene
            return;
        }
    } else if (mouseEvent->button() == Qt::RightButton) { //handle fighter movement and transporter path change
        //get item
        QGraphicsItem* item;
        item = itemAt(mouseEvent->scenePos(), QTransform());
        if(item == nullptr)
            return;

        //fighter/transporter can only move to planets
        if(item->type() != ItemTypes::Planet)
            return;

        //notify ManageGame about the pos change of this fighter
        if(m_currentlySelected->type() == ItemTypes::Fighter) {
            ManageGame::getinstance()->change_Fighter_position(static_cast<GraphicsPlanetItem*>(item)->getID(),
                                                               static_cast<GraphicsFighterItem*>(m_currentlySelected)->getID());
        }
        //notify ManageGame about the new target planet so that it recalculate its path.
        if(m_currentlySelected->type() == ItemTypes::Transporter) {
            ManageGame::getinstance()->change_transport_route(static_cast<GraphicsPlanetItem*>(item)->getID(),
                                                               static_cast<GraphicsTransporterItem*>(m_currentlySelected)->getID());
        }

    } else {
        return;
    }
}

void Scene2dHandler::handlePlanetSelection(GraphicsPlanetItem* planet)
{
    planet->selected();
    update();
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
    update();
    m_currentlySelected = fighter;
}

void Scene2dHandler::handleTransporterSelection(GraphicsTransporterItem* transporter)
{
    transporter->selected();
    update();
    m_currentlySelected = transporter;
}

void Scene2dHandler::unselectAll()
{
    if(m_currentlySelected == nullptr)
        return;

    switch (m_currentlySelected->type()) {
    case ItemTypes::Planet:
        static_cast<GraphicsPlanetItem*>(m_currentlySelected)->selected(false);
        break;
    case ItemTypes::Mine:
        //do nothing
        break;
    case ItemTypes::Factory:
        //do nothing
        break;
    case ItemTypes::Fighter:
            static_cast<GraphicsFighterItem*>(m_currentlySelected)->selected(false);
        break;
    case ItemTypes::Transporter:
            static_cast<GraphicsTransporterItem*>(m_currentlySelected)->selected(false);
        break;
    default:
        //ignore all other items in the scene
        break;
    }

    update();
    m_currentlySelected = nullptr;
}

void Scene2dHandler::placeFighter()
{
    auto fighters = ManageGame::getinstance()->get_attackSpaceCraftList();
    auto planets = MapFactory::getinstance().getMap(setting.value("Dateipfade/Map").toString().toStdString())->getPlanets();
    auto itemList = items();
    auto updatedFighter = QList<QGraphicsItem*>();

    //start animation and placement
    for (const auto& fighter : fighters) {
        bool foundFlag = false;
        //get current GraphicsItem
        for(auto& item : itemList) {
            if(item->type() == ItemTypes::Fighter && ((GraphicsFighterItem*)item)->getID() == fighter->m_id) {
                auto pos = planets[fighter->m_next_position]->getPos();
                foundFlag = true;

                //add updated fighter to list
                updatedFighter.append(item);

                //create and start animation
                auto animationPos = new QPropertyAnimation(static_cast<GraphicsFighterItem*>(item), "pos");
                animationPos->setDuration(2000);
                animationPos->setStartValue(item->pos());
                animationPos->setEndValue(QPointF(pos[0] - 10, pos[1] - 10));

                animationPos->setEasingCurve(QEasingCurve::InOutCubic);

                auto animationRot = new QPropertyAnimation(static_cast<GraphicsFighterItem*>(item), "rotation");
                animationRot->setDuration(500);
                animationRot->setStartValue(item->rotation());
                //It is rotated so that the top is directed to the target
                animationRot->setEndValue(getAngle(QPointF(pos[0], pos[1]), item->pos()) + 90);

                QParallelAnimationGroup *group = new QParallelAnimationGroup;
                group->addAnimation(animationPos);
                group->addAnimation(animationRot);

                group->start();

            }
        }

        //if no fighter found, create a new one
        if(!foundFlag) {
            auto newFighter = new GraphicsFighterItem(fighter->m_owner, fighter->m_id);
            auto pos = planets[fighter->m_position]->getPos();
            newFighter->setPos(pos[0] - 20, pos[1] - 20);
            addItem(newFighter);

            //add updated fighter to list
            updatedFighter.append(newFighter);
        }
    }

    //remove all unused fighter
    for(auto& item : itemList) {
        if(!updatedFighter.contains(item) && item->type() == ItemTypes::Fighter) {
            removeItem(item);
        }
    }

    update();
}

void Scene2dHandler::placeTransporter()
{
    auto transporters = ManageGame::getinstance()->get_transportSpaceCraftList();
    auto planets = MapFactory::getinstance().getMap(setting.value("Dateipfade/Map").toString().toStdString())->getPlanets();
    auto itemList = items();

    //start animation and placement
    for (const auto& transporter : transporters) {

        bool foundFlag = false;
        //get current GraphicsItem
        for(auto& item : itemList) {
            //move fighter if planet changed
            if(item->type() == ItemTypes::Transporter && static_cast<GraphicsTransporterItem*>(item)->getID() == transporter->m_id) {
                auto pos = planets[transporter->m_next_position]->getPos();
                static_cast<GraphicsTransporterItem*>(item)->setOwner(transporter->m_owner);
                foundFlag = true;

                //create and start animation
                auto animationPos = new QPropertyAnimation(static_cast<GraphicsFighterItem*>(item), "pos");
                animationPos->setDuration(2000);
                animationPos->setStartValue(item->pos());
                animationPos->setEndValue(QPointF(pos[0] + 50, pos[1] + 50));

                animationPos->setEasingCurve(QEasingCurve::InOutCubic);

                auto animationRot = new QPropertyAnimation(static_cast<GraphicsFighterItem*>(item), "rotation");
                animationRot->setDuration(500);
                animationRot->setStartValue(item->rotation());
                //It is rotated so that the top is directed to the target
                animationRot->setEndValue(getAngle(QPointF(pos[0], pos[1]), item->pos()) + 90);

                QParallelAnimationGroup *group = new QParallelAnimationGroup;
                group->addAnimation(animationPos);
                group->addAnimation(animationRot);

                group->start();
            }
        }

        //if no transporter found, create a new one
        if(!foundFlag) {
            auto newTransporter = new GraphicsTransporterItem(transporter->m_id);
            auto pos = planets[transporter->m_position]->getPos();
            newTransporter->setPos(pos[0] + 50, pos[1] + 50);
            newTransporter->setOwner(transporter->m_owner);
            addItem(newTransporter);
        }
    }

    update();
}

void Scene2dHandler::updateMap()
{
    MapFactory& f = MapFactory::getinstance();
    auto map = f.getMap(setting.value("Dateipfade/Map").toString().toStdString());
    auto planets = map->getPlanets();
    auto itemList = items();

    for(auto& item : itemList) {
        if (item->type() == ItemTypes::Planet) {
            static_cast<GraphicsPlanetItem*>(item)->setOwner(planets[static_cast<GraphicsPlanetItem*>(item)->getID()]->getOwner());
        }
    }

    //draw mines and factorys
    for(const auto& planet : planets) {

        //draw a mine icon if at least one mine exist on this planet
        if (itemAt(planet->getPos()[0], planet->getPos()[1], QTransform())->type() != ItemTypes::Mine
                && planet->getMine()) {
            auto mineItem = new GraphicsMineItem();
            mineItem->setPos(planet->getPos()[0] + 5, planet->getPos()[1] + 10);
            addItem(mineItem);
        }

        //draw a factory icon if a factory exist on this planet
        if (itemAt(planet->getPos()[0], planet->getPos()[1], QTransform())->type() != ItemTypes::Factory
                && planet->getFactorys()) {
            auto mineItem = new GraphicsFactoryItem();
            mineItem->setPos(planet->getPos()[0] + 25, planet->getPos()[1] + 10);
            addItem(mineItem);
        }
    }

}

double Scene2dHandler::getAngle(QPointF target, QPointF source)
{
    //calc angle in radian
    double angle = qAtan2(target.y() - source.y(), target.x() - source.x());

    //change angle to degree
    return angle * 180 / M_PI;
}

void Scene2dHandler::updateRound() {
    placeFighter();
    placeTransporter();
    updateMap();
}
}
