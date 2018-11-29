#include "object.h"

using namespace seye;

Object::Object(int id, QGeoCoordinate coord, double speed)
    : _id(id), _speedLimit(speed), _currentCoordinate(coord)
{}

Object::Object(const Object & obj)
    : _id(obj._id), _speedLimit(obj._speedLimit), _lastCheck(obj._lastCheck)
    , _currentCoordinate(obj._currentCoordinate), _state(obj._state)
    , _role(obj._role), _name(obj._name)

{}

Object::Object(int id, double alt, double longt, double speed)
    : Object(id, QGeoCoordinate(alt, longt), speed)
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
    // Сохраняем предыдущую координату
    _previousCoordinate = _currentCoordinate;
    // Ставим новую координату
    _currentCoordinate = coord;

    // Сразу проверяем насколько далеко отдалены
    // координаты друг от друга.
    auto distance = _currentCoordinate.distanceTo(_previousCoordinate);
    if (distance <= scoreMaxDistance())
    {
        _lastGoodCheck = QTime::currentTime();
    }
    _lastCheck = QTime::currentTime();

    emit coordinateChanged();
}

State Object::state() const
{
    return _state;
}

void Object::setState(State stat)
{
    _state = stat;
    emit stateChanged(stat);
}

double Object::checkTime()
{
    return _lastCheck.secsTo(_lastGoodCheck);
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

Role Object::role() const
{
    return _role;
}

void Object::setRole(Role newRole)
{
    _role = newRole;
}

QString Object::name() const
{
    return _name;
}

void Object::setName(QString name)
{
    if (_name == name)
        return;

    _name = name;
    emit nameChanged(_name);
}

double Object::scoreMaxDistance()
{
    return _speedLimit * (_lastCheck.secsTo(QTime::currentTime()));
}