#include "Session.h"
#include <QBrush>
#include <QImage>
#include <QMediaPlayer>
#include <QAudioOutput>

#include <QDebug>

Session::Session(QWidget * parent)
{
    //create a scene
    m_scene = new QGraphicsScene();
    m_scene->setSceneRect(0,0,670,430);

    //make the window
    setScene(m_scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(672,432);

    //set background
    setBackgroundBrush(QBrush(QImage(":/images/Resources/Pixters.png")));

    //add invisible start button over logo
    m_play = new QPushButton();
    m_play->setGeometry(120, 165, 430, 205);
    m_play->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));
    m_play->setCursor(Qt::PointingHandCursor);

    m_play->show();
    m_scene->addWidget(m_play);

    //restart session when play again button is clicked
    connect(m_play, SIGNAL(clicked()), this, SLOT(start()));
}

void Session::start()
{
    if(m_play != NULL)
    {
        qDebug() << "null";
        m_play->hide();
    }

    //set background
    setBackgroundBrush(QBrush(QImage(":/images/Resources/Background.png")));

    //create the pet
    m_pet = new Pet();
    m_pet->setPos(265,170);

    //create label with name of pet
    m_name = new QLabel();
    m_name->setText(m_pet->getName());
    m_name->setTextFormat(Qt::PlainText);
    m_name->setAlignment(Qt::AlignCenter);
    m_name->setGeometry(285,80,100,30); //x, y, w, h
    m_name->setAttribute(Qt::WA_TranslucentBackground);
    m_name->setStyleSheet("font-size: 22px; font-family: 'Ink Free', 'Segoe Print' ; color: white");

    //create the meter
    m_meter = new Meter(m_scene);

    //add pet to scene
    m_scene->addItem(m_pet);
    m_scene->addWidget(m_name);

    //play background music
    QAudioOutput * audioOutput = new QAudioOutput(this);
    m_music = new QMediaPlayer(this);
    m_music->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);
    m_music->setSource(QUrl("qrc:/sounds/Resources/Sounds/background_music.wav"));
    m_music->setLoops(QMediaPlayer::Infinite);
    m_music->play();

    //trigger game over once death value is changed
    connect(m_meter->m_morale, SIGNAL(valueChanged()), this, SLOT(gameOver()));
}

void Session::gameOver()
{
    //remove items from scene
    delete(m_meter);
    m_scene->clear();
    m_play = NULL;

    //stop music
    m_music->stop();

    //change background image
    setBackgroundBrush(QBrush(QImage(":/images/Resources/Game_Over.png")));

    //add play again button
    m_play_again = new QPushButton();
    m_play_again->setIcon(QIcon(":/images/Resources/Play_Again.png"));
    m_play_again->setIconSize(QSize(200, 80));
    m_play_again->move(380, 90);
    m_play_again->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));
    m_play_again->setCursor(Qt::PointingHandCursor);

    m_play_again->show();
    m_scene->addWidget(m_play_again);

    //restart session when play again button is clicked
    connect(m_play_again, SIGNAL(clicked()), this, SLOT(restartSession()));

    //add quit button
    m_quit = new QPushButton();
    m_quit->setIcon(QIcon(":/images/Resources/Quit.png"));
    m_quit->setIconSize(QSize(200, 80));
    m_quit->move(380, 200);
    m_quit->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));
    m_quit->setCursor(Qt::PointingHandCursor);

    m_quit->show();
    m_scene->addWidget(m_quit);

    //close program when quit button is clicked
    connect(m_quit, SIGNAL(clicked()), this, SLOT(close()));
}

void Session::restartSession()
{
    //remove game over buttons
    m_play_again->hide();
    m_quit->hide();

    //start new session
    start();
}
