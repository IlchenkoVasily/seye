#include "polygonmodel.h"

#include <QtDebug>

using namespace seye;

PolygonModel::PolygonModel(QObject *parent)
    : QAbstractTableModel(parent), _onCreatePolygon(false)
{
}

PolygonModel::~PolygonModel()
{
    for (int i = 0; i < _polygons.count(); i++)
    {
        delete _polygons[i];
    }

    _polygons.clear();
}

int PolygonModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _polygons.count();
}

int PolygonModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    // Здесь возвращается число 4.
    // Это число столбцов: айди, имя, цвет, цвет_рамки
    return 4;
}

QVariant PolygonModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _polygons.count())
        return QVariant();

    Polygon* poly = _polygons[index.row()];

    switch (role) {
    case Qt::DisplayRole: {
        if (index.column() == 0) return QString::number(poly->id());
        if (index.column() == 1) return QString(poly->name());
        return QVariant();
    }

    case Qt::DecorationRole: {
        if (index.column() == 2) return poly->color();
        if (index.column() == 3) return poly->borderColor();
        return QVariant();
    }

    case PathRole: {
        QVariantList path;

        for (auto coord: poly->path())
        {
            path.append(QVariant::fromValue(coord));
        }

        return path;
    }

    case IdRole: {
        return poly->id();
    }

    case ColorRole: {
        return QVariant::fromValue(poly->color());
    }

    case BorderColorRole: {
        return QVariant::fromValue(poly->borderColor());
    }

    case SelectionRole: {
        return poly->isSelected();
    }

    case MapColorRole: {
        return QVariant::fromValue(poly->mapColor());
    }

    case MapBorderColorRole: {
        return QVariant::fromValue(poly->mapBorderColor());
    }

    default:
        return QVariant();
    }
}

QVariant PolygonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section) {
            case 0:
                return QString("ID");
            case 1:
                return QString("Name");
            case 2:
                return QString("Color");
            case 3:
                return QString("Border");
            default:
                break;
            }
        }

        if (orientation == Qt::Vertical)
        {
            return QString::number(section);
        }
    }

    return QVariant();
}

// пока без реализации всвязи с отсутствием необходимости
bool PolygonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int changedRole;

    QString strValue = value.toString();
    if (strValue.isEmpty())
        return false;

    if (data(index, role) != value) {
        auto poly = _polygons[index.row()];
        switch (index.column()) {
        case 1: {
            poly->setName(strValue);
            changedRole = NameRole;
            break;
        }
        case 2: {
            QColor color(strValue);
            poly->setColor(color);
            changedRole = ColorRole;
            break;
        }
        case 3:{
            QColor b_color(strValue);
            poly->setBorderColor(b_color);
            changedRole = BorderColorRole;
            break;
        }
        }

        emit dataChanged(createIndex(index.row(), 0),
                         createIndex(index.row(), 0),
                         QVector<int>() << changedRole);
        return true;
    }
    return false;
}

// пока без реализации всвязи с отсутствием необходимости
Qt::ItemFlags PolygonModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled |
           Qt::ItemIsEditable;
}

void PolygonModel::addPolygon(Polygon* polygon)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _polygons << polygon;
    endInsertRows();
}

void PolygonModel::beginCreatePolygon()
{
    if (_onCreatePolygon)
        delete _tempPolygon;

    _onCreatePolygon = true;
    _tempPolygon = new Polygon;
}

void PolygonModel::addCoordinate(const QGeoCoordinate &coord)
{
    _tempPolygon->addCoordinate(coord);
}

void PolygonModel::endCreatePolygon()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _tempPolygon->setId(newPolyId++);
    _tempPolygon->setName("Polygon #" + QString::number(_tempPolygon->id()));
    _tempPolygon->setColor(QColor(64, 255, 64, 128));
    _tempPolygon->setBorderColor(QColor(0, 100, 0));
    _tempPolygon->setIsSelected(false);
    _polygons.append(_tempPolygon);
    endInsertRows();

    _tempPolygon = nullptr;
    _onCreatePolygon = false;

}

void PolygonModel::cancelCreatePolygon()
{
    delete _tempPolygon;
    _tempPolygon = nullptr;
    _onCreatePolygon = false;
}

const QList<Polygon*>& PolygonModel::toList() const
{
    return _polygons;
}

void PolygonModel::onPolygonSelected(const QItemSelection &selected,
                                     const QItemSelection &deselected)
{
    // Снимаем выделение с полигонов, которые были развыделены
    auto deselectedIdx = deselected.indexes();
    for (auto idx: deselectedIdx)
    {
        auto poly = _polygons[idx.row()];
        poly->setIsSelected(false);
        emit dataChanged(idx, idx, QVector<int>()
                         << MapColorRole
                         << MapBorderColorRole);
    }

    auto selectedIdx = selected.indexes();
    for (auto idx: selectedIdx)
    {
        auto poly = _polygons[idx.row()];
        poly->setIsSelected(true);
        emit dataChanged(idx, idx, QVector<int>()
                         << MapColorRole
                         << MapBorderColorRole);
    }
}

QHash<int, QByteArray> PolygonModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[PathRole] = "path";
    roles[IdRole] = "role";
    roles[ColorRole] = "color";
    roles[BorderColorRole] = "borderColor";
    roles[NameRole] = "name";
    roles[SelectionRole] = "isSelected";
    roles[MapColorRole] = "mapColor";
    roles[MapBorderColorRole] = "mapBorderColor";

    return roles;
}
