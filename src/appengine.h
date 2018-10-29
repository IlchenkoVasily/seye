#ifndef APPENGINE_H
#define APPENGINE_H

#include <QObject>
#include <QQuickView>
#include <QQmlContext>

#include "object.h"
#include "objectsmodel.h"
#include "polygonmodel.h"

namespace seye
{
    class AppEngine : public QObject
    {
        Q_OBJECT

    public:
        explicit AppEngine(QObject *parent = nullptr);

        /*
            Данный метод производит добавление
            моделей в QQmlContext из QQuickVier,
            а также соеденение сигналов и слотов.
        */
        void setUp();

    signals:

    public slots:

    private:
        /*
            Даннный метод проверяет каждый объект
            на вхождение в какой-либо полигон.
            Помечает и полигон, и объект, изменяя
            их внутренний статус.
        */
        void checkEntries();


        PolygonModel _polygonModel;
        ObjectModel _objectModel;
        QQuickView view;
        // here Connector
        // here DatabaseDriver
    };
}

#endif // APPENGINE_H
