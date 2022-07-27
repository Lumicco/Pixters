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
#include <QCryptographicHash>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QHeaderView>

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

    titleScreen();
}

void Session::titleScreen()
{
    //stop music if already playing
    if(m_music != NULL)
    {
        m_music->stop();
    }

    //remove admin menu buttons if necessary
    if(m_back != NULL)
    {
        m_back->hide();
    }

    if(m_scoreboard != NULL)
    {
        m_scoreboard->hide();
    }

    if(m_player_list != NULL)
    {
        m_player_list->hide();
    }

    //set background
    setBackgroundBrush(QBrush(QImage(":/images/Resources/Pixters.png")));

    //add invisible start button over logo
    m_start = new QPushButton();
    m_start->setGeometry(120, 165, 430, 205);
    m_start->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));
    m_start->setCursor(Qt::PointingHandCursor);

    m_start->show();
    m_scene->addWidget(m_start);

    //start session setup when button is clicked
    connect(m_start, SIGNAL(clicked()), this, SLOT(setup()));

    //add contract button
    m_contract = new QPushButton();
    m_contract->setIcon(QIcon(":/images/Resources/File.png"));
    m_contract->setIconSize(QSize(40, 40));
    m_contract->move(600, -20);
    m_contract->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));  //transparent background
    m_contract->setToolTip("Menu d'administration");
    m_contract->setCursor(Qt::PointingHandCursor);

    m_contract->show();
    m_scene->addWidget(m_contract);

    connect(m_contract, SIGNAL(clicked()), this, SLOT(showContract()));

    //add admin menu button
    m_admin_menu = new QPushButton();
    m_admin_menu->setIcon(QIcon(":/images/Resources/Wrench.png"));
    m_admin_menu->setIconSize(QSize(38, 38));
    m_admin_menu->move(550, -18);
    m_admin_menu->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));  //transparent background
    m_admin_menu->setToolTip("Mentions légales");
    m_admin_menu->setCursor(Qt::PointingHandCursor);

    m_admin_menu->show();
    m_scene->addWidget(m_admin_menu);

    connect(m_admin_menu, SIGNAL(clicked()), this, SLOT(openAdminMenu()));

    //play title screen music
    QAudioOutput * audioOutput = new QAudioOutput(this);
    m_music = new QMediaPlayer(this);
    m_music->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);
    m_music->setSource(QUrl("qrc:/sounds/Resources/Sounds/title_screen.wav"));
    m_music->setLoops(QMediaPlayer::Infinite);
    m_music->play();

    //connect to database
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("pixters");
    db.setUserName("root");
    db.setPassword("Sio2022!"); //phpMyAdmin password

    //open database
    if(!db.open())
    {
        qDebug() << "DB Connect Error: " << db.lastError();
    }
}

void Session::setup()
{
    //remove start screen buttons if necessary
    if(m_start != NULL)
    {
        m_start->hide();
    }

    if(m_admin_menu != NULL)
    {
        m_admin_menu->hide();
    }

    if(m_contract != NULL)
    {
        m_contract->hide();
    }

    //enter username
    bool ok;
    m_username = QInputDialog::getText(this, "Entrez votre nom",
                                             "Votre nom:", QLineEdit::Normal, 0, &ok);

    //return to title screen on cancel
    if(ok == false)
    {
        titleScreen();
    }
    else
    {
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
                                                 "Votre animal préféré:", items, 0, false, &ok);

        //return to title screen on cancel
        if(ok == false)
        {
            titleScreen();
        }
        else
        {
            //enter pet name
            m_pet_name = QInputDialog::getText(this, "Entrez le nom de votre pixter",
                                                     "Le nom de votre pixter:", QLineEdit::Normal, 0, &ok);

            //return to title screen on cancel
            if(ok == false)
            {
                titleScreen();
            }
            else
            {
                //check if pet name is too long
                while(m_pet_name.length() > 8)
                {
                    m_pet_name = QInputDialog::getText(this, "Le nom est trop long",
                                                             "Le nom doit faire moins que 8 caractères:", QLineEdit::Normal, 0, &ok);
                }

                //return to title screen on cancel
                if(ok == false)
                {
                    titleScreen();
                }
                else
                {
                    //initialize pet name if no name entered
                    if(m_pet_name.length() == 0)
                    {
                        m_pet_name = "Choupette";
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
            }
        }
    }
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
        m_highscore_user = m_username;
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
                   "Tous les éléments de cette application, y compris les graphismes et les bruitages, ont été créés sur mesure. La reproduction des éléments "
                   "de cette application est autorisée à la condition d\'y mentionner la source. Elles ne peuvent être utilisées à des fins commerciales "
                   "et publicitaires.");
    msgBox.exec();
}

void Session::openAdminMenu()
{
    //remove start screen buttons  if necessary
    if(m_start != NULL)
    {
        m_start->hide();
    }

    if(m_admin_menu != NULL)
    {
        m_admin_menu->hide();
    }

    if(m_contract != NULL)
    {
        m_contract->hide();
    }

    //set background
    setBackgroundBrush(QBrush(QImage(":/images/Resources/Admin_Menu.png")));

    //enter username
    bool ok;
    QString username = QInputDialog::getText(this, "Entrez le nom d'utilisateur",
                                             "Nom d'utilisateur:", QLineEdit::Normal, "admin", &ok);

    //return to title screen on cancel
    if(ok == false)
    {
        titleScreen();
    }
    else
    {
        //enter password
        QString password = QInputDialog::getText(this, "Entrez le mot de passe",
                                                 "Mot de passe:", QLineEdit::Password, 0, &ok);

        //return to title screen on cancel
        if(ok == false)
        {
            titleScreen();
        }
        else
        {
            //prepare select statement for root username
            QSqlQuery get_username;
            get_username.prepare("SELECT username FROM root");

            //execute statement
            if(!get_username.exec())
            {
                qDebug() << "SQL Statement Error: " << get_username.lastError();
            }

            //store result in variable (0 = column index)
            while (get_username.next())
            {
                m_root = get_username.value(0).toString();
            }

            //prepare select statement for (hashed) root password
            QSqlQuery get_password;
            get_password.prepare("SELECT password FROM root");

            //execute statement
            if(!get_password.exec())
            {
                qDebug() << "SQL Statement Error: " << get_password.lastError();
            }

            //store result in variable (0 = column index)
            while (get_password.next())
            {
                m_password = get_password.value(0).toString();
            }

            //hash password entered by user
            password = QString("%1").arg(QString(QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Sha1).toHex()));

            //compare hashes
            if(password == m_password && username == m_root)
            {
                //add back button
                m_back = new QPushButton();
                m_back->setIcon(QIcon(":/images/Resources/Door.png"));
                m_back->setIconSize(QSize(38, 38));
                m_back->move(5, -18);
                m_back->setToolTip("Quitter");
                m_back->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));
                m_back->setCursor(Qt::PointingHandCursor);

                m_back->show();
                m_scene->addWidget(m_back);

                //show scoreboard when scoreboard button is clicked
                connect(m_back, SIGNAL(clicked()), this, SLOT(titleScreen()));

                //add scoreboard button
                m_scoreboard = new QPushButton();
                m_scoreboard->setIcon(QIcon(":/images/Resources/Scoreboard.png"));
                m_scoreboard->setIconSize(QSize(250, 130));
                m_scoreboard->move(200, 40);
                m_scoreboard->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));
                m_scoreboard->setCursor(Qt::PointingHandCursor);

                m_scoreboard->show();
                m_scene->addWidget(m_scoreboard);

                //show scoreboard when scoreboard button is clicked
                connect(m_scoreboard, SIGNAL(clicked()), this, SLOT(showScoreboard()));

                //add player list button
                m_player_list = new QPushButton();
                m_player_list->setIcon(QIcon(":/images/Resources/Players.png"));
                m_player_list->setIconSize(QSize(250, 130));
                m_player_list->move(200, 180);
                m_player_list->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0); color: black;"));
                m_player_list->setCursor(Qt::PointingHandCursor);

                m_player_list->show();
                m_scene->addWidget(m_player_list);

                //show player list when player list button is clicked
                connect(m_player_list, SIGNAL(clicked()), this, SLOT(showUsers()));
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText("Nom d'utilisateur ou mot de passe incorrect.");
                msgBox.exec();

                titleScreen();
            }
        }
    }
}

void Session::showScoreboard()
{
    //create sql relational table model
    QSqlRelationalTableModel * model;
    model = new QSqlRelationalTableModel;

    //set up query
    model->setTable("scores");
    model->setRelation(3, QSqlRelation("users", "id_user", "username")); //fourth column is a foreign key that maps with id_user from users table, show username field
    model->setSort(1, Qt::DescendingOrder); //order by score desc
    model->select();

    //set column names
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Score"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Joueur"));

    //create table
    QTableView * scoreboard = new QTableView;
    scoreboard->setModel(model);
    scoreboard->setWindowTitle("Scores");
    scoreboard->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //show and set size of table
    scoreboard->show();
    scoreboard->hideColumn(0); //hide id column
    scoreboard->resizeColumnsToContents();
    scoreboard->setFixedSize(300, 350);
}

void Session::showUsers()
{
    //create sql relational table model
    QSqlRelationalTableModel * model;
    model = new QSqlRelationalTableModel;

    //set up query
    model->setTable("users");
    model->setRelation(3, QSqlRelation("pets", "id_pet", "species")); //fourth column is a foreign key that maps with id_pet from pets table, show species field
    model->select();

    //set column names
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom d'utilisateur"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom du pixter"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Espèce du pixter"));

    //create table
    QTableView * users = new QTableView;
    users->setModel(model);
    users->setWindowTitle("Joueurs");
    users->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //show and set size of table
    users->show();
    users->verticalHeader()->hide();
    users->setSortingEnabled(true);
    users->resizeColumnsToContents();
    users->setFixedSize(400, 400);
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
