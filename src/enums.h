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
        Destroyed,      // Поломанный
        Lost,           // Утерянный
        New,            // Новый
        Allowed,        // Разрешённый
        Intruder        // Нарушитель
    };
    Q_ENUM_NS(State)
}

#endif // ENUMS_H
