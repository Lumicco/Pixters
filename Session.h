#ifndef SESSION_H
#define SESSION_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QLabel>
#include <QMediaPlayer>
#include "Pet.h"
#include "Meter.h"

class Session: public QGraphicsView
{
    Q_OBJECT //needs Q_OBJECT macro

    public:
        Session(QWidget * parent=0);
        QGraphicsScene * m_scene;

        QPushButton * m_start;

        QString m_username;
        QString m_pet_type;
        QString m_pet_name;

        Pet * m_pet;
        QLabel * m_name;
        Meter * m_meter;

        QMediaPlayer * m_music;

        QPushButton * m_play_again;
        QPushButton * m_quit;

    public slots:
        void setup();
        void start();
        void gameOver();
        void restartSession();
};

#endif // SESSION_H
