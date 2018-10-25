#ifndef OBJECT_H
#define OBJECT_H

#include <QObject>
#include <QGeoCoordinate>
#include <QColor>

namespace seye
{
    class Object : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
        Q_PROPERTY(QGeoCoordinate coordinate READ coordinate WRITE setCoordinate NOTIFY coordinateChanged)

    public:
        Object(double latitude, double longitude);
        Object(QGeoCoordinate coord);

        // для свойства id
        int id() const;
        void setId(int newId);
        // для свойства coordinate
        QGeoCoordinate coordinate() const;
        void setCoordinate(QGeoCoordinate newCoord);

    signals:
        void idChanged();
        void coordinateChanged();

    private:
        int _id; // ?
        QGeoCoordinate _currentCoordinate;
    };
}


#endif // OBJECT_H
