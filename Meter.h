#ifndef METER_H
#define METER_H

#include <QGraphicsScene>
#include <QObject>
#include <QPushButton>
#include <QTimer>
#include "Health.h"
#include "Hunger.h"
#include "Satisfaction.h"

class Meter: public QObject
{
    Q_OBJECT //needs Q_OBJECT macro

    public:
        Meter(QGraphicsScene * scene=0);
        ~Meter();
        QPushButton * m_feed;
        QTimer * m_timer1;
        QTimer * m_timer2;
        Health * m_heart[3];
        Hunger * m_berry[3];
        Satisfaction * m_morale;

    public slots:
        void healthChange();
        void eat();
        void starve();
        void happinessChange();
};


#endif // METER_H
