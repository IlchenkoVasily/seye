/************************************************
** PolygonModel -- класс-модель, для передачи
** данных о полигоне в представление
************************************************/

#ifndef POLYGONMODEL_H
#define POLYGONMODEL_H

#include <QAbstractTableModel>
#include <QItemSelectionModel>
#include "polygon.h"

namespace seye
{
    class PolygonModel : public QAbstractTableModel
    {
        Q_OBJECT
        Q_PROPERTY(bool onCreate READ onCreate NOTIFY onCreateChanged)

    public:
        enum {
            PathRole = Qt::UserRole + 1,
            IdRole,
            NameRole,
            ColorRole,
            BorderColorRole,
            MapColorRole,
            MapBorderColorRole,
            SelectionRole
        };

        explicit PolygonModel(QObject *parent = nullptr);
        ~PolygonModel() override;

        /*
            Данный метод возвращает количество полигонов в моделе
        */
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        /*
            Данный метод возвращает количество полей на каждый полигон
        */
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        /*
            Данный метод возвращает данные полигона по индексу
            и роли.
        */
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        /*
            Данным метод делает в представлении таблицы именованные хедеры
        */
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;


        // Для изменения уже имеющихся данных:
        bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole) override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;

        /*
            Данный метод добавляет в модель созданный
            в qml новый полигон
        */
        Q_INVOKABLE void addPolygon(Polygon* polygon);

        /*
            Данный метод, вызываемый из qml, передаёт
            координату для создаваемого полигона.
        */
        Q_INVOKABLE void addCoordinate(const QGeoCoordinate& coord);

        /*
            Данный метод, вызываемый из qml, сообщает
            о конце создания полигона. Новый полигон
            добавляется в модель данных.
        */
        Q_INVOKABLE void endCreatePolygon();

        /*
            Данный метод, вызываемый из qml, сообщает
            об отмене создания нового полигона. Чистит
            выделенную под новый полигон память.
        */
        Q_INVOKABLE void cancelCreatePolygon();

        /*
            TODO write a commnet
        */
        bool onCreate() { return _onCreatePolygon; }

        /*
            Возвращает указатель на зону внимания
         */
        Polygon* attentionZone();

        /*
            Данный метод возвращает ссылку на лист
            со всеми полигонами.
        */
        const QList<Polygon*>& toList() const;

    signals:
        /*
            TODO write a comment
         */
        void onCreateChanged(bool);

        /*
            Сигнал для центрирования по координате.
         */
        void polygonCentering(const QGeoCoordinate& coordinate);

    public slots:

        /*
            Данный метод, вызываемый из qml, сообщает
            о начале создания нового полигона. Выделяет
            место в куче для нового Polygon.
        */
        void beginCreatePolygon();

        /*
            Слот для изменения статус isSelected во
            всех полигонах, которые были выделены.
         */
        void onPolygonSelected(const QItemSelection &selected, const QItemSelection &deselected);

        /*
            Слот по индексу находит полигон, расчитывает центр
            и отсылает сигнал polygonCentering(...).
         */
        void polygonLook(const QModelIndex& index);

    protected:
        /* */
        QHash<int, QByteArray> roleNames() const override;

    private:
        // udolee
        int newPolyId = 0;
        // Идёт процесс создания полигона.
        bool _onCreatePolygon;
        // Временная переменная для полигона.
        Polygon* _tempPolygon;
        // Зона внимания
        Polygon* _attentionZone;
        // Список всех полигонов
        QList<Polygon*> _polygons;
    };
}

#endif // POLYGONMODEL_H
