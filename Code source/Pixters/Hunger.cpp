#include "Hunger.h"

Hunger::Hunger(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    //set item image and size
    setPixmap(QPixmap(":/images/Resources/Strawberry.png").scaledToHeight(60));
}

void Hunger::setHunger(int new_hunger)
{
    m_berry_nb = new_hunger;
}

int Hunger::getHunger()
{
    return m_berry_nb;
}

void Hunger::hungerUp()
{
    this->setVisible(true);
}

void Hunger::hungerDown()
{
    this->setVisible(false);
}
