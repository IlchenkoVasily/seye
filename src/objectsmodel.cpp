#include "objectsmodel.h"

#include <QtDebug>

using namespace seye;

ObjectModel::ObjectModel(QObject* parent)
    : QAbstractListModel(parent)
{}

ObjectModel::~ObjectModel()
{

}

void ObjectModel::addObject(Object& newObj)
{
    int idx = _objects.indexOf(newObj);
    if (idx != -1)
    {
        _objects[idx].setCoordinate(newObj.coordinate());

        emit dataChanged(index(idx), index(rowCount() - 1), QVector<int>() << CoordinateRole);

        qDebug() << "Updated" << newObj.id() << "with" << newObj.coordinate();
        return;
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _objects << newObj;
    endInsertRows();

    qDebug() << "Added" << newObj.id() << "with" << newObj.coordinate();
}

const QList<Object>& ObjectModel::toList() const
{
    return _objects;
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

    Object object = _objects[index.row()];
    if (role == IdRole)
        return QVariant(object.id());
    else if (role == CoordinateRole)
        return QVariant::fromValue(object.coordinate());

    return QVariant();
}

void ObjectModel::update()
{
    // here new implementation

    emit dataChanged(index(0), index(rowCount() - 1), QVector<int>() << CoordinateRole);
}

QHash<int, QByteArray> ObjectModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[IdRole] = "id";
    roles[CoordinateRole] = "coordinate";

    return roles;
}
