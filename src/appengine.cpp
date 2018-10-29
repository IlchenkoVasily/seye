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
    _objectModel.addObject(new seye::Object(56.38943723960041, 85.21442028046647));
    _objectModel.addObject(new seye::Object(56.38866517548572, 85.2159223175081));
    _objectModel.addObject(new seye::Object(56.3867765218958, 85.2117595291229));
    _objectModel.addObject(new seye::Object(56.38634888921545, 85.21306844710358));

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    QQmlContext* context = view.rootContext();

    context->setContextProperty("objectModel", &_objectModel);
    context->setContextProperty("polygonModel", &_polygonModel);

    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.show();
}
