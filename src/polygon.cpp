#include "polygon.h"
#include <QtDebug>
#include <QGeoCoordinate>

using namespace seye;

Polygon::Polygon()
    : QGeoPolygon()
{

}

Polygon::Polygon(const QList<QGeoCoordinate> &path)
{
    this->setPath(path);
}

Polygon::~Polygon()
{

}

bool Polygon::isSelected()
{
    return _isSelected;
}

void Polygon::setIsSelected(bool newStatus)
{
    _isSelected = newStatus;
}

int Polygon::id()
{
    return _id;
}

void Polygon::setId(int newId)
{
    _id = newId;
}

QString Polygon::name()
{
    return _name;
}

void Polygon::setName(const QString &newName)
{
    _name = newName;
}

QColor Polygon::color()
{
    return _color;
}

void Polygon::setColor(const QColor &newColor)
{
    _color = newColor;
}

QColor Polygon::borderColor()
{
    return _borderColor;
}

void Polygon::setBorderColor(const QColor &newBorderColor)
{
    _borderColor = newBorderColor;
}

QColor Polygon::mapColor()
{
    if (_isSelected)
        return QColor(64, 64, 255, 128);
    else
        return color();
}

QColor Polygon::mapBorderColor()
{
    if (_isSelected)
        return QColor(0, 0, 255); // black
    else
        return borderColor();
}

QString Polygon::toString() const
{
    QStringList full;

    foreach (auto coord, path())
    {
        QString lat = QString::number(coord.latitude()),
                lon = QString::number(coord.longitude());


        full << lat + " " + lon;
    }

    QString complete = full.join("|");
    return complete;
}

void Polygon::fromString(const QString &coordinates)
{
    auto strCoords = coordinates.split("|");

    foreach (auto strCoord, strCoords)
    {
        auto temp = strCoord.split(" ");

        addCoordinate(QGeoCoordinate(
            temp[0].toDouble(),
            temp[1].toDouble())
        );
    }
}

QGeoCoordinate Polygon::center()
{
    double maxLentgh = 0;
    int index1 = 0, index2 = 1;
    auto poly = path();

    for (int i = 0; i < poly.size() - 1; i++)
        for (int j = i + 1; j < poly.size(); j++)
        {
            double check = poly[i].distanceTo(poly[j]);
            if (maxLentgh < check)
            {
                maxLentgh = check;
                index1 = i;
                index2 = j;
            }
        }

    auto first = poly[index1];
    auto second = poly[index2];

    double midLat = (first.latitude() + second.latitude()) / 2.;
    double midLon = (first.longitude() + second.longitude()) / 2.;

    return QGeoCoordinate(midLat, midLon);
}
