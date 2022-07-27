#ifndef SATISFACTION_H
#define SATISFACTION_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class Satisfaction: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT //needs Q_OBJECT macro

    private:
        bool m_death = false;

    public:
        Satisfaction(QGraphicsItem *parent=0);

        bool getDeath();

    public slots:
        void setDeath(bool status);

    signals:
        void valueChanged();
};

#endif // SATISFACTION_H
