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

/* вообще QSqlQuery сам проверяет открыта ли передаваемая бд
 * и если нет, то не создает переменную и говорит об этом,
 * но как на основе этого упростить проверки чот не представляю
 * потому что так и не понял что он возвращает при успехе
 * поэтому приходится вставлять запрос в тело, а не составлять его предварительно
*/

bool DBService::add(passport passport)
{
    if (db.isOpen()) return DBService::insert(passport);
    else  // дополнительно посылает логин с паролем
        if (db.open()) return DBService::insert(passport);
        else qDebug() << db.lastError().text();
    return false;
}

bool DBService::add(object object)
{
    if (db.isOpen()) return DBService::insert(object);
    else // дополнительно посылает логин с паролем
        if (db.open()) return DBService::insert(object);
        else qDebug() << db.lastError().text();
    return false;
}

bool DBService::add(zone zone)
{
    if (db.isOpen()) return DBService::insert(zone);
    else // дополнительно посылает логин с паролем
        if (db.open()) return DBService::insert(zone);
        else qDebug() << db.lastError().text();
    return false;
}

bool DBService::add(access access)
{
    if (db.isOpen()) return DBService::insert(access);
    else  // дополнительно посылает логин с паролем
        if (db.open()) return DBService::insert(access);
        else qDebug() << db.lastError().text();
    return false;
}

bool DBService::add(group group)
{
    if (db.isOpen())
    {
        if (!DBService::insert(group.name)) return false;
        if (!DBService::reference(group)) return false;
        return true;
    }
    else
        if (db.open()) // дополнительно посылает логин с паролем
        {
            if (!DBService::insert(group.name)) return false;
            if (!DBService::reference(group)) return false;
            return true;
        }
        else qDebug() << db.lastError().text();
    return false;
}

bool DBService::add(QString groupName)
{
    if (db.isOpen()) return DBService::insert(groupName);
    else // дополнительно посылает логин с паролем
        if (db.open()) return DBService::insert(groupName);
        else qDebug() << db.lastError().text();
    return false;
}

bool DBService::reference(group group)
{
    if (db.isOpen())
    {
        if(group.id) return DBService::insert(group.id, group.devices);
        else
        {
            group.id = selectGroupId(group.name);  //вернет 0 при неудаче
            if(group.id) return DBService::insert(group.id, group.devices);
            else return false;
        }
    }
    else
        if (db.open()) // дополнительно посылает логин с паролем
        {
            if(group.id) return DBService::insert(group.id, group.devices);
            else
            {
                group.id = selectGroupId(group.name);
                if(group.id) return DBService::insert(group.id, group.devices);
                else return false;
            }
        }
        else qDebug() << db.lastError().text();
    return false;
}

QList<passport> DBService::getAllPassports()
{
    QList<passport> data;
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

/*
bool DBService::update(QList<passport> passports)
{
    if (db.isOpen())
    {
        if(group.id) return DBService::insert(group.id, group.devices);
        else
        {
            group.id = selectGroupId(group.name);  //вернет 0 при неудаче
            if(group.id) return DBService::insert(group.id, group.devices);
            else return false;
        }
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
*/

bool DBService::update(QList<zone> data)
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
