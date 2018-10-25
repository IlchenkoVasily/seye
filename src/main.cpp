#include "objectsmodel.h"
#include "polygonmodel.h"

#include <QQuickView>
#include <QQmlContext>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    seye::ObjectModel model;
    model.addObject(new seye::Object(56.38943723960041, 85.21442028046647));
    model.addObject(new seye::Object(56.38866517548572, 85.2159223175081));
    model.addObject(new seye::Object(56.3867765218958, 85.2117595291229));
    model.addObject(new seye::Object(56.38634888921545, 85.21306844710358));

    seye::PolygonModel polyModel;

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    QQmlContext* context = view.rootContext();
    context->setContextProperty("objectModel", &model);
    context->setContextProperty("polygonModel", &polyModel);

    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    return a.exec();
}
