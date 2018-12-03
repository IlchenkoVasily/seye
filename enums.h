#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>
#include <QQmlEngine>

namespace seye
{
    Q_NAMESPACE

    /*
        Статусы расположены в данном
        порядке для удобства сортировки
        в прокси моделе по статусу.
     */
    enum State {
        Offline = 0,    // Отключенный девайс
        OutOfAttention, // За зоной внимания
        Destroyed,      // Поломанный
        Lost,           // Утерянный
        New,            // Новый
        Allowed,        // Разрешённый
        Intruder        // Нарушитель
    };
    Q_ENUM_NS(State)

    /*
        Перечисление ролей для
        последующей удобной установки
        иконки во вьюхе.
     */
    enum Role {
        Worker,
        Car,
        FuelCar,
        Pilot,
        Security
    };
    Q_ENUM_NS(Role)
}

#endif // ENUMS_H
