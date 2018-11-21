#ifndef OBJECT_H
#define OBJECT_H

#include <QObject>
#include <QGeoCoordinate>
#include <QColor>
#include <QTime>

#include "enums.h"

namespace seye
{
    class Object : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(int id READ id NOTIFY idChanged)
        Q_PROPERTY(QGeoCoordinate coordinate READ coordinate NOTIFY coordinateChanged)
        Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)

    public:

        Object(int id, double latitude, double longitude, double speed = 1.39);
        Object(int id, QGeoCoordinate coord, double speed);
        Object(const Object&);

        // для свойства id
        int id() const;
        void setId(int newId);

        // для свойства coordinate
        QGeoCoordinate coordinate() const;
        void setCoordinate(QGeoCoordinate newCoord);

        // для свойства status
        State state() const;
        void setState(State stat);

        /*
            Возвращает время между последней проверкой
            координаты и последней удачной проверкой.
         */
        double checkTime();

        //
        bool operator==(Object);
        Object &operator=(const Object&);

    signals:
        void idChanged();
        void coordinateChanged();
        void stateChanged(State);

    private:
        // Данные из бд
        int _id;            // айди
        double _speedLimit; // Предельная скорость
        //
        QTime _lastCheck;
        QTime _lastGoodCheck;
        QGeoCoordinate _currentCoordinate;
        QGeoCoordinate _previousCoordinate;
        State _state;

        /*
            Подсчитывает дистанцию, которую
            объект мог пройти за время с
            последнего обновления.
         */
        double scoreMaxDistance();
    };
}

#endif // OBJECT_H
