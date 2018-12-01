#ifndef SEYESTRUCT_H
#define SEYESTRUCT_H

#include <QDate>

namespace seye
{
    struct passport
    {
        qint64 id = 0; // ни в коем случае нигде его не меняем
        QString firstName = "Имя";
        QString lastName = "Фамилия";
        QString callSign = "Позывной"; // 32 символа, уникально
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
    };

    struct object
    {
        QString id = "FFFFFFFFFFFFFFFF"; // 16 обязательных символов, уникально
        qint16 role; // по сути номер иконки, может быть пустым
        qint16 speedLimit = 25; // для проверки валидности координат
        // велечина, на которую за секунду может измениться расстояние в метрах
        QString link = "Рупор"; // способ связи, 32 символа
        QString toString() const
        {
            QString string = id + ", "
                    + QString::number(role) + ", "
                    + QString::number(speedLimit) + ", "
                    + link;
            return string;
        }
    };
    
    struct zone
    {
        qint32 id = 0; // ни в коем случае нигде его не меняем
        QString name = "Имя";
        QString polygon = "Координаты";
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

    struct access
    {
        qint64 id = 0; // ни в коем случае нигде его не меняем
        QDateTime start = QDateTime::currentDateTime();
        QDateTime end = QDateTime::currentDateTime();
        QString priority = "Обычное"; // правило может быть: Обычное, Приоритетное, Неизменяемое
        QString name = "Имя";
        qint64 group = 0; // id группы
        qint32 zone = 0; // id зоны
        QString toString() const
        {
            QString string = QString::number(id) + ", "
                    + start.toString() + ", "
                    + end.toString() + ", "
                    + name + ", "
                    + QString::number(group) + ", "
                    + QString::number(zone);
            return string;
        }
    };

    struct group
    {
        qint64 id = 0; // ни в коем случае нигде его не меняем
        QString name = "Имя";
        QList<QString> devices; // список id объектов, состоящих в группе
        QString toString() const
        {
            QString string = QString::number(id) + ", "
                    + name + ":";
            for(int i = 0; i < devices.size(); ++i) string += " " + devices[i];
            return string;
        }
    };
}

#endif // SEYESTRUCT_H
