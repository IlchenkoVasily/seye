#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QtSql>

#include <QDebug>
#include <QDate>
#include <structs.h>

namespace seye
{
    class DBService
    {
    public:
        DBService(QString, QString, QString);
//        void isOpen();

        // успех - true и Success в дебаге, иначе fals и в дебаге причина неудачи
        // занесение данных в таблицу по одной строке
        bool add(passport);
        bool add(object, QString callSign);
        bool add(zone);
        bool add(access);
        bool add(group);

        // обновление данных после редактирования многих строк
        bool update(QVector<passport>);
        bool update(QList<object>);
        bool update(QVector<zone>);
        bool update(QVector<access>);
        bool update(QVector<group>);

        // запрос всех данных таблицы (для редактирования)
        QVector<passport> getAllPassports();
        QList<object> getAllObjects();
        QVector<access> getAllAccesses();
        QVector<group> getAllGroups();

        // запросы необходимого
        QVector<zone> getAllZones();
        passport getPassportForObject(QString objectId);
        QVector<access> getCurrentAccesses();
        object getObject(QString objectId);

        ~DBService();
    private:
        QSqlDatabase db;
    };
}
#endif // DBSERVICE_H
