/************************************************
** PolygonModel -- класс-модель, для передачи
** данных о полигоне в представление
************************************************/

#ifndef POLYGONMODEL_H
#define POLYGONMODEL_H

#include <QAbstractListModel>
#include <QGeoPolygon>

namespace seye
{
    class PolygonModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        enum {
            PathRole = Qt::UserRole + 1
        };

        explicit PolygonModel(QObject *parent = nullptr);

        // Basic functionality:
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        // Editable:
        bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole) override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;

        // этот метод должке также вызываться из qml
        Q_INVOKABLE void addPolygon(QGeoPolygon polygon);

    protected:
        QHash<int, QByteArray> roleNames() const override;

    private:
        QList<QGeoPolygon> _polygons;
    };
}

#endif // POLYGONMODEL_H
