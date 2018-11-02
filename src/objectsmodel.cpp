#include "objectsmodel.h"

#include <QtDebug>

using namespace seye;

ObjectModel::ObjectModel(QObject* parent)
    : QAbstractListModel(parent)
{
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(5000);
}

ObjectModel::~ObjectModel()
{
    // Освобождаем все указатели точек
    for (int i = 0; i < _objects.count(); i++)
    {
        delete _objects[i];
    }

    delete timer;
}

void ObjectModel::addObject(Object* newObj)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _objects << newObj;
    endInsertRows();
}

const QList<Object*>& ObjectModel::toList() const
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

    Object* object = _objects[index.row()];
    if (role == IdRole)
        return object->id();
    else if (role == CoordinateRole)
        return QVariant::fromValue(object->coordinate());

    return QVariant();
}

void ObjectModel::update()
{
    for(auto obj: _objects)
    {
        QGeoCoordinate coord = obj->coordinate();
        coord.setLatitude(coord.latitude() + 0.0001 * ((qrand() % 10) - 5));
        coord.setLongitude(coord.longitude() + 0.0001 * ((qrand() % 10) - 5));
        obj->setCoordinate(coord);
    }

    emit dataChanged(index(0), index(rowCount() - 1), QVector<int>() << CoordinateRole);
}

QHash<int, QByteArray> ObjectModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[IdRole] = "id";
    roles[CoordinateRole] = "coordinate";

    return roles;
}
