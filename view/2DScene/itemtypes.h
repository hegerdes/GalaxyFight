#ifndef ITEMTYPES_H
#define ITEMTYPES_H

#include <QGraphicsItem>

namespace asteroids {

enum ItemTypes { Planet = QGraphicsItem::UserType + 1, Transporter = QGraphicsItem::UserType + 2,
       Fighter = QGraphicsItem::UserType + 3, Mine = QGraphicsItem::UserType + 4,
       Factory = QGraphicsItem::UserType + 5 };

enum PlayerType { UNASSIGNED, PLAYER1, PLAYER2 };
}
#endif // ITEMTYPES_H
