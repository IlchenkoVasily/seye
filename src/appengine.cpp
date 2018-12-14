#include "appengine.h"

using namespace seye;

AppEngine::AppEngine(QObject *parent) : QObject(parent)
{
    login user(&_userRole);
    user.setModal(true);
    user.exec();

    if (_userRole.isEmpty()) qApp->quit();
    else
    {
        _database = user.getDatabase();

        _window = new MainWindow(_database, _userRole);

        _polygonModel = new PolygonModel(_window);
        _objectModel = new ObjectModel(_window);
        _passportModel = new QStandardItemModel(_window);
        _ruleModel = new QStandardItemModel(_window);

        connect(_passportModel, SIGNAL(itemChanged(QStandardItem*)),
                this, SLOT(onPassportUpdate(QStandardItem*)));

        connect(_ruleModel, SIGNAL(itemChanged(QStandrdItem*)),
                this, SLOT(onAccessUpdate(QStandardItem*)));
    }
}

AppEngine::~AppEngine()
{
    _connector->disconnect();
    delete _connector;
}

void AppEngine::setConnector(IConnector *connector)
{
    _connector = connector;
}

void AppEngine::setUp()
{
    //--- setup connnector ----//
    connect(_connector, SIGNAL(complete(ObjectsPakPtr&)),
            this, SLOT(onObjectsUpdate(ObjectsPakPtr&)));
    _connector->connectTo(228);
    _connector->start();

    // Поднимаем из бд все нужны модели
    // Полигоны
    foreach (auto poly, _database->getAllZones())
    {
        _polygonModel->addPolygon(poly);
    }

    // Объекты
    foreach (auto obj, _database->getAllObjects())
    {
        QString name = _database->getCallSignFor(obj.id);
        seye::Object object(obj, name);
        _objectModel->addObject(object);
    }

    // setup passports model
    setupPassports();

    // setup rules model
    setupRules();

    // Добавляем модели (уже поднятые из бд) во MainWindow
    _window->addModel("polygonModel", _polygonModel);
    _window->addModel("objectModel", _objectModel);
    _window->addModel("passportModel", _passportModel);
    _window->addModel("ruleModel", _ruleModel);

    // Коннектим селекшен модели для обновления выделений
    connect(_window->getPolygonSelection(),
            SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            _polygonModel,
            SLOT(onPolygonSelected(const QItemSelection&, const QItemSelection&)));

    // Показываем окно
    _window->show();
}

void AppEngine::onObjectsUpdate(ObjectsPakPtr& objPaks)
{
    for (auto begin = objPaks->begin(), end = objPaks->end(); begin != end; begin++)
    {
        Object obj(begin->devId, begin->latitude, begin->longitude);

        obj.setRole(Role::Worker);
        checkEntries(obj);
        _objectModel->addObject(obj);
    }
    emit objectsUpdated();
}

void AppEngine::onPassportUpdate(QStandardItem* item)
{
    auto index = item->index();
    int row = index.row();
    QString data = item->text();

    Passport pass;
    pass.id = _passportModel->item(row, 0)->accessibleDescription().toInt();
    pass.firstName = _passportModel->item(row, 1)->text();
    pass.lastName = _passportModel->item(row, 2)->text();
    pass.birthday = QDate::fromString(_passportModel->item(row, 3)->text(), Qt::ISODate);
    pass.callSign = _passportModel->item(row, 4)->text();
    pass.device = _passportModel->item(row, 5)->text();

    _database->update(QList<Passport>() << pass);
}

void AppEngine::onAccessUpdate(QStandardItem* item)
{
    auto index = item->index();
    int row = index.row();
    QString data = item->text();

    Access rule;
    rule.id = _ruleModel->item(row, 0)->accessibleDescription().toInt();
    rule.start = QDateTime::fromString(_ruleModel->item(row, 1)->text(), Qt::ISODate);
    rule.end = QDateTime::fromString(_ruleModel->item(row, 2)->text(), Qt::ISODate);
    rule.priority = _passportModel->item(row, 3)->text();
    rule.name = _passportModel->item(row, 4)->text();
    rule.group = _passportModel->item(row, 5)->accessibleDescription().toInt();
    rule.zone = _passportModel->item(row, 6)->accessibleDescription().toInt();

    _database->update(QList<Access>() << rule);
}

void AppEngine::checkEntries(Object& object)
{
    auto polygons = _polygonModel->toList();
    object.setState(State::Allowed);

    // Проверяем, находится ли наш объект в зоне внимания
    auto attention = _polygonModel->attentionZone();
    if (!attention->contains(object.coordinate()))
    {
        object.setState(State::OutOfAttention);
        return;
    }

    // Проверяем, закеширован ли список групп для данного объекта
    // Если нет, то кешируем.
    if (!groupsForObject.contains(object.id()))
        groupsForObject.insert(object.id(), _database->getGroupsIdForObject(object.id()));

    for (auto poly: polygons)
    {
        // не чекаем зону внимания.
        if (poly == attention)
            continue;

        if (poly->contains(object.coordinate()))
        {
            auto groupsForZone = groupsForZoneFromAccesses(poly->id());

            auto groupsObject = groupsForObject[object.id()];

            bool stillAllowed = false;
            foreach (qint64 objectGr, groupsObject)
            {
                if (groupsForZone->contains(objectGr))
                {
                    stillAllowed = true;
                    break;
                }
            }

            if (!stillAllowed)
                object.setState(State::Intruder);

            delete groupsForZone;
        }
    }
}

void AppEngine::checkEntriesAll()
{
    auto polygons = _polygonModel->toList();
    auto objects  = _objectModel->toList();

    // QTHREAD
    // or threadpool ?7?
    for (auto poly: polygons)
    {
        for (auto obj: objects)
        {
            if (poly->contains(obj.coordinate()))
            {
                qDebug() << "Объект[" << obj.id() << "] пересекает некоторую зону.";
            }
        }
        // где-то здесь установка статуса для полигона,
        // чтобы постоянно не драконить методы полигона.
    }
}

void AppEngine::setupPassports()
{
    auto passports = _database->getAllPassports();

    _passportModel->setHorizontalHeaderLabels(QStringList() << "№" << "Имя"
                                              << "Фамилия"  << "Дата рождения"
                                              << "Позывной" << "Устройство");

    int row = 1;
    foreach (auto pass, passports)
    {
        auto number    = new QStandardItem(QString::number(row++));
        auto lastName  = new QStandardItem(pass.lastName);
        auto firstName = new QStandardItem(pass.firstName);
        auto birth     = new QStandardItem(pass.birthday.toString(QString("dd-MM-yyyy")));
        auto link      = new QStandardItem(pass.callSign);
        auto device    = new QStandardItem(pass.device);

        number->setAccessibleDescription(QString::number(pass.id));

        _passportModel->appendRow(QList<QStandardItem*>()
                                  << number << firstName << lastName
                                  << birth  << link      << device);
    }
}
//                     groupsForZoneFromAccesses
QList<int>* AppEngine::groupsForZoneFromAccesses(int zoneId)
{
    QList<int>* list = new QList<int>;

    for (int i = 0; i < _ruleModel->rowCount(); i++)
    {
        auto zone_item = _ruleModel->item(i, 6);
        int zoneForCheck = zone_item->accessibleDescription().toInt();
        if (zoneForCheck == zoneId)
            list->append(_ruleModel->item(i, 5)->accessibleDescription().toInt());
    }

    return list;
}

void AppEngine::setupRules()
{
    auto rules = _database->getAllAccesses();

    _ruleModel->setHorizontalHeaderLabels(QStringList() << "№"      << "Время начала"
                                          << "Конца"    << "Статус" << "Название"
                                          << "Группа"   << "Зона");

    int row = 1;
    foreach (auto rule, rules)
    {
        auto number    = new QStandardItem(QString::number(row++));
        auto timeStart = new QStandardItem(rule.start.toString("dd-MM-yyyy hh:mm"));
        auto timeEnd   = new QStandardItem(rule.end.toString("dd-MM-yyyy hh:mm"));
        auto status    = new QStandardItem(rule.priority);
        auto name      = new QStandardItem(rule.name);
        auto group     = new QStandardItem(_database->getGroupName(rule.group));
        auto zone      = new QStandardItem(_database->getZoneName(rule.zone));

        number->setAccessibleDescription(QString::number(rule.id));
        group->setAccessibleDescription(QString::number(rule.group));
        zone->setAccessibleDescription(QString::number(rule.zone));

        _ruleModel->appendRow(QList<QStandardItem*>() << number
                                  << timeStart << timeEnd << status
                                  << name      << group   << zone);
    }
}
