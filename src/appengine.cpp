#include "appengine.h"

using namespace seye;

AppEngine::AppEngine(QObject *parent) : QObject(parent),
    _window(new MainWindow)
{
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

    // имитация доставания из бд )00)
    Polygon* poly = new Polygon;
    poly->fromString("56.390058884 85.212116396|"
                     "56.388846197 85.216967378|"
                     "56.385634613 85.213272466|"
                     "56.387214293 85.209644591");
    poly->setColor(QColor(64, 255, 64, 100));
    poly->setBorderColor(QColor("green"));
    poly->setName("Парсированная");
    poly->setIsSelected(false);
    _polygonModel.addPolygon(poly);

    // Добавляем модели (уже поднятые из бд) во MainWindow
    _window->addModel("polygonModel", &_polygonModel);
    _window->addModel("objectModel", &_objectModel);

    // Коннектим селекшен модели для обновления выделений
    connect(_window->getPolygonSelection(),
            SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            &_polygonModel,
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
        _objectModel.addObject(obj);
    }
    emit objectsUpdated();
}

void AppEngine::checkEntries(Object& object)
{
    auto polygons = _polygonModel.toList();
    object.setState(State::Allowed);

    // Проверяем, находится ли наш объект в зоне внимания
    auto attention = _polygonModel.attentionZone();
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
            object.setState(State::Intruder);
        }
    }
}

void AppEngine::checkEntriesAll()
{
    auto polygons = _polygonModel.toList();
    auto objects  = _objectModel.toList();

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
