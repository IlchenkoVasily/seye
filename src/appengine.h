#ifndef APPENGINE_H
#define APPENGINE_H

#include <QObject>
#include <QQuickView>
#include <QQmlContext>
#include <QTime>

#include "object.h"
#include "objectsmodel.h"
#include "polygonmodel.h"
#include "fakeconnector.h"
#include "mainwindow.h"

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
        void onObjectsUpdate(ObjectsPakPtr&);

    private:
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
            Здесь функции для добавления уведомления
        */
        void add_ward(int tarid, int warnid);
        void CollideTo(int &Objid);
        QStringList WarnsListToView; // уведомления на вывод
        //взято short чтобы не было сильно страшно
        short int WarningQ = 0; // количество уведомлений

        /*
            Структура для уведомления
        */
        struct warn
        {
            int warnid; //id нарушения
            int id; //id нарушителя
            int idzone; //id зоны
            QTime time; // время нарушения
        };
        warn WarnListStr[255]; //Уведомления для внутреннего использования

        MainWindow* _window;
        PolygonModel _polygonModel;
        ObjectModel _objectModel;
        IConnector* _connector;
        //QListWidget* _WarnList; // набор уведомлений
        // here DatabaseDriver
    };
}

#endif // APPENGINE_H
