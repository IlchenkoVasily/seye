#include <QApplication>

#include "appengine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    seye::AppEngine engine;
    engine.setUp();

    return a.exec();
}
