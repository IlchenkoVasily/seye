#include <QApplication>
#include "login.h"

#include "mainwindow.h"
#include "appengine.h"
#include "fakeconnector.h"
#include "enums.h"

int main(int argc, char *argv[])
{
//    login * auth = new login();

//       QObject::connect( auth, SIGNAL(emitComplited()),
//                         w, SLOT( exec()) );

//       auth->exec();

    QApplication a(argc, argv);

    qmlRegisterUncreatableMetaObject(seye::staticMetaObject, "seye", 1, 0,
                                     "States", "Error: only enums");

    seye::IConnector* conn = new seye::FakeConnector();

    seye::AppEngine engine;
    engine.setConnector(conn);
    engine.setUp();
    return a.exec();
}
