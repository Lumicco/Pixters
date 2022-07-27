#include "Satisfaction.h"

Satisfaction::Satisfaction(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    //set item image and size
    setPixmap(QPixmap(":/images/Resources/Happiness3.png").scaledToHeight(60));
}

void Satisfaction::setDeath(bool status)
{
    m_death = status;

    emit valueChanged();
}

bool Satisfaction::getDeath()
{
    return m_death;
}

