#include "Pet.h"
#include <QPointF>
#include <QTimer>

Pet::Pet(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    //set item image and size
    setPixmap(QPixmap(":/images/Resources/Pets/Pixter1.png").scaledToHeight(140));

    //connect
    QTimer * timer = new QTimer(); //slot executes everytime timer reaches 0
    connect(timer, SIGNAL(timeout()), this, SLOT(move())); //connect timeout signal from timer object to to move slot from this(pet) object

    timer->start(1000); //sets timer's time to 1s
}

void Pet::move()
{
    QPointF position;
    position = scenePos();
    int posX = position.x();

    //prevent pet going out of bounds
    if(posX <= 20)
    {
        setPos(x()+20, y());
    }
    else if(posX >= 500)
    {
        setPos(x()-20, y());
    }

    //move pet randomly left and right, mirror icon when going right
    if (rand() % 2 + 1 == 1)
    {
        setPos(x()-20, y());
        setPixmap(QPixmap(":/images/Resources/Pets/Pixter1.png").scaledToHeight(140));
    }
    else
    {
        setPos(x()+20, y());
        setPixmap((QPixmap(":/images/Resources/Pets/Pixter1.png").scaledToHeight(140)).transformed(QTransform().scale(-1, 1)));
    }
}

void Pet::setName(QString new_name)
{
    m_name = new_name;
}

QString Pet::getName()
{
    return m_name;
}
