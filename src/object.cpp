#include "object.h"

using namespace seye;

Object::Object(QGeoCoordinate coord)
    : _currentCoordinate(coord)
{

}

Object::Object(int id, double alt, double longt)
    : _id(id), _currentCoordinate(QGeoCoordinate(alt, longt))
{
}

int Object::id() const
{
    return _id;
}

void Object::setId(int newId)
{
    _id = newId;

    emit idChanged();
}

QGeoCoordinate Object::coordinate() const
{
    return _currentCoordinate;
}

void Object::setCoordinate(QGeoCoordinate coord)
{
    _currentCoordinate = coord;

    emit coordinateChanged();
}
