#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QtSql>

#include <structs.h>

namespace seye
{
    class DBService
    {
    public:
        DBService(QString, QString, QString);

        // успех - Success в дебаге (bool true), иначе причина неудачи (bool false)

        // занесение данных в таблицу (по одной строке)
        bool add(passport);
        // паспорт можно послать без device и дополнить его позже reference-ом
        // отправление полного паспорта просто создаст новый, но с девайсом
        bool add(object);
        bool add(zone);
        bool add(access);
        bool add(group);
        bool add(QString groupName); // создаст запись с именем в таблице групп

        // присоединение листа девайсов к группе
        bool reference(group); // подразумевается, что лист объектов группы, если он идет отдельно, предварительно заносится в group.devices
        // происходит select id по имени, которое носит не уникальный характер
        // на самом деле это не проблема, но, не смотря на возможность таких ситуаций, они не обрабатываются
        // изначально функция reference носила более общий характер и относилась еще и к паспорту
        // но такая надобность отпала

        // запрос id строки
        // не стоит ориентироваться на него, как на индекс QList, после первого же удаления они будут различаться
        // внутри функций отсутствует isOpen т.к. эти функции всегда используются в совокупности с другими и эти проверки избыточны
        qint64 getId(passport);
//        QString getId(object);
        // id объекта == id девайса и вносится нами, к тому же все остальные данные не однозначны
        qint32 getId(zone);
        qint64 getId(access);
        qint64 getId(group); // происходит select id по имени, которое носит не уникальный характер
        // на самом деле это не проблема, но, не смотря на возможность таких ситуаций, они не обрабатываются

        // обновление данных после редактирования таблицей
        // удаление идет отдельной фукнцией
        bool update(QList<passport>);
        bool update(QList<object>);
        bool update(QList<zone>);
        bool update(QList<access>);
        bool update(QList<group>);

        // удаление строки
        // по 3 варианта, внутри одинаковые
        bool drop(passport);
        bool dropPassport(passport);
        bool dropPassport(qint64 idPassport);
        bool drop(object);
        bool dropObject(object);
        bool dropObject(QString idDevice);
        bool drop(zone);
        bool dropZone(zone);
        bool dropZone(qint32 idZone);
        bool drop(access);
        bool dropAccess(access);
        bool dropAccess(qint64 idAccess);
        bool drop(group);
        bool dropGroup(group);
        bool dropGroup(qint64 idGroup);

        // запрос всей таблицы
        QList<passport> getAllPassports();
        QList<object> getAllObjects();
        QList<zone> getAllZones();
        QList<access> getAllAccesses();
        QList<group> getAllGroups();

        // ситуативные запросы
        // и крайне информативные коментарии к ним:
        QList<access> getCurrentAccesses(); // запрашивает правила на час, фильтрованный вариант getAllAccesses()
        QList<access> getAccessesFor(zone); // правила одной зоны
        QList<access> getAccessesFor(group); // правила одной группы
        QList<group> getCurrentGroups(QList<object>); // групп может быть много и на все случаи жизни
        // а значит имеет смысл запрашивать только для текущих объектов
        QList<group> getCurrentGroups(QList<QString> idDevices);
        QList<group> getGroupsFor(zone); // группы зоны
        QList<group> getGroupsFor(object); // группы объекта
        QList<zone> getZonesFor(group); // зоны группы :D
        QList<object> getObjectsFor(group); // объекты группы
        passport getPassportFor(object); // паспорт объекта
        passport getPassportFor(QString idDevice);
        object getObjectFor(QString idDevice); // объект по idDevice
        object getObjectFor(passport); // объект паспорта
        /* Получается, когда прилетает idDevice, всю возможную инфу о нём выдаст следующий набор:
         QList<object> objects;
         QList<passport> passports;
         QList<QList<access>> accesses;
         QList<QList<zone>> zones;
         int i = 0;
         objects[i] = dblink.getObjectFor(idDevice);
         passports[i] = dblink.getPassportFor(objects[i]);
         QList<group> groups = dblink.getGroupsFor(objects[i]);
         for(int j = 0; j < groups.size(); ++j) accesses[j] = dblink.getAccessesFor(groups[j]);
         for(int j = 0; j < groups.size(); ++j) zones[j] = dblink.getZonesFor(groups[j]);
         ++i;
         * Но эта информация избыточна, если нам нужно только присвоить статус объекту.
         * Достаточно будет этого:
         bool checkAccessFor(QString idDevice);
         * Кстати весьма быстрый вариант. Был бы.
         * Со стандартным плагином работать не будет
         * т.к. нужно распарсивать на нашей стороне QString в полигон
         * ибо PSQL не знает типа polygon...
         * Но дополнить плагин очень даже можно.
         * Вариант для постоянных проверок:
         QList<QString> getCurrentAccessPolygonsFor(QString idDevice);
         * Но правила весьма динамичны, а значит надо будет:
         * либо выполнять эту функцию хотя бы каждую минуту для каждого текущего объекта
         * что совершенно не выгодно
         * либо периодично выполнять getCurrentGroups(), getAllZones() и getCurrentAccesses()
         * и поиск необходимого на нашей стороне */

        // управление пользователем
        QString getRole(QString login) {return "operator";} // в нашем случае токен функционала
        bool creatNewUser(QString user, QString password, QString role);
        bool dropUser(QString user);
        QList<QString> getListOfUsers(); // Формат User, Role

        ~DBService();
    private:
        QSqlDatabase db;

        bool insert(passport passport) const
        {
            QSqlQuery query(db);
            if(passport.device == nullptr) query.prepare("INSERT INTO passports (first_name, last_name, call_sign, birthday)"
                                                        // id есть, но с автоинкрементом
                                                        "VALUES (:first_name, :last_name, :call_sign, :birthday)");
            else
            {
                query.prepare("INSERT INTO passports (first_name, last_name, call_sign, birthday, id_object)"
                              "VALUES (:first_name, :last_name, :call_sign, :birthday, :id_object)");
                query.bindValue(":id_object", passport.device);
            }
            query.bindValue(":first_name", passport.firstName);
            query.bindValue(":last_name", passport.lastName);
            query.bindValue(":call_sign", passport.callSign);
            query.bindValue(":birthday", passport.birthday);
            if (query.exec())
            {
                qDebug() << "Insert passport success";
                return true;
                // по уму следует вызывать returninig ключа после запроса, что бы проверять как он прошел на базе
                // а то это проверки только на стороне клиента
                // но пока и так пойдет (:
            }
            else
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
//            qDebug() << query.isValid();
//            qDebug()<< query.lastError();
            return false;
        }

        bool insert(object object) const
        {
            QSqlQuery query(db);
            query.prepare("INSERT INTO objects (id_device, id_character, speed_limit_mpers, link)"
                          "VALUES (:id_device, :id_character, :speed_limit_mpers, :link)");
            query.bindValue(":id_device", object.id);
            query.bindValue(":id_character", object.role);
            query.bindValue(":speed_limit_mpers", object.speedLimit);
            query.bindValue(":link", object.link);
            if (query.exec())
            {
                qDebug() << "Insert object success";
                return true;
            }
            else
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
            return false;
        }

        bool insert(zone zone) const
        {
            QSqlQuery query(db);
            query.prepare("INSERT INTO zones (zone_name, polygon, zone_color, line_color)"
                          // id есть, но с автоинкрементом
                          "VALUES (:zone_name, :polygon, :zone_color, :line_color)");
            query.bindValue(":zone_name", zone.name);
            query.bindValue(":polygon", zone.polygon);
            query.bindValue(":zone_color", zone.color);
            query.bindValue(":line_color", zone.lineColor);
            if (query.exec())
            {
                qDebug() << "Insert zone success";
                return true;
            }
            else
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
            return false;
        }

        bool insert(access access) const
        {
            QSqlQuery query(db);
            query.prepare("INSERT INTO access_rules (start, end, priority, rule_name, id_group, id_zone)"
                          // id есть, но с автоинкрементом
                          "VALUES (:start, :end, :priority, :rule_name, :id_group, :id_zone)");
            query.bindValue(":start", access.start);
            query.bindValue(":end", access.end);
            query.bindValue(":priority", access.priority);
            query.bindValue(":rule_name", access.name);
            query.bindValue(":id_group", access.group);
            query.bindValue(":id_zone", access.zone);
            if (query.exec())
            {
                qDebug() << "Insert access success";
                return true;
            }
            else
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
            return false;
        }

        bool insert(QString groupName)
        {
            QSqlQuery query(db);
            query.prepare("INSERT INTO task_groups (group_name)"
                          // id есть, но с автоинкрементом
                          "VALUES (:group_name)");
            query.bindValue(":group_name", groupName);
            if (query.exec())
            {
                qDebug() << "Insert group success";
                return true;
            }
            else
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
            return false;
        }

        bool insert(qint64 idGroup, QList<QString> idDevices) const
        {
            for(int i = 0; i < idDevices.size(); ++i)
            {
                QSqlQuery query(db); // есть clear, но так надежней
                query.prepare("INSERT INTO group_object (id_group, id_object)"
                              "VALUES (:id_group, :id_object)");
                query.bindValue(":id_group", idGroup);
                query.bindValue(":id_object", idDevices[i]);
                if (query.exec()) qDebug() << "Insert group-object " << i << " success";
                else
                {
                    if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                    else qDebug() << db.lastError().text();
                    return false;
                }
            }
            return true;
        }

        qint64 selectPassportId(QString callSign) const
        {
            QSqlQuery query(db);
            query.prepare("SELECT id FROM passports WHERE call_sign = (:call_sign)");
            query.bindValue(":call_sign", callSign);
            if (query.exec()) qDebug() << "Select passport id success";
            else
            {
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
                return 0;
            }
            query.next();
            return query.value(0).toInt();
        }

        qint32 selectZoneId(QString polygon) const
        {
            QSqlQuery query(db);
            query.prepare("SELECT id FROM zones WHERE polygon = (:polygon)");
            query.bindValue(":polygon", polygon);
            if (query.exec()) qDebug() << "Select zone id success";
            else
            {
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
                return 0;
            }
            query.next();
            return query.value(0).toInt();
        }

        qint64 selectAccessId(access access) const
        {
            QSqlQuery query(db);
            query.prepare("SELECT id FROM access_rules"
                          "WHERE start = (:start), end = (:end), id_group = (:id_group), id_zone = (:id_zone)");
            query.bindValue(":start", access.start);
            query.bindValue(":end", access.end);
            query.bindValue(":id_group", access.group);
            query.bindValue(":id_zone", access.zone);
            if (query.exec()) qDebug() << "Select access id success";
            else
            {
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
                return 0;
            }
            query.next();
            return query.value(0).toInt();
        }

        qint64 selectGroupId(QString groupName) const
        {
            QSqlQuery query(db);
            query.prepare("SELECT id FROM task_groups WHERE group_name = (:group_name)");
            query.bindValue(":group_name", groupName);
            if (query.exec()) qDebug() << "Select group id success";
            else
            {
                if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                else qDebug() << db.lastError().text();
                return 0;
            }
            query.next();
            return query.value(0).toInt();
        }

        bool UPDATE(QList<passport> passports) // изменить название функции и вынести цикл из тела
        {
            for(int i = 0; i < passports.size(); ++i)
            {
                QSqlQuery query(db); // есть clear, но так надежней
                if (passports[i].device == nullptr) query.prepare("UPDATE passports SET"
                                                                  "first_name = (:first_name),"
                                                                  "last_name = (:last_name),"
                                                                  "call_sign = (:call_sign),"
                                                                  "birthday = (:birthday)"
                                                                  "WHERE id = (:id)");
                else
                {
                    query.prepare("UPDATE passports SET"
                                  "first_name = (:first_name), last_name = (:last_name),"
                                  "call_sign = (:call_sign), birthday = (:birthday),"
                                  "id_object = (:id_object)"
                                  "WHERE id = (:id)");
                    query.bindValue(":id_object", passports[i].device);
                }
                query.bindValue(":first_name", passports[i].firstName);
                query.bindValue(":last_name", passports[i].lastName);
                query.bindValue(":call_sign", passports[i].callSign);
                query.bindValue(":birthday", passports[i].birthday);
                if(passports[i].id) query.bindValue(":id", passports[i].id);
                else
                {
                    passports[i].id = selectPassportId(passports[i].callSign);
                    if(passports[i].id) query.bindValue(":id", passports[i].id);
                    else return false;
                }
                query.bindValue(":id", passports[i].id);
                if (query.exec()) qDebug() << "Update passport " << i << " success";
                else
                {
                    if (db.lastError().text() == " ") qDebug() << "Не все графы заполнены верно";
                    else qDebug() << db.lastError().text();
                    return false;
                }
            }
            return true;
        }
    };
}
#endif // DBSERVICE_H
