#ifndef OBJECT_H
#define OBJECT_H

#include <QObject>
#include <QGeoCoordinate>
#include <QColor>

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

        Object(int id, double latitude, double longitude);
        Object(int id, QGeoCoordinate coord);
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

        //
        bool operator==(Object);
        Object &operator=(const Object&);

    signals:
        void idChanged();
        void coordinateChanged();
        void stateChanged(State);

    private:
        int _id; // ?
        QGeoCoordinate _currentCoordinate;
        State _state;
    };
}

#endif // OBJECT_H
