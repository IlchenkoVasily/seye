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
            if (poly->contains(obj->coordinate()))
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
//    connect(_connector, SIGNAL(IConnecter::complete(QList<Pak>*)), this, SLOT(onObjectsUpdate()));
    _connector->connectTo(228);
//    _connector->

//    _objectModel.addObject(new seye::Object(1, 56.38943723960041, 85.21442028046647));
//    _objectModel.addObject(new seye::Object(2, 56.38866517548572, 85.2159223175081));
//    _objectModel.addObject(new seye::Object(3, 56.3867765218958, 85.2117595291229));
//    _objectModel.addObject(new seye::Object(4, 56.38634888921545, 85.21306844710358));

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
