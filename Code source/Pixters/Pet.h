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
        QString m_name;
        QString m_type;
        QPixmap m_pixter;


    public:
        Pet(QString type, QGraphicsItem *parent=0);

        void setName(QString name);
        QString getName();

        void setType(QString type);
        QString getType();

    public slots:
        void move();
};

#endif // PET_H
