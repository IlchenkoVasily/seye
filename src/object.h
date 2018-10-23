#ifndef OBJECT_H
#define OBJECT_H

#include <QObject>
#include <QGeoCoordinate>

namespace seye
{
    class Object
    {
    public:
        Object(double latitude, double longitude);

        int id() const;
        QGeoCoordinate coordinate() const;

    private:
        int _id; // ?
        QGeoCoordinate _currentCoordinate;
    };
}


#endif // OBJECT_H
