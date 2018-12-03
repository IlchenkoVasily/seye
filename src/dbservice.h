#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QtSql>

#include <structs.h>
#include <polygon.h>

namespace seye
{
    class DBService
    {
    public:
        DBService(const QString host, const QString login, const QString password);

        // успех запроса - Success в дебаге (bool true), иначе - причина неудачи (bool false)
        // следует добавить сигнал с причиной ошибки

        // занесение данных в таблицу (по одной строке), возвращает id записи
        // не стоит ориентироваться на него, как на индекс QList, после первого же drop они будут различаться
        qint64 add(const Passport&); // можно послать без device
        bool add(const ObjectDev&); // айди вбиваем самостоятельно, возвращать нечего
        qint32 add(const Zone&);
//        bool add(Polygon&);
        qint64 add(const Access&);
        qint64 add(Group&);
        qint64 add(const QString& groupName); // создаст запись с именем в таблице групп
        qint32 add(const User&, const QString& password);

        // присоединение листа девайсов к группе
        bool reference(const Group&); // подразумевается, что лист объектов группы, если он идет отдельно, предварительно заносится в group.devices
        // функция нормально отработает даже если в листе девайсов есть уже ранее присоедененные, но сообщит об этом

        // удаление строки
        // ранее было по 3 варианта функции т.к. подаётся целая структура, когда нужен только id
        // например: bool drop(passport); bool dropPassport(passport); bool dropPassport(qint64 idPassport);
        // но это создаёт больше путаницы, чем помогает
        bool dropReference(const qint64& idGroup, const QString& idDevice);
        bool drop(const Passport&);
        bool drop(const ObjectDev&);
        bool drop(const Zone&);
//        bool drop(Polygon&);
        bool drop(const Access&);
        bool drop(const Group&);
        bool drop(const User&);

        // обновление данных после редактирования таблицей
        // удаление и добавления здесь не отслеживаются
        bool update(const QList<Passport>&);
        bool update(const QList<ObjectDev>&);
        bool update(const QList<Zone>&);
//        bool update(QList<Polygon>&);
        bool update(const QList<Access>&);
        bool update(const QList<Group>&);

        // запрос всей таблицы
        QList<Passport> getAllPassports();
        QList<ObjectDev> getAllObjects();
        QList<Zone> getAllZones();
//        QList<Polygon> getAllZones();
        QList<Access> getAllAccesses();
        QList<Group> getAllGroups();
        QList<User> getAllUsers();

        // управление пользователем
        QString getRole(const QString& userName); // в нашем случае токен функционала
        bool changePassword(const QString& userName, const QString& password);

        // ситуативные запросы
        QString getCallSignFor(const QString& idDevice);
        Passport getPassportFor(const ObjectDev&);
        Passport getPassportFor(const QString& idDevice);
        QList<AccessLine> getAllAccessesForTimeline();

        ~DBService();
    private:
        QSqlDatabase db;

        bool open();
        bool whatIsError() const; // Выводит ошибку, всегда false
        // хотя в таком случае следует сделать void

        qint64 insert(const Passport&) const;
        bool insert(const ObjectDev&) const;
        qint32 insert(const Zone&) const;
//        bool insert(Polygon&) const;
        qint64 insert(const Access&) const;
        qint64 insert(const QString& groupName) const;

        bool insert(const qint64& idGroup, const QString& idDevice) const;

        bool create(const QString& userName, const QString& password) const;
        qint32 selectUserId(const QString& userName) const;

        QList<Passport> selectAllPassports() const;
        QList<ObjectDev> selectAllObjects() const;
        QList<Zone> selectAllZones() const;
//        QList<Polygon> selectAllZones() const;
        QList<Access> selectAllAccesses() const;
        QList<Group> selectAllGroups() const;
        Group selectAllReferences(Group&) const;
        QList<User> selectAllUsers() const;
        QString selectRole(const qint32& idUser) const; // вообще можно сначала роль с айдишниками членства запрашивать, а потом по oid юзеров,
        // но хрен редьки не слаще
        QString selectSignFor(const QString& idDevice) const;
        Passport selectPassportFor(const QString& idDevice) const;

        // неочевидное поведение, возвращает 0 при ошибке, 1 если удалось найти и 2 если нет
        qint16 select(const qint64& idGroup, const QString& idDevice) const;
        qint16 selectGroup(const QString& groupName) const;
        qint16 selectSign(const QString& callSign) const;
        qint16 selectObjectId(const QString& idDevice) const;
        // вообще следовало бы функции назвать не по запросу, а по типу действия (isExist)
        // но приват я организовал для себя, снаружи эти функции всё = не вызвать
        // а для заботы о тех, кто в дальнейшем будет заниматься моим кодом, мне слишком мало платят

        qint16 selectUser(const QString& userName) const;
        bool grant(const User&) const;
        bool alter(const QString& userName) const; // если разобраться с наследованием на стороне бд, то надобность в команде отпадёт
        bool alter(const QString& userName, const QString& password) const;
        bool DROP(const QString& userName) const;

        // связыне элементы удаляются без дополнительной команды т.к. активирован каскад на уровне бд
        bool deleteReference(const qint64& idGroup, const QString& idDevice) const;
        bool deletePassport(const qint64& idPassport) const;
        bool deleteObject(const QString& idDevice) const;
        bool deleteZone(const qint32& idZone) const;
        bool deleteAccess(const qint64& idAccess) const;
        bool deleteGroup(const qint64& idGroup) const;

        bool upDate(const Passport&) const;
        bool upDate(const ObjectDev&) const;
        bool upDate(const Zone&) const;
//        bool upDate(Polygon&) const;
        bool upDate(const Access&) const;
        bool upDate(const Group&) const;
    };
}
#endif // DBSERVICE_H
