#include <QApplication>
#include "Session.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Session * game = new Session();

    game->show();
    return a.exec();
}
