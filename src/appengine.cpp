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
    connect(_connector, SIGNAL(complete(ObjectsPakPtr&)), this, SLOT(onObjectsUpdate(ObjectsPakPtr&)));
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

    for (auto poly: polygons)
    {
        // не чекаем зону внимания.
        if (poly == attention)
            continue;

        if (poly->contains(object.coordinate()))
        {
            auto groupsForZone = groupsForZoneFromAccesses(poly->id());

            auto groupsForObject = _database->getGroupsIdForObject(object.id());

            bool stillAllowed = false;
            foreach (int objectGr, groupsForObject)
            {
                for (int i = 0; i < groupsForZone->count(); i++)
                {
                    int zoneGr = groupsForZone->at(i);
                    if (objectGr == zoneGr)
                    {
                        stillAllowed = true;
                        break;
                    }
                }
            }

            if (!stillAllowed)
                object.setState(State::Intruder);
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
        auto zoneForCheck = _ruleModel->data(_ruleModel->index(i, 6)).toInt();
        if (zoneForCheck == zoneId)
            list->append(_ruleModel->data(_ruleModel->index(i, 5)).toInt());
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

        _ruleModel->appendRow(QList<QStandardItem*>() << number
                                  << timeStart << timeEnd << status
                                   << name     << group   << zone);
    }

//    _ruleModel->
}
