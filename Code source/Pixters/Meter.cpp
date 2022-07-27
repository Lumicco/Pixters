#include "Meter.h"

#include <QAudioOutput>
#include <QMediaPlayer>

Meter::Meter(QGraphicsScene * scene)
{
    //add feed button
    m_feed = new QPushButton();
    m_feed->setIcon(QIcon(":/images/Resources/Feed.png"));
    m_feed->setIconSize(QSize(50, 50));
    m_feed->move(0, -20);
    m_feed->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));  //transparent background
    m_feed->setToolTip("Nourrir");
    m_feed->setCursor(Qt::PointingHandCursor);

    m_feed->show();

    scene->addWidget(m_feed);

    connect(m_feed, SIGNAL(clicked()), this, SLOT(eat()));

    //add heal button
    m_heal = new QPushButton();
    m_heal->setIcon(QIcon(":/images/Resources/Medicine2.png"));
    m_heal->setIconSize(QSize(50, 50));
    m_heal->move(8, 40);
    m_heal->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));  //transparent background
    m_heal->setToolTip("Soigner");
    m_heal->setCursor(Qt::PointingHandCursor);

    m_heal->show();

    scene->addWidget(m_heal);

    connect(m_heal, SIGNAL(clicked()), this, SLOT(healthIncrease()));

    //add timer for medicine increase
    m_medicine_timer = new QTimer();
    connect(m_medicine_timer, SIGNAL(timeout()), this, SLOT(medicineUp()));
    m_medicine_timer->start(10000);

    //create score display
    m_score_display = new QLabel();
    m_score_display->setText(QString("Score : ").append("%1").arg(m_score));
    m_score_display->setTextFormat(Qt::PlainText);
    m_score_display->setAlignment(Qt::AlignLeft);
    m_score_display->setGeometry(270,10,250,30); //x, y, w, h
    m_score_display->setAttribute(Qt::WA_TranslucentBackground);
    m_score_display->setStyleSheet("font-size: 24px; font-weight: bold; font-family: 'Courier New', monospace; color: black");

    m_score_display->show();

    scene->addWidget(m_score_display);

    //add timer for score change
    m_score_timer = new QTimer();
    connect(m_score_timer, SIGNAL(timeout()), this, SLOT(increaseScore()));
    m_score_timer->start(1000);

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
    m_health_timer = new QTimer();
    connect(m_health_timer, SIGNAL(timeout()), this, SLOT(healthDecrease()));
    m_health_timer->start(5000);

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
    x = 310;

    m_morale = new Satisfaction();
    m_morale->setPos(x,y);

    scene->addItem(m_morale);

    //add timer for stat change
    m_hunger_timer = new QTimer();
    connect(m_hunger_timer, SIGNAL(timeout()), this, SLOT(starve()));
    connect(m_hunger_timer, SIGNAL(timeout()), this, SLOT(happinessChange()));
    m_hunger_timer->start(3000);
}

//destructor
Meter::~Meter()
{
    //delete member objects
    delete(m_feed);
    delete(m_hunger_timer);
    delete(m_health_timer);
    delete(m_score_timer);

    for(int i = 0; i < 3; i++)
    {
        delete(m_berry[i]);
        delete(m_heart[i]);
    }

    delete(m_morale);
}

void Meter::healthDecrease()
{
    //reduce number of hearts by 1 when berries are depleted
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

            //play alert sound
            QAudioOutput * audioOutput = new QAudioOutput(this);
            QMediaPlayer * sound = new QMediaPlayer(this);
            sound->setAudioOutput(audioOutput);
            audioOutput->setVolume(50);
            sound->setSource(QUrl("qrc:/sounds/Resources/Sounds/alert.wav"));
            sound->play();
        }
        else if(m_heart[0]->getHealth() == 1)
        {
            m_heart[0]->healthDown();
            m_heart[0]->setHealth(0);
        }
    }
}

void Meter::healthIncrease()
{
    //play healing sound
    QAudioOutput * audioOutput = new QAudioOutput(this);
    QMediaPlayer * sound = new QMediaPlayer(this);
    sound->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);
    //if medcicine not empty play healing sound
    if(m_medicine > 0)
    {
        sound->setSource(QUrl("qrc:/sounds/Resources/Sounds/bweep.wav"));
    }
    //if medicine empty play alert sound
    else
    {
        sound->setSource(QUrl("qrc:/sounds/Resources/Sounds/no_medicine.wav"));
    }
    sound->play();

    //do not use medicine if health is full
    if(m_heart[0]->getHealth() < 3)
    {
        //only heal when medicine not empty
        if(m_medicine != 0)
        {
            //increase number of hearts by 1
            if(m_heart[0]->getHealth() == 0)
            {
                m_heart[0]->healthUp();
                m_heart[0]->setHealth(1);
            }
            else if(m_heart[0]->getHealth() == 1)
            {
                m_heart[1]->healthUp();
                m_heart[0]->setHealth(2);
            }
            else if(m_heart[0]->getHealth() == 2)
            {
                m_heart[2]->healthUp();
                m_heart[0]->setHealth(3);
            }
        }

        //change icon according to number of medicines and deplete medicine
        if(m_medicine == 2)
        {
            m_heal->setIcon(QIcon(":/images/Resources/Medicine1.png"));
            m_medicine--;
        }
        else if(m_medicine == 1)
        {
            m_heal->setIcon(QIcon(":/images/Resources/Medicine0.png"));
            m_medicine--;
        }
    }
}

void Meter::medicineUp()
{
    //change icon according to number of medicines and increase medicine
    if(m_medicine == 0)
    {
        m_heal->setIcon(QIcon(":/images/Resources/Medicine1.png"));
        m_medicine++;
    }
    else if(m_medicine == 1)
    {
        m_heal->setIcon(QIcon(":/images/Resources/Medicine2.png"));
        m_medicine++;
    }
}

void Meter::starve()
{
    //reduce number of berries by 1
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
    //play eating sound
    QAudioOutput * audioOutput = new QAudioOutput(this);
    QMediaPlayer * sound = new QMediaPlayer(this);
    sound->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);
    sound->setSource(QUrl("qrc:/sounds/Resources/Sounds/eating.wav"));
    sound->play();

    //increase number of berries by 1
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
        m_morale->setPixmap(QPixmap(":/images/Resources/Happiness3.png").scaledToHeight(60));
    }
    else if(m_berry[0]->getHunger() == 2)
    {
        m_morale->setPixmap(QPixmap(":/images/Resources/Happiness2.png").scaledToHeight(60));
    }
    else if(m_berry[0]->getHunger() == 1)
    {
        m_morale->setPixmap(QPixmap(":/images/Resources/Happiness1.png").scaledToHeight(60));
    }
    else if(m_heart[0]->getHealth() == 0)
    {
        m_morale->setPixmap(QPixmap(":/images/Resources/Death.png").scaledToHeight(80));
        m_morale->setDeath(true);
    }
}

void Meter::increaseScore()
{
    m_score++;
    m_score_display->setText(QString("Score : ").append("%1").arg(m_score));
}

void Meter::setScore(int new_score)
{
    m_score = new_score;
}

int Meter::getScore()
{
    return m_score;
}
