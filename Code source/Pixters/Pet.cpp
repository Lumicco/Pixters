#include "Pet.h"
#include <QPointF>
#include <QTimer>

Pet::Pet(QString type, QGraphicsItem *parent): QGraphicsPixmapItem(parent), m_type(type)
{

    //set item image and size
    if(m_type == "Papillon de mer")
    {
        m_pixter.load(":/images/Resources/Pets/Pixter1.png");
    }
    else if(m_type == "Chien")
    {
        m_pixter.load(":/images/Resources/Pets/Pixter2.png");
    }
    else if(m_type == "Chenille")
    {
        m_pixter.load(":/images/Resources/Pets/Pixter3.png");
    }
    else if(m_type == "Papillon")
    {
        m_pixter.load(":/images/Resources/Pets/Pixter4.png");
    }
    else if(m_type == "Serpent")
    {
        m_pixter.load(":/images/Resources/Pets/Pixter5.png");
    }
    else if(m_type == "Autre")
    {
        m_pixter.load(":/images/Resources/Pets/Pixter6.png");
    }
    else if(m_type == "Chat")
    {
        m_pixter.load(":/images/Resources/Pets/Pixter7.png");
    }
    else if(m_type == "Lézard")
    {
        m_pixter.load(":/images/Resources/Pets/Pixter8.png");
    }

    setPixmap(m_pixter.scaledToHeight(140));

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
        setPixmap(m_pixter.scaledToHeight(140));
    }
    else
    {
        setPos(x()+20, y());
        setPixmap((m_pixter.scaledToHeight(140)).transformed(QTransform().scale(-1, 1)));
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

void Pet::setType(QString type)
{
    m_type = type;
}

QString Pet::getType()
{
    return m_type;
}
