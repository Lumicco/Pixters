#include "Session.h"
#include <QBrush>
#include <QImage>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QInputDialog>
#include <QSqlDatabase>

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
    m_start = new QPushButton();
    m_start->setGeometry(120, 165, 430, 205);
    m_start->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));
    m_start->setCursor(Qt::PointingHandCursor);

    m_start->show();
    m_scene->addWidget(m_start);

    //play title screen music
    QAudioOutput * audioOutput = new QAudioOutput(this);
    m_music = new QMediaPlayer(this);
    m_music->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);
    m_music->setSource(QUrl("qrc:/sounds/Resources/Sounds/title_screen.wav"));
    m_music->setLoops(QMediaPlayer::Infinite);
    m_music->play();

    //start session setup when button is clicked
    connect(m_start, SIGNAL(clicked()), this, SLOT(setup()));
}

void Session::setup()
{
    if(m_start != NULL)
    {
        m_start->hide();
    }

    //enter username
    m_username = QInputDialog::getText(this, tr("Entrez votre nom"),
                                             tr("Votre nom:"), QLineEdit::Normal);

    //choose favourite animal
    QStringList items;
    items << tr("Papillon de mer") << tr("Chien") << tr("Chenille") << tr("Papillon")
          << tr("Serpent") << tr("Chat") << tr("Lézard") << tr("Autre");

    m_pet_type = QInputDialog::getItem(this, tr("Quel est votre animal préféré ?"),
                                             tr("Votre animal préféré:"), items, 0, false);

    //enter pet name
    m_pet_name = QInputDialog::getText(this, tr("Entrez le nom de votre pixter"),
                                             tr("Le nom de votre pixter:"), QLineEdit::Normal);

    //connect to database
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("pixters");
    db.setUserName("root");
    db.setPassword("AT2jgTMEx-cHeIT9");

    if(db.open())
    {
        qDebug() << "success";
    }

    //start session after last popup
    start();
}

void Session::start()
{
    //set background
    setBackgroundBrush(QBrush(QImage(":/images/Resources/Background.png")));

    //create the pet
    m_pet = new Pet();
    m_pet->setPos(265,170);

    //set pet name to name entered by user
    if(!m_pet_name.isEmpty())
    {
        m_pet->setName(m_pet_name);
    }


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
    m_start = NULL;

    //play background music
    m_music->setSource(QUrl("qrc:/sounds/Resources/Sounds/game_over.wav"));
    m_music->setLoops(1);
    m_music->play();

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
