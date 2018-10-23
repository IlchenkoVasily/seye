#include "objectsmodel.h"

using namespace seye;

ObjectModel::ObjectModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

void ObjectModel::addObject(const Object& newObj)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _objects << newObj;
    endInsertRows();
}

int ObjectModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return _objects.count();
}

QVariant ObjectModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= _objects.count())
        return QVariant();

    const Object& object = _objects[index.row()];
    if (role == IdRole)
        return object.id();
    else if (role == CoordinateRole)
        return QVariant::fromValue(object.coordinate());

    return QVariant();
}

QHash<int, QByteArray> ObjectModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[IdRole] = "id";
    roles[CoordinateRole] = "coordinate";

    return roles;
}
