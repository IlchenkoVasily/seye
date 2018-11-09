#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QtSql>

class DBService
{
public:
    DBService();
//тут пока ничего не менял
//прокатывает ли соединение пока не тестил

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "seyedb");
/*
 * Ругается на db
 * Александер сказал перенести в метод, шо логично, но пока туда не заглядывал
    db.setHostName("localhost");
    db.setDatabaseName("seye");
    db.setUserName("admin");
    db.setPassword("admin");
    bool ok = db.open();
*/

 //id есть, но он с автоинкрементом
    void addPassport(QString callSign, QString firstName, QString lastName, QDate date)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO passports (call_sign, first_name, last_name, birthday)"
                      "VALUES (:call_sign, :first_name, :last_name, :birthday)");
        query.bindValue(":call_sign", callSign);
        query.bindValue(":first_name", firstName);
        query.bindValue(":last_name", lastName);
        query.bindValue(":birthday", date);
//необходима обработка ошибок
        query.exec();
    }

    void addObject(QString idDevice, int idRole, QString callSign, int speedLimitMperS, int link)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO objects (id_device, id_character, call_sign, speed_limit_mpers, link)"
                      "VALUES (:id_device, :id_character, :call_sign, :speed_limit_mpers, :link)");
        query.bindValue(":id_device", idDevice);
        query.bindValue(":id_character", idRole);
        query.bindValue(":call_sign", callSign);
        query.bindValue(":speed_limit_mpers", speedLimitMperS);
        query.bindValue(":link", link);
        query.exec();
    }

};

#endif // DBSERVICE_H
