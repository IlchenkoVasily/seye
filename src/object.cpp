#include "object.h"

using namespace seye;

Object::Object(QGeoCoordinate coord)
    : _currentCoordinate(coord)
{}

Object::Object(const Object & obj)
    : _id(obj._id), _currentCoordinate(obj._currentCoordinate)
{}

Object::Object(int id, double alt, double longt)
    : _id(id), _currentCoordinate(QGeoCoordinate(alt, longt))
{}

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

bool Object::operator==(Object another)
{
    return this->_id == another._id;
}

Object &Object::operator=(const Object &obj)
{
    if (this != &obj)
    {
        _id = obj._id;
        _currentCoordinate = QGeoCoordinate(obj._currentCoordinate);
    }

    return *this;
}
