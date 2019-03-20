#ifndef SCENE2DHANDLER_H
#define SCENE2DHANDLER_H

#include <QGraphicsScene>

class Scene2dHandler : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene2dHandler(QObject* parent = nullptr);
};

#endif // SCENE2DHANDLER_H
