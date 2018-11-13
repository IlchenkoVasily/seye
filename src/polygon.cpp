#include "polygon.h"

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
