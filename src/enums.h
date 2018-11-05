#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>
#include <QQmlEngine>

namespace seye
{
    Q_NAMESPACE
    enum State {
        Allowed = 228,
        Intruder
    };
    Q_ENUM_NS(State)
}

#endif // ENUMS_H
