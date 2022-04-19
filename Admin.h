#ifndef ADMIN_H
#define ADMIN_H

#include <QGraphicsView>
#include <QWidget>

class Admin: public QGraphicsView
{
    Q_OBJECT //needs Q_OBJECT macro

    private:
        QString m_password;

    public:
        Admin();
        ~Admin();

        void menu();
};

#endif // ADMIN_H
