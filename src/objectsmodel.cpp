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
        // Обновляем координаты у объекта
        _objects[idx].setCoordinate(newObj.coordinate());
        _objects[idx].setState(newObj.state());

        // Сигнал о том, что данные в модели изменены.
        // Индексы наших объектов в моделе, изменённый параметр
        emit dataChanged(index(idx), index(idx), QVector<int>() << CoordinateRole << StateRole);
        return;
    }

    // Вставка нового объекта
    // Модельный индекс, индекс начала вставки и конца вставки
    //
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _objects << newObj;
    endInsertRows();
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

    switch (role) {
    case IdRole:
        return QVariant(object.id());
    case CoordinateRole:
        return QVariant::fromValue(object.coordinate());
    case StateRole:
        return QVariant(object.state());
    default:
        return QVariant();
    }
}

void ObjectModel::update()
{
    // here new implementation

    // Обновление всей модели
    emit dataChanged(index(0), index(rowCount() - 1), QVector<int>() << CoordinateRole);
}

QHash<int, QByteArray> ObjectModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[IdRole] = "id";
    roles[CoordinateRole] = "coordinate";
    roles[StateRole] = "state";

    return roles;
}
