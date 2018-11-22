#include "objectsmodel.h"

#define MAX_LOST_TIME 5

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

        // Если вермя между последним нормальным пакетом
        // и последним пакетом > 1 секунды (время между
        // обновлениями), то помечаем пакет потерянным.
        if (_objects[idx].checkTime() > 1.)
        {
            _objects[idx].setState(State::Lost);
        }

        // Если время между последним нормальным пакетом
        // и последним пакетом > максимально возможного времени
        // то помечаем устройство как поломанное.
        if (_objects[idx].checkTime() > MAX_LOST_TIME)
        {
            _objects[idx].setState(State::Destroyed);
            emit noticePushed(_objects[idx].id(), "empty name",
                              State::Destroyed);
        }

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
    emit noticePushed(newObj.id(), "empty name",
                      State::New);
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

    // Здесь возвращается число 3
    // Это число столбцов: айди, цвет, информация о объекте
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

    case RoleRole:
        return QVariant(object.role());

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

bool ObjectModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

}

Qt::ItemFlags ObjectModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled; /*|
           Qt::ItemIsEditable;*/
}

QHash<int, QByteArray> ObjectModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[IdRole] = "id";
    roles[CoordinateRole] = "coordinate";
    roles[StateRole] = "state";
    roles[RoleRole] = "role";

    return roles;
}
