#ifndef APPENGINE_H
#define APPENGINE_H

#include <QObject>
#include <QQuickView>
#include <QQmlContext>
#include <QStandardItemModel>

#include "object.h"
#include "objectsmodel.h"
#include "polygonmodel.h"
#include "fakeconnector.h"
#include "mainwindow.h"
#include "dbservice.h"
#include "login.h"

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
        void objectsUpdated();

    public slots:
        /*
            Слот для принятия пакетов от датчиков.
         */
        void onObjectsUpdate(ObjectsPakPtr&);

        /*
            Метод обновляет пасспорт (по одной ячейке).
        */
        void onPassportUpdate(QStandardItem*);

        /*
            Метод обновляет правило (по одной ячейке).
         */
        void onAccessUpdate(QStandardItem*);

    private:
        MainWindow* _window;
        PolygonModel* _polygonModel;
        ObjectModel* _objectModel;
        QStandardItemModel* _passportModel;
        QStandardItemModel* _ruleModel;
        IConnector* _connector;
        DBService* _database;
        QString _userRole;

        /*
            Мап для девайсов и списка групп, в которые
            он входит.
         */
        QMap<QString, QList<qint64>> groupsForObject;

          /*----------------*/
         /*---- Методы ----*/
        /*----------------*/

        /*
            Данный метод проверяет каждый полигон
            на наличие в нём объекта object.
            Помечает полигон, объект, изменяя
            их внутренний статус.
         */
        void checkEntries(Object& object);

        /*
            Даннный метод проверяет каждый полигон
            на вхождение в себя любого объекта.
            Помечает и полигон, и объект, изменяя
            их внутренний статус.
        */
        void checkEntriesAll();

        /*
            Даннный метод поднимает из бд модель
            пасспортов.
        */
        void setupPassports();

        /*
            Данный метод возвращает список айди групп,
            полученных из правила, где участвует зона
            с айди zoneId.
         */
        QList<int>* groupsForZoneFromAccesses(int zoneId);

        /*
            Даннный метод поднимает из бд модель
            правил.
        */
        void setupRules();
    };
}

#endif // APPENGINE_H
