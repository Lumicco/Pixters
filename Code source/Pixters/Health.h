#ifndef HEALTH_H
#define HEALTH_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class Health: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT //needs Q_OBJECT macro

    private:
        int m_health_nb = 3;

    public:
        Health(QGraphicsItem *parent=0);
        int getHealth();
        void setHealth(int health_nb);

    public slots:
        void healthUp();
        void healthDown();
};

#endif // HEALTH_H
