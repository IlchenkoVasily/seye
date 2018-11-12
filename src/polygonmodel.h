/************************************************
** PolygonModel -- класс-модель, для передачи
** данных о полигоне в представление
************************************************/

#ifndef POLYGONMODEL_H
#define POLYGONMODEL_H

#include <QAbstractListModel>
#include "polygon.h"

namespace seye
{
    class PolygonModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        enum {
            PathRole = Qt::UserRole + 1,
            IdRole,
            NameRole,
            ColorRole,
            BorderColorRole
        };

        explicit PolygonModel(QObject *parent = nullptr);
        ~PolygonModel() override;

        /*
            Данный метод возвращает количество полигонов в моделе
        */
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        /*
            Данный метод возвращает данные полигона по индексу
            и роли.
        */
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

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
            Данный метод, вызываемый из qml, сообщает
            о начале создания нового полигона. Выделяет
            место в куче для нового Polygon.
        */
        Q_INVOKABLE void beginCreatePolygon();

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
            Данный метод возвращает ссылку на лист
            со всеми полигонами.
        */
        const QList<Polygon*>& toList() const;

    protected:
        /* */
        QHash<int, QByteArray> roleNames() const override;

    private:
        // Идёт процесс создания полигона.
        bool _onCreatePolygon;
        // Временная переменная для полигона.
        Polygon* _tempPolygon;

        QList<Polygon*> _polygons;
    };
}

#endif // POLYGONMODEL_H
