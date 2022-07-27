#ifndef METER_H
#define METER_H

#include <QGraphicsScene>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "Health.h"
#include "Hunger.h"
#include "Satisfaction.h"

class Meter: public QObject
{
    Q_OBJECT //needs Q_OBJECT macro

    private:
        int m_medicine = 2;

        QPushButton * m_feed;
        QPushButton * m_heal;
        QLabel * m_score_display;

        QTimer * m_medicine_timer;
        QTimer * m_hunger_timer;
        QTimer * m_health_timer;
        QTimer * m_score_timer;

        Health * m_heart[3];
        Hunger * m_berry[3];

    public:
        Meter(QGraphicsScene * scene=0);
        ~Meter();

        Satisfaction * m_morale;
        int m_score = 0;

        void setScore(int new_score);
        int getScore();

    public slots:
        void healthDecrease();
        void healthIncrease();
        void medicineUp();
        void eat();
        void starve();
        void happinessChange();
        void increaseScore();
};


#endif // METER_H
