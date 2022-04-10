#ifndef SATISFACTION_H
#define SATISFACTION_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class Satisfaction: public QObject, public QGraphicsPixmapItem
{
    public:
        Satisfaction(QGraphicsItem *parent=0);
};

#endif // SATISFACTION_H
