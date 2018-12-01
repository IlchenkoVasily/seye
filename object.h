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
        Q_PROPERTY(Role role READ role WRITE setRole NOTIFY roleChanged)
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

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

        Role role() const;
        void setRole(Role newRole);

        QString name() const;
        void setName(QString name);

    signals:
        void idChanged();
        void coordinateChanged();
        void stateChanged(State);
        void roleChanged(Role);
        void nameChanged(QString name);

    private:
        // Данные из бд
        int _id;            // айди
        double _speedLimit; // Предельная скорость
        // Время последнего изменения координат
        // и время последнего удачного изменения.
        QTime _lastCheck;
        QTime _lastGoodCheck;
        // Текущая позиция объекта и предыдущая.
        QGeoCoordinate _currentCoordinate;
        QGeoCoordinate _previousCoordinate;
        // Статус объекта.
        State _state;
        // Роль объекта.
        Role _role;
        // Позывной объекта
        QString _name;

        /*
            Подсчитывает дистанцию, которую
            объект мог пройти за время с
            последнего обновления.
         */
        double scoreMaxDistance();
    };
}

#endif // OBJECT_H
