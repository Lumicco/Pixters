#include "Satisfaction.h"

Satisfaction::Satisfaction(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    //set item image and size
    setPixmap(QPixmap(":/images/Resources/Happiness3.png").scaledToHeight(80));
}
