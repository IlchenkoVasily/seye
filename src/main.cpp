#include <QApplication>
#include "login.h"

#include "mainwindow.h"
#include "appengine.h"
#include "fakeconnector.h"
#include "enums.h"
#include "dbservice.h"

int main(int argc, char *argv[])
{
//    login * auth = new login();

//       QObject::connect( auth, SIGNAL(emitComplited()),
//                         w, SLOT( exec()) );

//       auth->exec();

    QApplication a(argc, argv);

    qmlRegisterUncreatableMetaObject(seye::staticMetaObject, "seye", 1, 0,
                                     "States", "Error: only enums");
    qmlRegisterUncreatableMetaObject(seye::staticMetaObject, "seye", 1, 0,
                                     "Roles", "Error: only enums");

    seye::AppEngine engine;

    seye::IConnector* conn = new seye::FakeConnector(&engine);

    engine.setConnector(conn);
    engine.setUp();
    return a.exec();

//    QString host = "31.211.74.221";
//    QString login = "pradlol";
//    QString password = "g1e6111213";
//    seye::DBService dblink(host, login, password);
////    seye::passport passport;
////    passport.callSign = "";
////    passport.firstName = "";
////    passport.lastName = "тот еще";
////    dblink.add(passport);
////    dblink.isOpen();
////    QVector<seye::passport> passports = dblink.getAllPassports();
////    for(int i = 0; i < passports.size(); ++i)
////        qDebug() << seye::toString(passports[i]) << endl;
////    seye::object object;
////    object.id = "9999999999999990";
////    dblink.add(object, "");
//    seye::passport p;
//    p.toString();
//    dblink.add(p);
}
