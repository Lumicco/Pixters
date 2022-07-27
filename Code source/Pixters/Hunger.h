#ifndef HUNGER_H
#define HUNGER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class Hunger: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT //needs Q_OBJECT macro

    private:
        int m_berry_nb = 3;

    public:
        Hunger(QGraphicsItem *parent=0);
        int getHunger();
        void setHunger(int hunger_nb);

    public slots:
        void hungerUp();
        void hungerDown();
};

#endif // HUNGER_H
