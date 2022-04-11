#include "Meter.h"

#include <QAudioOutput>
#include <QMediaPlayer>

Meter::Meter(QGraphicsScene * scene)
{
    //add feed button
    m_feed = new QPushButton();
    m_feed->setIcon(QIcon(":/images/Resources/Feed.png"));
    m_feed->setIconSize(QSize(50, 50));
    m_feed->move(10, -20);
    m_feed->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));  //transparent background
    m_feed->setToolTip("Nourrir");
    m_feed->setCursor(Qt::PointingHandCursor);

    m_feed->show();

    scene->addWidget(m_feed);

    connect(m_feed, SIGNAL(clicked()), this, SLOT(eat()));

    //add timer for stat change
    m_timer1 = new QTimer();
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(starve()));
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(happinessChange()));
    m_timer1->start(3000);

    //add three hearts to scene
    int x = 20;
    int y = 348;

    m_heart[0] = new Health();
    m_heart[0]->setPos(x,y);

    m_heart[1] = new Health();
    m_heart[1]->setPos(x+70,y);

    m_heart[2] = new Health();
    m_heart[2]->setPos(x+140,y);

    scene->addItem(m_heart[0]);
    scene->addItem(m_heart[1]);
    scene->addItem(m_heart[2]);

    //add timer for stat change
    m_timer2 = new QTimer();
    connect(m_timer2, SIGNAL(timeout()), this, SLOT(healthChange()));
    m_timer2->start(5000);

    //add three strawberries to scene
    x = 450;

    m_berry[0] = new Hunger();
    m_berry[0]->setPos(x,y);

    m_berry[1] = new Hunger();
    m_berry[1]->setPos(x+70,y);

    m_berry[2] = new Hunger();
    m_berry[2]->setPos(x+140,y);

    scene->addItem(m_berry[0]);
    scene->addItem(m_berry[1]);
    scene->addItem(m_berry[2]);

    //add happiness indicator to scene
    x = 300;

    m_morale = new Satisfaction();
    m_morale->setPos(x,y-10);

    scene->addItem(m_morale);
}

//destructor
Meter::~Meter()
{
    //delete member objects
    delete(m_feed);
    delete(m_timer1);
    delete(m_timer2);

    for(int i = 0; i < 3; i++)
    {
        delete(m_berry[i]);
        delete(m_heart[i]);
    }

    delete(m_morale);
}

void Meter::healthChange()
{
    //reduce number of hearts when berries are depleted
    if(m_berry[0]->getHunger() == 0)
    {
        if(m_heart[0]->getHealth() == 3)
        {
            m_heart[2]->healthDown();
            m_heart[0]->setHealth(2);
        }
        else if(m_heart[0]->getHealth() == 2)
        {
            m_heart[1]->healthDown();
            m_heart[0]->setHealth(1);
        }
        else if(m_heart[0]->getHealth() == 1)
        {
            m_heart[0]->healthDown();
            m_heart[0]->setHealth(0);
        }
    }
}

void Meter::starve()
{
    //reduce number of berries
    if(m_berry[0]->getHunger() == 3)
    {
        m_berry[2]->hungerDown();
        m_berry[0]->setHunger(2);
    }
    else if(m_berry[0]->getHunger() == 2)
    {
        m_berry[1]->hungerDown();
        m_berry[0]->setHunger(1);
    }
    else if(m_berry[0]->getHunger() == 1)
    {
        m_berry[0]->hungerDown();
        m_berry[0]->setHunger(0);
    }
}

void Meter::eat()
{
    //play background music
    QAudioOutput * audioOutput = new QAudioOutput(this);
    QMediaPlayer * sound = new QMediaPlayer(this);
    sound->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);
    sound->setSource(QUrl("qrc:/sounds/Resources/Sounds/eating.wav"));
    sound->play();

    //increase number of berries
    if(m_berry[0]->getHunger() == 0)
    {
        m_berry[0]->hungerUp();
        m_berry[0]->setHunger(1);
        this->happinessChange();
    }
    else if(m_berry[0]->getHunger() == 1)
    {
        m_berry[1]->hungerUp();
        m_berry[0]->setHunger(2);
        this->happinessChange();
    }
    else if(m_berry[0]->getHunger() == 2)
    {
        m_berry[2]->hungerUp();
        m_berry[0]->setHunger(3);
        this->happinessChange();
    }
}

void Meter::happinessChange()
{
    //reduce happiness
    if(m_berry[0]->getHunger() == 3)
    {
        m_morale->setPixmap(QPixmap(":/images/Resources/Happiness3.png").scaledToHeight(80));
    }
    else if(m_berry[0]->getHunger() == 2)
    {
        m_morale->setPixmap(QPixmap(":/images/Resources/Happiness2.png").scaledToHeight(80));
    }
    else if(m_berry[0]->getHunger() == 1)
    {
        m_morale->setPixmap(QPixmap(":/images/Resources/Happiness1.png").scaledToHeight(80));
    }
    else if(m_heart[0]->getHealth() == 0)
    {
        m_morale->setPixmap(QPixmap(":/images/Resources/Death.png").scaledToHeight(80));
        m_morale->setDeath(true);
    }
}
