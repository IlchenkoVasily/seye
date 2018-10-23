#include "object.h"

using namespace seye;

Object::Object(double alt, double longt)
    : _id(123), _currentCoordinate(QGeoCoordinate(alt, longt))
{
//    this->;
}

int Object::id() const
{
    return _id;
}

QGeoCoordinate Object::coordinate() const
{
    return _currentCoordinate;
}
