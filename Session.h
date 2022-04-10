#ifndef SESSION_H
#define SESSION_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QLabel>
#include "Pet.h"
#include "Meter.h"

class Session: public QGraphicsView
{
    public:
        Session(QWidget * parent=0);
        void gameOver();

        QGraphicsScene * scene;
        Pet * pet;
        QLabel * name;
        Meter * meter;
};

#endif // SESSION_H
