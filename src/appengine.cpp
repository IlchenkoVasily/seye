#include "appengine.h"

using namespace seye;

AppEngine::AppEngine(QObject *parent) : QObject(parent)
{

}

void AppEngine::checkEntries()
{

}

void AppEngine::setUp()
{
    _objectModel.addObject(new seye::Object(1, 56.38943723960041, 85.21442028046647));
    _objectModel.addObject(new seye::Object(2, 56.38866517548572, 85.2159223175081));
    _objectModel.addObject(new seye::Object(3, 56.3867765218958, 85.2117595291229));
    _objectModel.addObject(new seye::Object(4, 56.38634888921545, 85.21306844710358));

    _polygonModel.beginCreatePolygon();
    _polygonModel.addCoordinate(QGeoCoordinate(56.389152172, 85.204978905));
    _polygonModel.addCoordinate(QGeoCoordinate(56.386812158, 85.206395111));
    _polygonModel.addCoordinate(QGeoCoordinate(56.386455798, 85.214591942));
    _polygonModel.addCoordinate(QGeoCoordinate(56.389152172, 85.212918243));
    _polygonModel.endCreatePolygon();

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    QQmlContext* context = view.rootContext();

    context->setContextProperty("objectModel", &_objectModel);
    context->setContextProperty("polygonModel", &_polygonModel);

    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.show();
}
