#include "objectsmodel.h"

#include <QtDebug>

using namespace seye;

ObjectModel::ObjectModel(QObject* parent)
    : QAbstractTableModel(parent)
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
        emit dataChanged(index(idx, 0), index(idx, 0),
                         QVector<int>() << CoordinateRole << StateRole);
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
    if (parent.isValid())
        return 0;

    return _objects.count();
}

int ObjectModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 3;
}

QVariant ObjectModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= _objects.count())
        return QVariant();

    Object object = _objects[index.row()];

    switch (role) {
    case Qt::DisplayRole: {
        if (index.column() == 0) return QString::number(object.id());
        return QVariant();
    }

    case Qt::DecorationRole: {
        if (index.column() == 1)
        {
            auto state = object.state();
            if (state == State::Intruder) return QColor(255, 0, 0);
            if (state == State::Allowed) return QColor(0, 255, 0);
        }
        return QVariant();
      }

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

QVariant ObjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section) {
            case 0:
                return QString("ID");
            case 1:
                return QString("Status");
            case 2:
                return QString("Info");
            default:
                break;
            }
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ObjectModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[IdRole] = "id";
    roles[CoordinateRole] = "coordinate";
    roles[StateRole] = "state";

    return roles;
}
