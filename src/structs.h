#ifndef SEYESTRUCT_H
#define SEYESTRUCT_H

#include <QDate>

namespace seye
{
    // все id, которые тут фигурируют, относятся к взаимодействию с бд, так что не следует их менять без участия функций dbservice
    // так же это означает, что ориентироваться на них в качестве индекса QList тоже не следует

    struct Passport
    {
        qint64 id = 0;
        QString firstName;
        QString lastName;
        QString callSign; // максимум 32 символа, уникально, обязательное
        QDate birthday;
        QString device = nullptr; // либо пустое, либо уникальное значение из 16 символов
        // если задаваемое значение отсутствует в таблице объектов, то будет ошибка
        QString toString() const
        {
            QString string = QString::number(id) + ", "
                    + firstName + ", "
                    + lastName + ", "
                    + callSign + ", "
                    + birthday.toString();
            return string;
        }
        QString toStringForPopUp() const
        {
            QString string = firstName + " "
                    + lastName + ", "
                    + birthday.toString();
            return string;
        }
    };

    struct ObjectDev // Object уже есть, целый класс ):
    {
        QString id; // 16 обязательных символов, уникально
        qint16 role; // по сути номер иконки, может быть пустым
        qint16 speedLimit = 25; // для проверки валидности координат, обязательное
        // велечина, на которую за секунду может измениться расстояние в метрах
        QString link; // способ связи, максимум 32 символа, обязательное
        QString toString() const
        {
            QString string = id + ", "
                    + QString::number(role) + ", "
                    + QString::number(speedLimit) + ", "
                    + link;
            return string;
        }
    };
    
//    struct Zone
//    {
//        qint32 id = 0;
//        QString name;
//        QString polygon; // обязательное
//        QString color = "# и 8знач"; // -||-
//        QString lineColor = "# и 6зн"; // -||-
//        QString toString() const
//        {
//            QString string = QString::number(id) + ", "
//                    + name + ", "
//                    + polygon + ", "
//                    + color + ", "
//                    + lineColor;
//            return string;
//        }
//    };

    struct Access
    {
        qint64 id = 0;
        QDateTime start; // обязательное
        QDateTime end; // -||-
        QString priority = "Обычное"; // обязательное, возможные значения: Обычное, Приоритетное, Неизменяемое
        QString name;
        qint64 group = 0; // id группы, обязательное
        qint32 zone = 0; // id зоны, -||-
        // если какого-либо из айди не существует в соответствующей таблице, то будет ошибка
        QString toString() const
        {
            QString string = QString::number(id) + ", "
                    + start.toString() + ", "
                    + end.toString() + ", "
                    + priority + ", "
                    + name + ", "
                    + QString::number(group) + ", "
                    + QString::number(zone);
            return string;
        }
    };

    struct Group
    {
        qint64 id = 0;
        QString name; // уникальное, обязательное
        QList<QString> devices; // список id объектов, состоящих в группе
        QString toString() const
        {
            QString string = QString::number(id) + ", "
                    + name + ":";
            for(int i = 0; i < devices.size(); ++i) string += " " + devices[i];
            return string;
        }
    };

    struct User
    {
        qint32 id;
        QString name; // ну юзер без имени не юзер, уникальное
        QString role; // обязательное, возможные значения: operator, supervisor, admin
        QString toString() const
        {
            QString string = QString::number(id) + ", "
                    + name + ", "
                    + role;
            return string;
        }
        QString toStringRole() const
        {
            return role;
        }
    };

    struct AccessLine
    {
        qint64 id = 0;
        QDateTime start = QDateTime::fromString("2018-11-30 03:09:02", "yyyy-MM-dd hh:mm:ss");
        QDateTime end = QDateTime::fromString("2018-11-30 03:09:02", "yyyy-MM-dd hh:mm:ss");
        QString priority = "Обычное"; // возможные значения: Обычное, Приоритетное, Неизменяемое
        QString name;
        qint64 group = 0; // id группы
        qint32 zone = 0; // id зоны
        QString toString() const
        {
            QString string = QString::number(id) + ", "
                    + start.toString() + ", "
                    + end.toString() + ", "
                    + priority + ", "
                    + name + ", "
                    + QString::number(group) + ", "
                    + QString::number(zone);
            return string;
        }
    };
}

#endif // SEYESTRUCT_H
