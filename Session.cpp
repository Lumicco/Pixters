#include "Session.h"
#include <QBrush>
#include <QImage>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QInputDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QMessageBox>

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

    //add contract button
    m_contract = new QPushButton();
    m_contract->setIcon(QIcon(":/images/Resources/File.png"));
    m_contract->setIconSize(QSize(40, 40));
    m_contract->move(600, -20);
    m_contract->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));  //transparent background
    m_contract->setToolTip("Mentions légales");
    m_contract->setCursor(Qt::PointingHandCursor);

    m_contract->show();

    m_scene->addWidget(m_contract);

    connect(m_contract, SIGNAL(clicked()), this, SLOT(showContract()));

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
    m_username = QInputDialog::getText(this, "Entrez votre nom",
                                             "Votre nom:", QLineEdit::Normal);

    //initialize username if no name entered
    if(m_username.length() == 0)
    {
        m_username = "joueur";
    }

    //choose favourite animal
    QStringList items;
    items << "Papillon de mer" << "Chien" << "Chat" << "Chenille"
          << "Papillon" << "Serpent" << "Lézard" << "Autre";

    m_pet_type = QInputDialog::getItem(this, "Quel est votre animal préféré ?",
                                             "Votre animal préféré:", items, 0, false);


    //enter pet name
    m_pet_name = QInputDialog::getText(this, "Entrez le nom de votre pixter",
                                             "Le nom de votre pixter:", QLineEdit::Normal);

    //check if pet name is too long
    while(m_pet_name.length() > 8)
    {
        m_pet_name = QInputDialog::getText(this, "Le nom est trop long",
                                                 "Le nom doit faire moins que 8 caractères:", QLineEdit::Normal);
    }

    //initialize pet name if no name entered
    if(m_pet_name.length() == 0)
    {
        m_pet_name = "Choupette";
    }

    //connect to database
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("pixters");
    db.setUserName("root");
    db.setPassword("AT2jgTMEx-cHeIT9");

    //open database
    if(!db.open())
    {
        qDebug() << "DB Connect Error: " << db.lastError();
    }

    //initialize variable to store pet id
    int pet_type = 1;

    //prepare select statement for pet id
    QSqlQuery get_type;
    get_type.prepare("SELECT id_pet FROM pets WHERE species like :species");
    get_type.bindValue(":species", m_pet_type);

    //execute statement
    if(!get_type.exec())
    {
        qDebug() << "SQL Statement Error: " << get_type.lastError();
    }

    //store result in variable (0 = column index)
    while (get_type.next())
    {
        pet_type = get_type.value(0).toInt();
    }

    //prepare insert statement
    QSqlQuery insert_val;
    insert_val.prepare("INSERT INTO users (username, pet_name, id_pet)"
                  "VALUES (:username, :pet_name, :id_pet)");
    insert_val.bindValue(":username", m_username);
    insert_val.bindValue(":pet_name", m_pet_name);
    insert_val.bindValue(":id_pet", pet_type);

    //execute statement
    if(!insert_val.exec())
    {
        qDebug() << "SQL Statement Error: " << insert_val.lastError();
    }

    //prepare select statement for last inserted user id
    QSqlQuery get_user;
    get_user.prepare("SELECT LAST_INSERT_ID() FROM users");

    //execute statement
    if(!get_user.exec())
    {
        qDebug() << "SQL Statement Error: " << get_user.lastError();
    }

    //store result in variable (0 = column index)
    while (get_user.next())
    {
        m_id_user = get_user.value(0).toInt();
    }

    //start session after last popup
    start();
}

void Session::start()
{
    //set background
    setBackgroundBrush(QBrush(QImage(":/images/Resources/Background.png")));

    //create the pet
    m_pet = new Pet(m_pet_type);
    m_pet->setPos(265,170);
    m_pet->setName(m_pet_name);

    //create label with name of pet
    m_name = new QLabel();
    m_name->setText(m_pet->getName());
    m_name->setTextFormat(Qt::PlainText);
    m_name->setAlignment(Qt::AlignCenter);
    m_name->setGeometry(270,80,135,30); //x, y, w, h
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
    //store score in variable
    int score;
    score = m_meter->getScore();

    //prepare select statement for high score
    QSqlQuery get_highscore;
    get_highscore.exec("SELECT score, username FROM scores, users WHERE scores.id_user=users.id_user AND score=(SELECT max(score) FROM scores)");

    //execute statement
    if(!get_highscore.exec())
    {
        qDebug() << "SQL Statement Error: " << get_highscore.lastError();
    }

    //store result in variable (0 = column index)
    while (get_highscore.next())
    {
        m_highscore = get_highscore.value(0).toInt();
        m_highscore_user = get_highscore.value(1).toString();
    }

    //set new high score if session score higher than high score
    if(m_highscore < score)
    {
        m_highscore = score;
    }

    //prepare update statement for high score
    QSqlQuery insert_score;
    insert_score.prepare("INSERT INTO scores(score, date, id_user)"
                         "VALUES (:score, :date, :id_user)");
    insert_score.bindValue(":score", score);
    insert_score.bindValue(":date", QDateTime::currentDateTime().toString());
    insert_score.bindValue(":id_user", m_id_user);

    //execute statement
    if(!insert_score.exec())
    {
        qDebug() << "SQL Statement Error: " << insert_score.lastError();
    }

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

    //create score display
    m_score_display = new QLabel();
    m_score_display->setText(QString("Votre score : ").append("%1").arg(score));
    m_score_display->setTextFormat(Qt::PlainText);
    m_score_display->setAlignment(Qt::AlignLeft);
    m_score_display->setGeometry(380,20,300,30); //x, y, w, h
    m_score_display->setAttribute(Qt::WA_TranslucentBackground);
    m_score_display->setStyleSheet("font-size: 22px; font-weight: bold; font-family: 'Courier New', monospace; color: black");

    m_score_display->show();

    m_scene->addWidget(m_score_display);

    //initialize string for high score diplay
    QString display = "Meilleur score : ";
    display.append(QString::number(m_highscore));

    //show high score
    m_highscore_display = new QLabel();
    m_highscore_display->setText(display);
    m_highscore_display->setTextFormat(Qt::PlainText);
    m_highscore_display->setAlignment(Qt::AlignLeft);
    m_highscore_display->setGeometry(380,45,300,60); //x, y, w, h
    m_highscore_display->setAttribute(Qt::WA_TranslucentBackground);
    m_highscore_display->setStyleSheet("font-size: 22px; font-weight: bold; font-family: 'Courier New', monospace; color: black");
    m_highscore_display->setToolTip(QString("par ").append(m_highscore_user));

    m_highscore_display->show();

    m_scene->addWidget(m_highscore_display);

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

    m_score_display->hide();
    m_highscore_display->hide();

    //start new session
    start();
}

void Session::showContract()
{
    QMessageBox msgBox;
    msgBox.setText("Pixters est édité par Laura Huber.\n"
                   "Tous les éléments de ce site, y compris les graphismes et les bruitages, ont été créés sur mesure. La reproduction des éléments "
                   "de cette application est autorisée à la condition d\'y mentionner la source. Elles ne peuvent être utilisées à des fins commerciales "
                   "et publicitaires.");
    msgBox.exec();
}

QString Session::getUsername()
{
    return m_username;
}

QString Session::getPetType()
{
    return m_pet_type;
}

QString Session::getPetName()
{
    return m_pet_name;
}

void Session::setUsername(QString username)
{
    m_username = username;
}

void Session::setPetType(QString pet_type)
{
    m_pet_type = pet_type;
}

void Session::setPetName(QString pet_name)
{
    m_pet_name = pet_name;
}

