#ifndef ADMIN_H
#define ADMIN_H

#include <QObject>

class Admin: public QObject
{
    Q_OBJECT //needs Q_OBJECT macro

    public:
        Admin();
        ~Admin();

        void menu();
};

#endif // ADMIN_H
