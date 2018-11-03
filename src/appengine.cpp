#include "appengine.h"

using namespace seye;

AppEngine::AppEngine(QObject *parent) : QObject(parent)
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

void AppEngine::checkEntries()
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
                // Здесь установка статуса для объектов
            }
        }
        // где-то здесь установка статуса для полигона,
        // чтобы постоянно не драконить методы полигона.
    }
}

void AppEngine::setUp()
{
    //--- setup connnector ----//
    connect(_connector, SIGNAL(complete(ObjectsPakPtr&)), this, SLOT(onObjectsUpdate(ObjectsPakPtr&)));
    _connector->connectTo(228);
    _connector->start();

    // имитация доставания из бд )00)
    _polygonModel.beginCreatePolygon();
    _polygonModel.addCoordinate(QGeoCoordinate(56.389152172, 85.204978905));
    _polygonModel.addCoordinate(QGeoCoordinate(56.386812158, 85.206395111));
    _polygonModel.addCoordinate(QGeoCoordinate(56.386455798, 85.214591942));
    _polygonModel.addCoordinate(QGeoCoordinate(56.389152172, 85.212918243));
    _polygonModel.endCreatePolygon();

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    QQmlContext* context = view.rootContext();

    context->setContextProperty("polygonModel", &_polygonModel);
    context->setContextProperty("objectModel", &_objectModel);

    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.show();
}

void AppEngine::onObjectsUpdate(ObjectsPakPtr& objPaks)
{
    for (auto begin = objPaks->begin(), end = objPaks->end(); begin != end; begin++)
    {
        Object obj(begin->devId, begin->latitude, begin->longitude);
        _objectModel.addObject(obj);
    }
}
