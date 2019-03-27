#ifndef ITEMTYPES_H
#define ITEMTYPES_H

#include <QGraphicsItem>
#include "../../rendering/2D/PlanetChanges.hpp"

namespace asteroids {

enum ItemTypes { Planet = QGraphicsItem::UserType + 1, Transporter = QGraphicsItem::UserType + 2,
       Fighter = QGraphicsItem::UserType + 3, Mine = QGraphicsItem::UserType + 4,
       Factory = QGraphicsItem::UserType + 5, HQ = QGraphicsItem::UserType + 5 };

using PlayerType = PlanetChanges::Owner;
}
#endif // ITEMTYPES_H
