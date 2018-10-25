#include "polygonmodel.h"

using namespace seye;

PolygonModel::PolygonModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int PolygonModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return _polygons.count();
}

QVariant PolygonModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _polygons.count())
        return QVariant();

    QGeoPolygon poly = _polygons[index.row()];

    if (role == PathRole)
        return QVariant::fromValue(poly.path());

    return QVariant();
}

// пока без реализации всвязи с отсутствием необходимости
bool PolygonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

// пока без реализации всвязи с отсутствием необходимости
Qt::ItemFlags PolygonModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

void PolygonModel::addPolygon(QGeoPolygon polygon)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _polygons << polygon;
    endInsertRows();
}

QHash<int, QByteArray> PolygonModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[PathRole] = "path";

    return roles;
}
