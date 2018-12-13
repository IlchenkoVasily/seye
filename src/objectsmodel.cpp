#include "objectsmodel.h"
#include <QtDebug>

#include <QColor>

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
        Object& editable = _objects[idx];

        if (newObj.state() > editable.state())
            emit noticePushed(newObj.id(), editable.name(), State::Allowed);

        editable.setCoordinate(newObj.coordinate());
        editable.setState(newObj.state());

        // Если вермя между последним нормальным пакетом
        // и последним пакетом > 1 секунды (время между
        // обновлениями), то помечаем пакет потерянным.
        if (editable.checkTime() > 1.)
        {
            editable.setState(State::Lost);
        }

        // Если время между последним нормальным пакетом
        // и последним пакетом > максимально возможного времени
        // то помечаем устройство как поломанное.
        if (editable.checkTime() > MAX_LOST_TIME)
        {
            editable.setState(State::Destroyed);
            emit noticePushed(editable.id(), editable.name(),
                              State::Destroyed);
        }

        // Проверка на то, является ли объект нарушителем
        if (editable.state() == State::Intruder)
            emit noticePushed(editable.id(), editable.name(),
                              State::Intruder);

        // Проверка на то, находится ли объект за зоной внимания
        if (editable.state() == State::OutOfAttention)
            emit noticePushed(editable.id(), editable.name(),
                              State::OutOfAttention);

        // Сигнал о том, что данные в модели изменены.
        // Индексы наших объектов в моделе, изменённый параметр
        emit dataChanged(index(idx, 0), index(idx, 1),
                         QVector<int>() << CoordinateRole << StateRole);
        return;
    }

    // Вставка нового объекта
    // Модельный индекс, индекс начала вставки и конца вставки
    //
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _objects << newObj;
    endInsertRows();
    emit noticePushed(newObj.id(), newObj.name(),
                      State::New);
}

const QList<Object>& ObjectModel::toList() const
{
    return _objects;
}

void ObjectModel::objectSelected(const QModelIndex& index)
{
    auto obj = _objects[index.row()];
    qDebug() << obj.id() << obj.name();
    auto coordinate = obj.coordinate();

    emit objectCentering(coordinate);
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
        if (index.column() == 0)
        {
            QString returnableName = object.name();
            if (returnableName.isEmpty())
                returnableName = object.id();
            return returnableName;
        }
        if (index.column() == 2) return object.link();
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

    case NameRole:
        return object.name().isEmpty() ? QVariant(object.id()) : QVariant(object.name());

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
                return QString("Имя");
            case 1:
                return QString("Статус");
            case 2:
                return QString("Информация");
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
    roles[NameRole] = "name";

    return roles;
}
