#include "object.h"
#include <QtDebug>

using namespace seye;

Object::Object(QString id, QGeoCoordinate coord, double speed)
    : _id(id), _speedLimit(speed), _currentCoordinate(coord)
{}

Object::Object(const Object & obj)
    : _id(obj._id), _speedLimit(obj._speedLimit), _lastCheck(obj._lastCheck)
    , _currentCoordinate(obj._currentCoordinate), _previousCoordinate(obj._previousCoordinate)
    ,  _state(obj._state), _role(obj._role), _name(obj._name), _link(obj._link)

{}

Object::Object(const ObjectDev& obj, QString name)
    : _id(obj.id), _speedLimit(obj.speedLimit),
      _state(State::Offline), _role(Role(obj.role))
{
    _name = name;
    _link = obj.link;
}

Object::Object(QString id, double alt, double longt, double speed)
    : Object(id, QGeoCoordinate(alt, longt), speed)
{}

QString Object::id() const
{
    return _id;
}

void Object::setId(QString newId)
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
    auto maxDistance = scoreMaxDistance();
    if (distance <= maxDistance)
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
        _speedLimit = obj._speedLimit;
        _lastCheck = obj._lastCheck;
        _lastGoodCheck = obj._lastGoodCheck;
        _currentCoordinate = QGeoCoordinate(obj._currentCoordinate);
        _previousCoordinate = QGeoCoordinate(obj._previousCoordinate);
        _state = obj._state;
        _role = obj._role;
        _name = obj._name;
        _link = obj._link;
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
    return _name.isEmpty() ? _id : _name;
}

void Object::setName(QString name)
{
    if (_name == name)
        return;

    _name = name;
    emit nameChanged(_name);
}

QString Object::link() const
{
    return _link;
}

void Object::setLink(QString link)
{
    _link = link;
}

double Object::speedLimit() const
{
    return _speedLimit;
}

void Object::setSpeedLibit(double speed)
{
    _speedLimit = speed;
}

double Object::scoreMaxDistance()
{
    return _speedLimit * (_lastCheck.secsTo(QTime::currentTime()));
}
