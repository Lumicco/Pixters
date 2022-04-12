#ifndef PET_H
#define PET_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>
#include <QString>

class Pet: public QObject, public QGraphicsPixmapItem //item needs to derive from QObject to use signals and slots
{
    Q_OBJECT //needs Q_OBJECT macro

    private:
        QString m_name = "Choupette";

    public:
        Pet(QGraphicsItem *parent=0);
        void setName(QString name);
        QString getName();

    public slots:
        void move();
};

#endif // PET_H
