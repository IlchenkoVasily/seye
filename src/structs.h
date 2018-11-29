#ifndef SEYESTRUCT_H
#define SEYESTRUCT_H

#include <QDate>

namespace seye
{
    // все первые id, которые тут фигурируют, относятся к взаимодействию с бд, так что не следует их менять без участия функций dbservice
    // так же это означает, что ориентироваться на них в качестве индекса QList тоже не следует
    struct Passport
    {
        qint64 id = 0;
        QString firstName;
        QString lastName;
        QString callSign = "Позывной"; // максимум 32 символа, уникально
        QDate birthday = QDate::currentDate();
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
        QString toString(const QString& idDevice) const // для попапа
        {
            if (idDevice == device)
            {
                QString string = firstName + " "
                        + lastName + ", "
                        + birthday.toString();
                return string;
            }
            else return "У этого пасспорта не такой девайс";
        }
    };

    struct ObjectDev // Object уже есть, целый класс
    {
        QString id = "FFFFFFFFFFFFFFFF"; // 16 обязательных символов, уникально
        qint16 role; // по сути номер иконки, может быть пустым
        qint16 speedLimit = 25; // для проверки валидности координат
        // велечина, на которую за секунду может измениться расстояние в метрах
        QString link = "Рупор"; // способ связи, максимум 32 символа
        QString toString() const
        {
            QString string = id + ", "
                    + QString::number(role) + ", "
                    + QString::number(speedLimit) + ", "
                    + link;
            return string;
        }
    };
    
    struct Zone
    {
        qint32 id = 0;
        QString name;
        QString polygon;
        QString color = "# и 8знач";
        QString lineColor = "# и 6зн";
        QString toString() const
        {
            QString string = QString::number(id) + ", "
                    + name + ", "
                    + polygon + ", "
                    + color + ", "
                    + lineColor;
            return string;
        }
    };

    struct Access
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

    struct Group
    {
        qint64 id = 0;
        QString name; // должно быть уникальным
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
        QString name;
        QString role; // возможные значения: operator, supervisor, admin
        QString toString() const
        {
            QString string = QString::number(id) + ", "
                    + name + ", "
                    + role;
            return string;
        }
        QString toString(const QString& roleName) const
        {
            if (roleName == role) return role;
            else return "У этого пользователя не такая роль";
        }
    };
}

#endif // SEYESTRUCT_H
