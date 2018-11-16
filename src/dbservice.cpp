#include "dbservice.h"

using namespace seye;

DBService::DBService(QString hostAddress, QString userName, QString userPassword)
{
    db = QSqlDatabase::addDatabase("QPSQL", "seyedb");
//    qDebug() << db.isValid();
    db.setHostName(hostAddress);
    db.setDatabaseName("seye");
    db.setUserName(userName);
    db.setPassword(userPassword);
    db.open();
}
DBService::~DBService()
{
    db.close();
}

/*
 * qDebug() << dblink.db.isOpen();
 * как-то это не красиво
 * и тело программы становится загруженым
 * и вообще db надо приватной сделать
 * потому определяю у себя
*/
//void DBService::isOpen()
//{
//    qDebug() << db.isOpen();
//}

bool DBService::add(passport passport)
{
    if (db.isOpen())
    {
        QSqlQuery query(db); /* вообще QSqlQuery сам проверяет открыта ли передаваемая бд
                               и если нет, то не создает переменную и говорит об этом,
                               но как на основе этого упростить проверки чот не представляю
                               потому что так и не понял что он возвращает при успехе
                             */
        query.prepare("INSERT INTO passports (call_sign, first_name, last_name, birthday)"
                      // id есть, но с автоинкрементом
                      "VALUES (:call_sign, :first_name, :last_name, :birthday)");
        query.bindValue(":call_sign", passport.callSign);
        query.bindValue(":first_name", passport.firstName);
        query.bindValue(":last_name", passport.lastName);
        query.bindValue(":birthday", passport.birthday);
        if (query.exec())
        {
            qDebug() << "Success";
            return(true);
            // по уму следует вызывать returninig ключа после запроса, что бы проверять как он прошел на базе
            // а то это проверки только на стороне клиента
            // но не разобрался как
        }
        else
            if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
            else qDebug() << db.lastError().text();
        return(false);
    }
    else
        if (db.open()) // дополнительно посылает логин с паролем
        {
            QSqlQuery query(db);
            query.prepare("INSERT INTO passports (call_sign, first_name, last_name, birthday)"
                          "VALUES (:call_sign, :first_name, :last_name, :birthday)");
            query.bindValue(":call_sign", passport.callSign);
            query.bindValue(":first_name", passport.firstName);
            query.bindValue(":last_name", passport.lastName);
            query.bindValue(":birthday", passport.birthday);
            if (query.exec())
            {
                qDebug() << "Success";
                return(true);
            }
            else
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
            return(false);
        }
        else qDebug() << db.lastError().text();
    return(false);
}

bool DBService::add(object object, QString callSign)
{
    if (db.isOpen())
    {
        QSqlQuery query(db);
        query.prepare("SELECT id FROM passports WHERE call_sign = (:call_sign)");
        query.bindValue(":call_sign", callSign);
        if (query.exec()) qDebug() << "Success";
        else
        {
            if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
            else qDebug() << db.lastError().text();
            return(false);
        }
        query.next();
        object.passport = query.value(0).toInt();
        query.prepare("INSERT INTO objects (id_device, id_character, id_passport, speed_limit_mpers, link)"
                      "VALUES (:id_device, :id_character, :id_passport, :speed_limit_mpers, :link)");
        query.bindValue(":id_device", object.id);
        query.bindValue(":id_character", object.role);
        query.bindValue(":id_passport", object.passport);
        query.bindValue(":speed_limit_mpers", object.speedLimit);
        query.bindValue(":link", object.link);
        if (query.exec())
        {
            qDebug() << "Success";
            return(true);
        }
        else
            if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
            else qDebug() << db.lastError().text();
        return(false);
    }
    else
        if (db.open()) // дополнительно посылает логин с паролем
        {
            QSqlQuery query(db);
            query.prepare("SELECT id FROM passports where call_sign = (:call_sign)");
            query.bindValue(":call_sign", callSign);
            if (query.exec()) qDebug() << "Success";
            else
            {
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
                return(false);
            }
            object.passport = query.value(0).toInt();
            query.prepare("INSERT INTO zones (id_device, id_character, id_passport, speed_limit_mreps, link)"
                          "VALUES (:id_device, :id_character, :id_passport, :speed_limit_mpers, :link)");
            query.bindValue(":id_device", object.id);
            query.bindValue(":id_character", object.role);
            query.bindValue("id_passport", object.passport);
            query.bindValue(":speed_limit_mpers", object.speedLimit);
            query.bindValue(":link", object.link);
            if (query.exec())
            {
                qDebug() << "Success";
                return(true);
            }
            else
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
            return(false);
        }
        else qDebug() << db.lastError().text();
    return(false);
}

bool DBService::add(zone zone)
{
    if (db.isOpen())
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO zones (polygon, zone_name, zone_color, line_color, line_width)"
                      // id есть, но с автоинкрементом
                      "VALUES (:polygon, :zone_name, :zone_color, :line_color, :line_width)");
        query.bindValue(":polygon", zone.polygon);
        query.bindValue(":zone_name", zone.name);
        query.bindValue(":zone_color", zone.color);
        query.bindValue(":line_color", zone.lineColor);
        query.bindValue(":line_width", zone.lineWidth);
        if (query.exec())
        {
            qDebug() << "Success";
            return(true);
        }
        else
            if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
            else qDebug() << db.lastError().text();
        return(false);
    }
    else
        if (db.open()) // дополнительно посылает логин с паролем
        {
            QSqlQuery query(db);
            query.prepare("INSERT INTO zones (polygon, zone_name, zone_color, line_color, line_width)"
                          "VALUES (:polygon, :zone_name, :zone_color, :line_color, :line_width)");
            query.bindValue(":polygon", zone.polygon);
            query.bindValue(":zone_name", zone.name);
            query.bindValue(":zone_color", zone.color);
            query.bindValue(":line_color", zone.lineColor);
            query.bindValue(":line_width", zone.lineWidth);
            if (query.exec())
            {
                qDebug() << "Success";
                return(true);
            }
            else
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
            return(false);
        }
        else qDebug() << db.lastError().text();
    return(false);
}

QVector<passport> DBService::getAllPassports()
{
    QVector<passport> data;
    if (db.isOpen())
    {
        passport doc;
        QSqlQuery query(db);
        if (query.exec("SELECT * FROM passports")) qDebug() << "Success";
        else
        {
            qDebug() << db.lastError().text();
            return(data);
        }
        while(query.next())
        {
            // можно было бы обойтись без магических числе,
            // но при изменении формата данных всё = придется лезть в функцию
            // потому усложнять её работу лучше не стоит
            // бывает и исчерпывающая автоматизация...
            doc.id = query.value(0).toInt();
            doc.callSign = query.value(1).toString();
            doc.firstName = query.value(2).toString();
            doc.lastName  = query.value(3).toString();
            doc.birthday = query.value(4).toDate();
            data.push_back(doc);
        }
        return(data);
    }
    else
        if (db.open()) // дополнительно посылает логин с паролем
        {
            passport doc;
            QSqlQuery query(db);
            if (query.exec("SELECT * FROM passports")) qDebug() << "Success";
            else
            {
                qDebug() << db.lastError().text();
                return(data);
            }
            while(query.next())
            {
                doc.id = query.value(0).toInt();
                doc.callSign = query.value(1).toString();
                doc.firstName = query.value(2).toString();
                doc.lastName  = query.value(3).toString();
                doc.birthday = query.value(4).toDate();
                data.push_back(doc);
            }
            return(data);
        }
        else qDebug() << db.lastError().text();
    return(data);
}

// реализовано крайне топорно, временная мера
// к тому же пока не известно сколько данных о редактируемых ячейках приходит с форм
bool DBService::update(QVector<passport> data)
{
    if (db.isOpen())
    {
        QSqlQuery query(db);
        if (query.exec("DELETE FROM passports")) qDebug() << "Success";
        else
        {
            if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
            else qDebug() << db.lastError().text();
            return(false);
        }
        for(int i = data.count(), j = 0; j < i; ++j) if (!add(data[j])) return(false);
        return(true);
        // возможно даже этого хватит, удалять стоит то, что не добавляли, но осталось
        // хотя в случае ошибки главное прекратить постоянные обновления с базы и восстановить там удаленные данные
    }
    else
        if (db.open()) // дополнительно посылает логин с паролем
        {
            QSqlQuery query(db);
            if (query.exec("DELETE FROM passports")) qDebug() << "Success";
            else
            {
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
                return(false);
            }
            for(int i = data.count(), j = 0; j < i; ++j) if (!add(data[j])) return(false);
            return(true);
        }
        else qDebug() << db.lastError().text();
    return(false);
}

bool DBService::update(QVector<zone> data)
{
    if (db.isOpen())
    {
        QSqlQuery query(db);
        if (query.exec("DELETE FROM zones")) qDebug() << "Success";
        else
        {
            if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
            else qDebug() << db.lastError().text();
            return(false);
        }
        for(int i = data.count(), j = 0; j < i; ++j) if (!add(data[j])) return(false);
        return(true);
    }
    else
        if (db.open()) // дополнительно посылает логин с паролем
        {
            QSqlQuery query(db);
            if (query.exec("DELETE FROM zones")) qDebug() << "Success";
            else
            {
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
                return(false);
            }
            for(int i = data.count(), j = 0; j < i; ++j) if (!add(data[j])) return(false);
            return(true);
        }
        else qDebug() << db.lastError().text();
    return(false);
}
