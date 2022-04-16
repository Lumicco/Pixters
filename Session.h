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

    private:
        int m_highscore = 0;
        QString m_highscore_user = "";

        int m_id_user;
        QString m_username;
        QString m_pet_type;
        QString m_pet_name;

        QPushButton * m_start;
        QPushButton * m_contract;

        Pet * m_pet;
        QLabel * m_name;
        Meter * m_meter;

        QMediaPlayer * m_music;

        QLabel * m_score_display;
        QLabel * m_highscore_display;

        QPushButton * m_play_again;
        QPushButton * m_quit;

    public:
        Session(QWidget * parent=0);
        QGraphicsScene * m_scene;

        QString getUsername();
        QString getPetType();
        QString getPetName();

        void setUsername(QString username);
        void setPetType(QString pet_type);
        void setPetName(QString pet_name);

    public slots:
        void setup();
        void start();
        void gameOver();
        void restartSession();
        void showContract();
};

#endif // SESSION_H
