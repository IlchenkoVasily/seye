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
