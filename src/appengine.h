#ifndef APPENGINE_H
#define APPENGINE_H

#include <QObject>
#include <QQuickView>
#include <QQmlContext>

#include "object.h"
#include "objectsmodel.h"
#include "polygonmodel.h"
#include "fakeconnector.h"

namespace seye
{
    class AppEngine : public QObject
    {
        Q_OBJECT

    public:
        explicit AppEngine(QObject *parent = nullptr);
        ~AppEngine();

        /*
            Данный метод устанавливает коннектор
            для взаимодействия с внешней базовой
            станции и получения из неё данных о
            положении объектов.
         */
        void setConnector(IConnector* connector);

        /*
            Данный метод производит добавление
            моделей в QQmlContext из QQuickVier,
            а также соеденение сигналов и слотов.
        */
        void setUp();

    signals:

    public slots:
        /*
            Слот для принятия пакетов от датчиков.
         */
        void onObjectsUpdate();

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
        IConnector* _connector;
        // here DatabaseDriver
    };
}

#endif // APPENGINE_H
