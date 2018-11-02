#include <QApplication>

#include "appengine.h"
#include "fakeconnector.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    seye::IConnector* conn = new seye::FakeConnector();

    seye::AppEngine engine;
    engine.setConnector(conn);
    engine.setUp();

    return a.exec();
}
