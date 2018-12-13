#include "dbservice.h"

using namespace seye;

DBService::DBService(const QString hostAddress, const QString userName, const QString userPassword)
{
    db = QSqlDatabase::addDatabase("QPSQL", "seyedb");
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

/* Вообще QSqlQuery сам проверяет открыта ли передаваемая бд
 * и если нет, то не создает переменную и говорит об этом,
 * но как на основе этого упростить проверки чот не представляю
 * потому что так и не понял что он возвращает при успехе
 * поэтому приходится везде вставлять open()
 *
 * Магия класса:
 * - query.value везде содержит фиксированные числа,
 * иногда это обусловлено тем, что запрашивается только один столбик и такое магией не назвать,
 * но в остальных ситуациях это жесткая завязка на соответствие с бд для корректного парсинга,
 * так что разбираться с этой магией придётся только в случае радикальных изменений на стороне бд;
 * - проверки существования строк возвращают 0, 1 или 2,
 * как иначе реализовать полноценную обработку ошибок - не знаю; (enum? сути не меняет)
 * - проверка ролей юзера происходит по такому же принципу (admin, supervisor, operator);
 * - вообще вся работа с юзером ориентирована на базу и там сплошная магия...
 *
 * Костыль:
 * bindValue подставляет значения в ковычках и экранирует специальные символы.
 * При работе с пользователем ковычки лишние и потому приходится составлять строку запроса самостоятельно.
 * Всё бы ничего, но пока там отсутсвует обработка символов, нуждающихся в экранировании,
 * потому user.name и password должны быть без таких символов.
 *
 * Контракт:
 * - После добавления записи возвращается её id, который необходимо присвоить соответствующему значению;
 * - Т.к. правило добавляется для зоны и группы, они обязаны существовать;
 * - Т.к. группа формируется из объектов, они обязаны существовать;
 * - Т.к. объект присоеденяется к паспорту, он обязан существовать;
 * - Проверки на соответствие длинны заполняемых граф проводится на этапе заполнения
 * (тут еще такой момент: база-то скушает любую длинну т.к. сама подгонит размеры,
 * но тогда хранимое значение будет отличаться от нашего, что повлечет конфликты).
 * Ориентирование на соблюдение этих условий помогает сократить количество проверок и делает код читаемей.
*/

qint64 DBService::add(const Passport& passport)
{
    if (open())
        if (selectSign(passport.callSign) == 2) return insert(passport);
    return 0;
}

bool DBService::add(const ObjectDev& object)
{
    if (open())
        if (selectObjectId(object.id) == 2) return insert(object);
    return false;
}

bool DBService::add(Polygon* zone)
{
    if (open()) return insert(zone);
    return false;
}

qint64 DBService::add(const Access& access)
{
    if (open()) return insert(access);
    return 0;
}

qint64 DBService::add(Group& group)
{
    if (open())
        group.id = add(group.name);
        if (reference(group)) return group.id; // у нас тут ссылка на ссылку - это нормально?
    return 0;
}

qint64 DBService::add(const QString& groupName)
{
    if (open())
        if (selectGroup(groupName) == 2) return insert(groupName);
    return 0;
}

qint32 DBService::add(const User& user, const QString& password)
{
    if (open())
        if (selectUser(user.name) == 2)
            if (create(user.name, password))
                if (grant(user))
                    if (user.role == "operator") return selectUserId(user.name);
                    else
                        if (alter(user.name)) return selectUserId(user.name);
    return 0;
}

bool DBService::reference(const Group& group)
{
    if (open())
    {
        qint16 go;
        for(int i = 0; i < group.devices.size(); ++i)
            if (go = select(group.id, group.devices[i]) == 2)
                if (insert(group.id, group.devices[i])) qDebug() << "Device " << group.devices[i];
                else qDebug() << "Девайс " << group.devices[i] << " присоеденить не удалось, но мы не сдаёмся";
            else
                if (!go) return false;
        return true;
    }
    return false;
}

bool DBService::dropReference(const qint64& idGroup, const QString& idDevice)
{
    if (open()) return deleteReference(idGroup, idDevice);
    return false;
}

bool DBService::drop(const Passport& passport)
{
    if (open()) return deletePassport(passport.id);
    return false;
}

bool DBService::drop(const ObjectDev& object)
{
    if (open()) return deleteObject(object.id);
    return false;
}

bool DBService::drop(Polygon* zone)
{
    if (open()) return deleteZone(zone->id());
    return false;
}

bool DBService::drop(const Access& access)
{
    if (open()) return deleteAccess(access.id);
    return false;
}

bool DBService::drop(const Group& group)
{
    if (open()) return deleteGroup(group.id);
    return false;
}

bool DBService::drop(const User& user)
{
    if (open()) return DROP(user.name);
    return false;
}

bool DBService::update(const QList<Passport>& passports)
{
    if (open())
    {
        for(int i = 0; i < passports.size(); ++i)
            if (upDate(passports[i])) qDebug() << i;
            else return false;
        return true;
    }
    return false;
}

bool DBService::update(const QList<ObjectDev>& objects)
{
    if (open())
    {
        for(int i = 0; i < objects.size(); ++i)
            if (upDate(objects[i])) qDebug() << i;
            else return false;
        return true;
    }
    return false;
}

bool DBService::update(QList<Polygon*>& zones)
{
    if (open())
    {
        for(int i = 0; i < zones.size(); ++i)
            if (upDate(zones[i])) qDebug() << i;
            else return false;
        return true;
    }
    return false;
}

bool DBService::update(const QList<Access>& accesses)
{
    if (open())
    {
        for(int i = 0; i < accesses.size(); ++i)
            if (upDate(accesses[i])) qDebug() << i;
            else return false;
        return true;
    }
    return false;
}

bool DBService::update(const QList<Group>& groups)
{
    if (open())
    {
        for(int i = 0; i < groups.size(); ++i)
            if (upDate(groups[i]))
                if (reference(groups[i])) qDebug() << i; // reference имеет смысл?
                else return false;
            else return false;
        return true;
    }
    return false;
}

QList<Passport> DBService::getAllPassports()
{
    if (open()) return selectAllPassports();
    QList<Passport> passports;
    return passports;
}

QList<ObjectDev> DBService::getAllObjects()
{
    if (open()) return selectAllObjects();
    QList<ObjectDev> objects;
    return objects;
}

QList<Polygon*> DBService::getAllZones()
{
    if (open()) return selectAllZones();
    QList<Polygon*> zones;
    return zones;
}

QList<Access> DBService::getAllAccesses()
{
    if (open()) return selectAllAccesses();
    QList<Access> accesses;
    return accesses;
}

QList<Group> DBService::getAllGroups()
{
    QList<Group> groups;
    if (open())
    {
        groups = selectAllGroups();
        for(int i = 0; i < groups.size(); ++i)
            groups[i] = selectAllReferences(groups[i]);
    }
    return groups;
}

QList<User> DBService::getAllUsers()
{
    QList<User> users;
    if (open())
    {
        users = selectAllUsers();
        for(int i = 0; i < users.size(); ++i)
            users[i].role = selectRole(users[i].id);
    }
    return users;
}

QString DBService::getRole(const QString& userName)
{
    if (open())
        if (qint32 id = selectUserId(userName)) return selectRole(id);
    return nullptr;
}

bool DBService::changePassword(const QString& userName, const QString& password)
{
    if (open()) return alter(userName, password);
    return false;
}

QString DBService::getCallSignFor(const QString& idDevice)
{
    if (open()) return selectSignFor(idDevice);
    return nullptr;
}

Passport DBService::getPassportFor(const ObjectDev& object)
{
    if (open()) return selectPassportFor(object.id);
    Passport passport;
    return passport;
}

Passport DBService::getPassportFor(const QString& idDevice)
{
    if (open()) return selectPassportFor(idDevice);
    Passport passport;
    return passport;
}

QString DBService::getZoneName(const qint32& idZone)
{
    if (open()) return selectZoneName(idZone);
    return nullptr;
}

QString DBService::getGroupName(const qint64& idGroup)
{
    if (open()) return selectGroupName(idGroup);
    return nullptr;
}

QList<qint64> DBService::getGroupsIdForObject(const QString& idDevice)
{
    if (open()) return selectGroupsIdForObject(idDevice);
    QList<qint64> fail;
    return fail;
}

QList<AccessLine> DBService::getAllAccessesForTimeline()
{
    QList<AccessLine> line;
    if (open())
    {
        QList<Access> accesses = selectAllAccesses();
        AccessLine rule;
        QDateTime timeNow = QDateTime::currentDateTime();
        QDateTime timeInc = QDateTime::currentDateTime().addSecs(3600);
        for(int i = 0; i < accesses.size(); ++i)
            if (accesses[i].priority != "Неизменяемое")
            {
                if (accesses[i].end < QDateTime::currentDateTime())
                    if (deleteAccess(accesses[i].id)) qDebug() << "Уже закончилось"; else return line;
                else
                {
                    if (accesses[i].start < timeInc) // ? повторяшки и проработать стыки
                    {
                        rule.change = accesses[i].start;
                        rule.name = "Начало \"" + accesses[i].name + "\"";
                        if (!select(rule, accesses[i].zone)) return line;
                        rule.groupName = selectGroupName(accesses[i].group);
                        line.push_back(rule); // ? в хрен пойми каком порядке и с повторами времени, но в пределах часа
                    }
                    if (accesses[i].end < timeInc)
                    {
                        rule.change = accesses[i].end;
                        rule.name = "\"" + accesses[i].name + "\" подходит к концу";
                        if (!select(rule, accesses[i].zone)) return line;
                        rule.groupName = selectGroupName(accesses[i].group);
                        line.push_back(rule); // ? в хрен пойми каком порядке и с повторами времени, но в пределах часа
                    }
                }
            }
    }
    return line;
}

//----------------------------------------------------------------------
//------------------------ private функции -----------------------------
//----------------------------------------------------------------------

bool DBService::open()
{
    if (db.isOpen()) return true;
    if (db.open()) return true;
    return whatIsError();
}

bool DBService::whatIsError() const
{
    if (db.lastError().text() == " ") qDebug() << "Запрос составлен неверно"; // db.lastError().databaseText() Вообще ничего не пишет
    else qDebug() << db.lastError().text();
    return false;
}

qint64 DBService::insert(const Passport& passport) const
{
    QSqlQuery query(db);
    if (passport.device == nullptr) query.prepare("INSERT INTO passports (first_name, last_name, call_sign, birthday) "
                                                  "VALUES (:first_name, :last_name, :call_sign, :birthday) "
                                                  "RETURNING id");
    else
        if (selectObjectId(passport.device) == 1) // здесь проверку оставил т.к. пока что форма добавления недостаточно навороченная
        {
            query.prepare("INSERT INTO passports (first_name, last_name, call_sign, birthday, id_object) "
                          "VALUES (:first_name, :last_name, :call_sign, :birthday, :id_object) "
                          "RETURNING id");
            query.bindValue(":id_object", passport.device);
        }
        else return false;
    query.bindValue(":first_name", passport.firstName);
    query.bindValue(":last_name", passport.lastName);
    query.bindValue(":call_sign", passport.callSign);
    query.bindValue(":birthday", passport.birthday);
    if (query.exec())
    {
        qDebug() << "Insert passport success";
        if (query.next()) return query.value(0).toInt();
        else qDebug() << "Запись не создалась О_о";
    }
    return whatIsError();
}
// много логики, но вынос её наружу делает функцию совсем нечитаемой

bool DBService::insert(const ObjectDev& object) const
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO objects (id_device, id_character, speed_limit_mpers, link) "
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
    return whatIsError();
}

bool DBService::insert(Polygon* zone) const
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO zones (zone_name, polygon, zone_color, line_color) "
                  "VALUES (:zone_name, :polygon, :zone_color, :line_color) "
                  "RETURNING id");
    query.bindValue(":zone_name", zone->name());
    query.bindValue(":polygon", zone->toString());
    query.bindValue(":zone_color", zone->color().name(QColor::HexArgb));
    query.bindValue(":line_color", zone->borderColor().name(QColor::HexRgb));
    if (query.exec())
    {
        qDebug() << "Insert zone success";
        if (query.next())
        {
            zone->setId(query.value(0).toInt());
            return true;
        }
        else qDebug() << "Запись не создалась О_о";
    }
    return whatIsError();
}

qint64 DBService::insert(const Access& access) const
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO access_rules (start, \"end\", priority, rule_name, id_group, id_zone) "
                  "VALUES (:start, :end, :priority, :rule_name, :id_group, :id_zone) "
                  "RETURNING id");
    query.bindValue(":start", access.start);
    query.bindValue(":end", access.end);
    query.bindValue(":priority", access.priority);
    query.bindValue(":rule_name", access.name);
    query.bindValue(":id_group", access.group);
    query.bindValue(":id_zone", access.zone);
    if (query.exec())
    {
        qDebug() << "Insert access success";
        if (query.next()) return query.value(0).toInt();
        else qDebug() << "Запись не создалась О_о";
    }
    return whatIsError();
}

qint64 DBService::insert(const QString& groupName) const
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO task_groups (group_name) "
                  "VALUES (:group_name) "
                  "RETURNING id");
    query.bindValue(":group_name", groupName);
    if (query.exec())
    {
        qDebug() << "Insert group success";
        if (query.next()) return query.value(0).toInt();
        else qDebug() << "Запись не создалась О_о";
    }
    return whatIsError();
}

bool DBService::insert(const qint64& idGroup, const QString& idDevice) const
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO group_object (id_group, id_object) "
                  "VALUES (:id_group, :id_object)");
    query.bindValue(":id_group", idGroup);
    query.bindValue(":id_object", idDevice);
    if (query.exec())
    {
        qDebug() << "Insert group-object success";
        return true;
    }
    return whatIsError();
}

bool DBService::create(const QString& userName, const QString& password) const
{
    QSqlQuery query(db);
    QString string = "CREATE USER " + userName + " PASSWORD '" + password + "'"; // костыль
    // пробывал скомбинировать способы и всячески подставить пароль через prepare, но не вышло
    if (query.exec(string))
    {
        qDebug() << "Create user success";
        return true;
    }
    return whatIsError();
}

qint32 DBService::selectUserId(const QString& userName) const
{
    QSqlQuery query(db);
    query.prepare("SELECT oid FROM pg_roles WHERE rolname = (:rolname)");
    query.bindValue(":rolname", userName);
    if (query.exec()) qDebug() << "Select user id success";
    else return whatIsError();
    query.next();
    return query.value(0).toInt(); // немагия (:
}

QList<Passport> DBService::selectAllPassports() const
{
    QList<Passport> passports;
    QSqlQuery query(db);
    if (query.exec("SELECT * FROM passports")) qDebug() << "Select all passports success";
    else if (!whatIsError()) return passports;
    Passport passport;
    while(query.next())
    {
        passport.id = query.value(0).toInt(); // тут лонг же, но не предупреждает ?
        passport.firstName = query.value(1).toString();
        passport.lastName  = query.value(2).toString();
        passport.callSign = query.value(3).toString();
        passport.birthday = query.value(4).toDate();
        passport.device = query.value(5).toString();
        passports.push_back(passport);
    }
    return passports;
}

QList<ObjectDev> DBService::selectAllObjects() const
{
    QList<ObjectDev> objects;
    QSqlQuery query(db);
    if (query.exec("SELECT * FROM objects")) qDebug() << "Select all objects success";
    else if (!whatIsError()) return objects;
    ObjectDev object;
    while(query.next())
    {
        object.id = query.value(0).toString();
        object.role = query.value(1).toInt(); // вот предупреждать предупреждает, а для short-то нет ничего
        object.speedLimit = query.value(2).toInt();
        object.link = query.value(3).toString();
        objects.push_back(object);
    }
    return objects;
}

QList<Polygon*> DBService::selectAllZones() const
{
    QList<Polygon*> zones;
    QSqlQuery query(db);
    if (query.exec("SELECT * FROM zones")) qDebug() << "Select all zones success";
    else if (!whatIsError()) return zones;
    Polygon* zone;
    while(query.next())
    {
        zone = new Polygon;
        zone->setId(query.value(0).toInt());
        zone->setName(query.value(1).toString());
        zone->fromString(query.value(2).toString());
        zone->setColor(query.value(3).toString());
        zone->setBorderColor(query.value(4).toString());
        zones.push_back(zone);
    }
    return zones;
}

QList<Access> DBService::selectAllAccesses() const
{
    QList<Access> accesses;
    QSqlQuery query(db);
    if (query.exec("SELECT * FROM access_rules")) qDebug() << "Select all accesses success";
    else if (!whatIsError()) return accesses;
    Access access;
    while(query.next())
    {
        access.id = query.value(0).toInt(); // лонг ?
        access.start = query.value(1).toDateTime();
        access.end = query.value(2).toDateTime();
        access.priority = query.value(3).toString();
        access.name = query.value(4).toString();
        access.group = query.value(5).toInt();
        access.zone = query.value(6).toInt();
        accesses.push_back(access);
    }
    return accesses;
}

QList<Group> DBService::selectAllGroups() const
{
    QList<Group> groups;
    QSqlQuery query(db);
    if (query.exec("SELECT * FROM task_groups")) qDebug() << "Select all groups success";
    else if (!whatIsError()) return groups;
    Group group;
    while(query.next())
    {
        group.id = query.value(0).toInt(); // лонг ?
        group.name = query.value(1).toString();
        groups.push_back(group);
    }
    return groups;
}

Group DBService::selectAllReferences(Group& group) const
{
    QSqlQuery query(db);
    query.prepare("SELECT id_object FROM group_object WHERE id_group = (:id_group)");
    query.bindValue(":id_group", group.id);
    if (query.exec()) qDebug() << "Select all devices for group success";
    else if (!whatIsError()) return group;
    while(query.next()) group.devices.push_back(query.value(0).toString()); // немагия (:
    return group;
}

QList<User> DBService::selectAllUsers() const
{
    QList<User> users;
    QSqlQuery query(db);
    if (query.exec("SELECT * FROM pg_roles WHERE oid > 16411")) qDebug() << "Select all users success";
    else if (!whatIsError()) return users;
    User user;
    while(query.next())
    {
        user.id = query.value(12).toInt();
        user.name = query.value(0).toString();
        users.push_back(user);
    }
    return users;
}

QString DBService::selectRole(const qint32& idUser) const
{
    QSqlQuery query(db);
    QString string = "{" + QString::number(idUser) + "}";
    query.prepare("SELECT groname FROM pg_group WHERE grolist @> (:grolist)");
    query.bindValue(":grolist", string);
    string = nullptr;
    if (query.exec()) qDebug() << "Select role for user success";
    else if (!whatIsError()) return string;
    if (query.next()) string = query.value(0).toString();
    return string;
}

QString DBService::selectSignFor(const QString& idDevice) const
{
    QSqlQuery query(db);
    query.prepare("SELECT call_sign FROM passports WHERE id_object = (:id_object)");
    query.bindValue(":id_object", idDevice);
    QString sign = nullptr;
    if (query.exec()) qDebug() << "Select sign for " << idDevice << " success";
    else if (!whatIsError()) return sign;
    if (query.next()) sign = query.value(0).toString();
    return sign;
}

Passport DBService::selectPassportFor(const QString& idDevice) const
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM passports WHERE id_object = (:id_object)");
    query.bindValue(":id_object", idDevice);
    Passport passport;
    if (query.exec()) qDebug() << "Select passport for " << idDevice << " success";
    else if (!whatIsError()) return passport;
    if (query.next())
    {
        passport.id = query.value(0).toInt(); // лонг ?
        passport.firstName = query.value(1).toString();
        passport.lastName  = query.value(2).toString();
        passport.callSign = query.value(3).toString();
        passport.birthday = query.value(4).toDate();
        passport.device = idDevice;
    }
    return passport;
}

QString DBService::selectZoneName(const qint32 idZone) const
{
    QSqlQuery query(db);
    query.prepare("SELECT zone_name FROM zones WHERE id = (:id)");
    query.bindValue(":id", idZone);
    QString name = nullptr;
    if (query.exec()) qDebug() << "Select zone_name success";
    else if (!whatIsError()) return name;
    if (query.next()) name = query.value(0).toString(); // немагия (:
    return name;
}

QString DBService::selectGroupName(const qint64 idGroup) const
{
    QSqlQuery query(db);
    query.prepare("SELECT group_name FROM task_groups WHERE id = (:id)");
    query.bindValue(":id", idGroup);
    QString name = nullptr;
    if (query.exec()) qDebug() << "Select group_name success";
    else if (!whatIsError()) return name;
    if (query.next()) name = query.value(0).toString(); // немагия (:
    return name;
}

QList<qint64> DBService::selectGroupsIdForObject(const QString& idDevice) const
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM group_object WHERE id_object = (:id_object)");
    query.bindValue(":id_object", idDevice);
    QList<qint64> idGroups;
    if (query.exec()) qDebug() << "Select id groups for " << idDevice << " success";
    else if (!whatIsError()) return idGroups;
    while(query.next()) idGroups.push_back(query.value(0).toInt()); // лонг ? немагия (:
    return idGroups;
}

bool DBService::select(AccessLine& rule, const qint32& idZone) const
{
    QSqlQuery query(db);
    query.prepare("SELECT zone_name, zone_color FROM zones WHERE id = (:id)");
    query.bindValue(":id", idZone);
    if (query.exec()) qDebug() << "Select zone name and color success";
    else return whatIsError();
    if (query.next())
    { // всё еще немагия (:
        rule.zoneName = query.value(0).toString();
        rule.color = query.value(1).toString();
    }
    return true;
}

qint16 DBService::select(const qint64& idGroup, const QString& idDevice) const
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM group_object WHERE id_group = (:id_group) AND id_object = (:id_object)");
    query.bindValue(":id_group", idGroup);
    query.bindValue(":id_object", idDevice);
    if (query.exec()) qDebug() << "Select group_object success";
    else return whatIsError();
    if (query.next()) qDebug() << "Девайс " << idDevice << " состоит в этой группе";
    else return 2;
    return 1; // если запись существует
}

qint16 DBService::selectGroup(const QString& groupName) const
{
    QSqlQuery query(db);
    query.prepare("SELECT id FROM task_groups WHERE group_name = (:group_name)");
    query.bindValue(":group_name", groupName);
    if (query.exec()) qDebug() << "Select group success";
    else return whatIsError();
    if (query.next()) qDebug() << "Группа с именем " << groupName << " существует";
    else return 2;
    return 1; // если запись существует
}

qint16 DBService::selectObjectId(const QString& idDevice) const
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM objects WHERE id_device = (:id_device)");
    query.bindValue(":id_device", idDevice);
    if (query.exec()) qDebug() << "Select object success";
    else return whatIsError();
    if (query.next()) qDebug() << "Девайс " << idDevice << " существует";
    else return 2;
    return 1; // если запись существует
}

qint16 DBService::selectSign(const QString& callSign) const
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM passports WHERE call_sign = (:call_sign)");
    query.bindValue(":call_sign", callSign);
    if (query.exec()) qDebug() << "Select sign success";
    else return whatIsError();
    if (query.next()) qDebug() << "Пасспорт с позывным " << callSign << " существует";
    else return 2;
    return 1; // если запись существует
}

qint16 DBService::selectUser(const QString& userName) const
{
    QSqlQuery query(db);
    query.prepare("SELECT oid FROM pg_roles WHERE rolname = (:rolname)");
    query.bindValue(":rolname", userName);
    if (query.exec()) qDebug() << "Select user success";
    else return whatIsError();
    if (query.next()) qDebug() << "Пользователь с именем " << userName << " существует";
    else return 2;
    return 1; // если запись существует
}

bool DBService::grant(const User& user) const
{
    QSqlQuery query(db);
    QString string = "GRANT " + user.role + " TO " + user.name; // костыль
    if (query.exec(string))
    {
        qDebug() << "Grant role success";
        return true;
    }
    return whatIsError();
}

bool DBService::alter(const QString& userName) const
{
    QSqlQuery query(db);
    QString string = "ALTER ROLE " + userName + " CREATEROLE"; // костыль
    if (query.exec(string))
    {
        qDebug() << "Alter user success";
        return true;
    }
    return whatIsError();
}

bool DBService::alter(const QString& user, const QString& password) const
{
    QSqlQuery query(db);
    QString string = "ALTER ROLE " + user + " PASSWORD '" + password + "'"; // костыль
    if (query.exec(string))
    {
        qDebug() << "Alter users password success";
        return true;
    }
    return whatIsError();
}

bool DBService::DROP(const QString& userName) const
{
    QSqlQuery query(db);
    QString string = "DROP ROLE " + userName; // костыль
    if (query.exec(string))
    {
        qDebug() << "Drop user success";
        return true;
    }
    return whatIsError();
}

bool DBService::deleteReference(const qint64& idGroup, const QString& idDevice) const
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM group_object WHERE id_group = (:id_group) AND id_object = (:id_object)");
    query.bindValue(":id_group", idGroup);
    query.bindValue(":id_object", idDevice);
    if (query.exec())
    {
        qDebug() << "Delete " << idDevice << " from group success"; // выглядит будто сам девайс удалили
        return true;
    }
    return whatIsError();
}

bool DBService::deletePassport(const qint64& idPassport) const
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM passports WHERE id = (:id)");
    query.bindValue(":id", idPassport);
    if (query.exec())
    {
        qDebug() << "Delete passport success";
        return true;
    }
    return whatIsError();
}

bool DBService::deleteObject(const QString& idDevice) const
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM objects WHERE id_device = (:id_device)");
    query.bindValue(":id_device", idDevice);
    if (query.exec())
    {
        qDebug() << "Delete object success";
        return true;
    }
    return whatIsError();
}

bool DBService::deleteZone(const qint32& idZone) const
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM zones WHERE id = (:id)");
    query.bindValue(":id", idZone);
    if (query.exec())
    {
        qDebug() << "Delete zone success";
        return true;
    }
    return whatIsError();
}

bool DBService::deleteAccess(const qint64& idAccess) const
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM access_rules WHERE id = (:id)");
    query.bindValue(":id", idAccess);
    if (query.exec())
    {
        qDebug() << "Delete access success";
        return true;
    }
    return whatIsError();
}

bool DBService::deleteGroup(const qint64& idGroup) const
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM task_groups WHERE id = (:id)");
    query.bindValue(":id", idGroup);
    if (query.exec())
    {
        qDebug() << "Delete group success";
        return true;
    }
    return whatIsError();
}

bool DBService::upDate(const Passport& passport) const
{
    QSqlQuery query(db);
    if (passport.device == nullptr) query.prepare("UPDATE passports SET "
                                                  "first_name = (:first_name), last_name = (:last_name), "
                                                  "call_sign = (:call_sign), birthday = (:birthday) "
                                                  "WHERE id = (:id)");
    else
    {
        query.prepare("UPDATE passports SET "
                      "first_name = (:first_name), last_name = (:last_name), call_sign = (:call_sign), birthday = (:birthday), "
                      "id_object = (:id_object) WHERE id = (:id)");
        query.bindValue(":id_object", passport.device);
    }
    query.bindValue(":first_name", passport.firstName);
    query.bindValue(":last_name", passport.lastName);
    query.bindValue(":call_sign", passport.callSign);
    query.bindValue(":birthday", passport.birthday);
    query.bindValue(":id", passport.id);
    if (query.exec())
    {
        qDebug() << "Update passport success";
        return true;
    }
    return whatIsError();
}

bool DBService::upDate(const ObjectDev& object) const
{
    QSqlQuery query(db);
    query.prepare("UPDATE objects SET "
                  "id_character = (:id_character), speed_limit_mpers = (:speed_limit_mpers), link = (:link) "
                  "WHERE id_device = (:id_device)");
    query.bindValue(":id_character", object.role);
    query.bindValue(":speed_limit_mpers", object.speedLimit);
    query.bindValue(":link", object.link);
    query.bindValue(":id_device", object.id);
    if (query.exec())
    {
        qDebug() << "Update object success";
        return true;
    }
    return whatIsError();
}

bool DBService::upDate(Polygon* zone) const
{
    QSqlQuery query(db);
    query.prepare("UPDATE zones SET zone_name = (:zone_name), polygon = (:polygon), "
                  "zone_color = (:zone_color), line_color = (:line_color) "
                  "WHERE id = (:id)");
    query.bindValue(":zone_name", zone->name());
    query.bindValue(":polygon", zone->toString());
    query.bindValue(":zone_color", zone->color().name(QColor::HexArgb));
    query.bindValue(":line_color", zone->borderColor().name(QColor::HexRgb));
    query.bindValue(":id", zone->id());
    if (query.exec())
    {
        qDebug() << "Update zone success";
        return true;
    }
    return whatIsError();
}

bool DBService::upDate(const Access& access) const
{
    QSqlQuery query(db);
    query.prepare("UPDATE access_rules SET start = (:start), \"end\" = (:end), priority = (:priority), "
                  "rule_name = (:rule_name), id_group = (:id_group), id_zone = (:id_zone) "
                  "WHERE id = (:id)");
    query.bindValue(":start", access.start);
    query.bindValue(":end", access.end);
    query.bindValue(":priority", access.priority);
    query.bindValue(":rule_name", access.name);
    query.bindValue(":id_group", access.group);
    query.bindValue(":id_zone", access.zone);
    query.bindValue(":id", access.id);
    if (query.exec())
    {
        qDebug() << "Update access success";
        return true;
    }
    return whatIsError();
}

bool DBService::upDate(const Group& group) const
{
    QSqlQuery query(db);
    query.prepare("UPDATE task_groups SET group_name = (:group_name) "
                  "WHERE id = (:id)");
    query.bindValue(":group_name", group.name);
    query.bindValue(":id", group.id);
    if (query.exec())
    {
        qDebug() << "Update group success";
        return true;
    }
    return whatIsError();
}
