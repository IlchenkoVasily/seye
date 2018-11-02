#include "objectsmodel.h"
#include "mainwindow.h"

#include <QQuickView>
#include <QQmlContext>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    seye::ObjectModel model;
    model.addObject(new seye::Object(56.38943723960041, 85.21442028046647));
    model.addObject(new seye::Object(56.38866517548572, 85.2159223175081));
    model.addObject(new seye::Object(56.3867765218958, 85.2117595291229));
    model.addObject(new seye::Object(56.38634888921545, 85.21306844710358));

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    QQmlContext* context = view.rootContext();
    context->setContextProperty("objectModel", &model);

    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

//    w.addModel("objectModel", &model);
  w.show();

    return a.exec();
}


//#include <QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
//}
