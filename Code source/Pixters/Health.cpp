#include "Health.h"

Health::Health(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    //set item image and size
    setPixmap(QPixmap(":/images/Resources/Heart.png").scaledToHeight(60));
}

void Health::setHealth(int new_health)
{
    m_health_nb = new_health;
}

int Health::getHealth()
{
    return m_health_nb;
}

void Health::healthUp()
{
    this->setVisible(true);
}

void Health::healthDown()
{
    this->setVisible(false);
}
