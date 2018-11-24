#include "polygonmodel.h"
#include <QtDebug>

using namespace seye;

PolygonModel::PolygonModel(QObject *parent)
    : QAbstractTableModel(parent), _onCreatePolygon(false)
{
    // Сетап зоны внимания
    _attentionZone = new Polygon;
    _attentionZone->setColor(QColor("#00000000"));
    _attentionZone->setBorderColor(QColor("#FF606060"));
    _attentionZone->setName("Зона внимания");
    _attentionZone->setIsSelected(false);
    QGeoCoordinate coords[] {
        QGeoCoordinate(56.386923585, 85.207064102, 0),
        QGeoCoordinate(56.387148596, 85.206401389, 0),
        QGeoCoordinate(56.388129332, 85.207528001, 0),
        QGeoCoordinate(56.388354335, 85.206913887, 0),
        QGeoCoordinate(56.388907054, 85.207567764, 0),
        QGeoCoordinate(56.388750533, 85.208067008, 0),
        QGeoCoordinate(56.389371724, 85.208769484, 0),
        QGeoCoordinate(56.389643186, 85.207965392, 0),
        QGeoCoordinate(56.389873072, 85.208217223, 0),
        QGeoCoordinate(56.389975786, 85.208128861, 0),
        QGeoCoordinate(56.390318165, 85.208495562, 0),
        QGeoCoordinate(56.390401848, 85.208271057, 0),
        QGeoCoordinate(56.390501322, 85.208166451, 0),
        QGeoCoordinate(56.390773018, 85.20743689, 0),
        QGeoCoordinate(56.390882884, 85.207455666, 0),
        QGeoCoordinate(56.391058074, 85.206937999, 0),
        QGeoCoordinate(56.391130822, 85.207015783, 0),
        QGeoCoordinate(56.39184939, 85.204990715, 0),
        QGeoCoordinate(56.391821182, 85.204743952, 0),
        QGeoCoordinate(56.394145187, 85.205586166, 0),
        QGeoCoordinate(56.394333724, 85.205731005, 0),
        QGeoCoordinate(56.394372321, 85.206624181, 0),
        QGeoCoordinate(56.394406466, 85.20743689, 0),
        QGeoCoordinate(56.394315909, 85.20761928, 0),
        QGeoCoordinate(56.393897268, 85.207726569, 0),
        QGeoCoordinate(56.393759205, 85.208429307, 0),
        QGeoCoordinate(56.393202493, 85.209201784, 0),
        QGeoCoordinate(56.393249999, 85.210274667, 0),
        QGeoCoordinate(56.393223277, 85.210910351, 0),
        QGeoCoordinate(56.393208484, 85.2112758, 0),
        QGeoCoordinate(56.393824579, 85.213416203, 0),
        QGeoCoordinate(56.394262519, 85.214102849, 0),
        QGeoCoordinate(56.394405034, 85.215025528, 0),
        QGeoCoordinate(56.394838515, 85.215709492, 0),
        QGeoCoordinate(56.395240815, 85.215639754, 0),
        QGeoCoordinate(56.395524353, 85.216087683, 0),
        QGeoCoordinate(56.396732706, 85.216074272, 0),
        QGeoCoordinate(56.396445785, 85.21777527, 0),
        QGeoCoordinate(56.396886665, 85.219328269, 0),
        QGeoCoordinate(56.397850895, 85.221162482, 0),
        QGeoCoordinate(56.396422643, 85.223416546, 0),
        QGeoCoordinate(56.394059011, 85.228881386, 0),
        QGeoCoordinate(56.391679605, 85.226311092, 0),
        QGeoCoordinate(56.389634009, 85.231144299, 0),
        QGeoCoordinate(56.388854763, 85.231207568, 0),
        QGeoCoordinate(56.38784347, 85.229791929, 0),
        QGeoCoordinate(56.385925881, 85.224991411, 0),
        QGeoCoordinate(56.385662467, 85.223072422, 0),
        QGeoCoordinate(56.385254194, 85.218595403, 0),
        QGeoCoordinate(56.383831346, 85.216918772, 0),
        QGeoCoordinate(56.382341268, 85.215539964, 0),
        QGeoCoordinate(56.381516814, 85.215374507, 0),
        QGeoCoordinate(56.380454159, 85.21377509, 0),
        QGeoCoordinate(56.379098314, 85.210521103, 0),
        QGeoCoordinate(56.372984199, 85.203660154, 0),
        QGeoCoordinate(56.369294488, 85.198178013, 0),
        QGeoCoordinate(56.368084869, 85.194968147, 0),
        QGeoCoordinate(56.368555281, 85.19289442, 0),
        QGeoCoordinate(56.369331143, 85.190710388, 0),
        QGeoCoordinate(56.371249346, 85.192916481, 0),
        QGeoCoordinate(56.37469454, 85.195817493, 0),
        QGeoCoordinate(56.375189303, 85.19408571, 0),
        QGeoCoordinate(56.375409195, 85.194262198, 0),
        QGeoCoordinate(56.378194391, 85.199523729, 0),
        QGeoCoordinate(56.380649592, 85.202204132, 0),
        QGeoCoordinate(56.381303064, 85.202612259, 0),
        QGeoCoordinate(56.383342802, 85.203472636, 0),
        QGeoCoordinate(56.38461911, 85.204255799, 0),
        QGeoCoordinate(56.385535094, 85.205116175, 0),
        QGeoCoordinate(56.386188482, 85.205855216, 0),
        QGeoCoordinate(56.386676989, 85.206572196, 0),
        QGeoCoordinate(56.386664777, 85.20667147, 0),
        QGeoCoordinate(56.386902922, 85.207046506, 0)
    };
    for (int i = 0; i < 73; i++)
    {
        _attentionZone->addCoordinate(coords[i]);
    }

    _polygons.append(_attentionZone);

    _attentionZone->toString();
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
    // Это число столбцов:  имя, цвет, цвет_рамки, информация о зоне
    return 4;
}

QVariant PolygonModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _polygons.count())
        return QVariant();

    Polygon* poly = _polygons[index.row()];

    switch (role) {
    case Qt::DisplayRole: {
//        if (index.column() == 0) return QString::number(poly->id());
        if (index.column() == 0) return QString(poly->name());
        return QVariant();
    }

    case Qt::DecorationRole: {
        if (index.column() == 1) return poly->color();
        if (index.column() == 2) return poly->borderColor();
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
                return QString("Name");
            case 1:
                return QString("Color");
            case 2:
                return QString("Border");
            case 3:
                return QString("Info");
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
        case 0: {
            poly->setName(strValue);
            changedRole = NameRole;
            break;
        }
        case 1: {
            QColor color(strValue);
            poly->setColor(color);
            changedRole = ColorRole;
            break;
        }
        case 2:{
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

    if (index.row() == 0)
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

Polygon *PolygonModel::attentionZone()
{
    return _attentionZone;
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
