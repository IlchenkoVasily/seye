#include "polygonmodel.h"

using namespace seye;

PolygonModel::PolygonModel(QObject *parent)
    : QAbstractListModel(parent), _onCreatePolygon(false)
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
    Q_UNUSED(parent)

    return _polygons.count();
}

QVariant PolygonModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _polygons.count())
        return QVariant();

    Polygon* poly = _polygons[index.row()];

    switch (role) {
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

    default:
        return QVariant();
    }
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
    _tempPolygon->setId(228);
    _tempPolygon->setName("Bonjur");
    _tempPolygon->setColor(QColor(64, 255, 64, 128));
    _tempPolygon->setBorderColor(QColor(0, 100, 0));
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

QHash<int, QByteArray> PolygonModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[PathRole] = "path";
    roles[IdRole] = "role";
    roles[ColorRole] = "color";
    roles[BorderColorRole] = "borderColor";
    roles[NameRole] = "name";

    return roles;
}
