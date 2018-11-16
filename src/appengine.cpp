#include "appengine.h"
#include "QTime"

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
33    _connector->start();

    // имитация доставания из бд )00)
    _polygonModel.beginCreatePolygon();
    _polygonModel.addCoordinate(QGeoCoordinate(56.390058884, 85.212116396));
    _polygonModel.addCoordinate(QGeoCoordinate(56.388846197, 85.216967378));
    _polygonModel.addCoordinate(QGeoCoordinate(56.385634613, 85.213272466));
    _polygonModel.addCoordinate(QGeoCoordinate(56.387214293, 85.209644591));
    _polygonModel.endCreatePolygon();

    _window->addModel("polygonModel", &_polygonModel);
    _window->addModel("objectModel", &_objectModel);

    _window->show();
}

void AppEngine::onObjectsUpdate(ObjectsPakPtr& objPaks)
{
    for (auto begin = objPaks->begin(), end = objPaks->end(); begin != end; begin++)
    {
        Object obj(begin->devId, begin->latitude, begin->longitude);
        checkEntries(obj);
        _objectModel.addObject(obj);
    }
}

void AppEngine::checkEntries(Object& object)
{
    auto polygons = _polygonModel.toList();
    object.setState(State::Allowed);

    for (auto poly: polygons)
    {
        if (poly->contains(object.coordinate()))
        {
            object.setState(State::Intruder);
            //CollideTo(obj.id()); //при пересечение зоны будет уведомление
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

void AppEngine::AddToWidget()
{
    WarnList.addItems(WarnsListToView);
}

void AppEngine::CollideTo(int &Objid, int &Zoneid)
{
    add_ward();
}

void AppEngine::add_ward(int tarid, int warnid)
{
    /*tarid - id нарушителя
     *warnid - id нарушения
     */
    WarnListStr[WarningQ].time = QTime::currentTime();
    WarnListStr[WarningQ].id = tarid;
    WarnListStr[WarningQ].warnid = warnid; //TODO id зоны, пока не используется
    WarnsListToView = QStringList() << WarnListStr[WarningQ].time.toString() + '|' + QString(WarnListStr[WarningQ].id) + "зашёл в зону";
    if(WarningQ == 255) WarningQ = 0;
    WarningQ++;
    AddToWidget();

}
