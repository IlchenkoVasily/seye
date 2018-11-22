#ifndef SEYESTRUCT_H
#define SEYESTRUCT_H

#include <QString>
#include <QDate>

namespace seye
{
    struct passport
    {
        int id = 0;
        QString callSign = "0"; // позывной
        QString firstName = "0";
        QString lastName = "0";
        QDate birthday = QDate::currentDate();
//        const QString toString();
//        хз, не могу использовать
    };

    struct zone
    {
        int id = 0;
        QString name = "0";
        QString polygon = "0";
        QString color = "0";
        QString lineColor = "0";
        QString lineWidth = "0";
    };

    struct object
    {
        QString id = "0";
        QString role = "0";
        int passport = 0; // номер объекта
        QString speedLimit = "0";
        QString link = "0";// телефон
    };

    struct access
    {
        int id = 0;
        QString name = "0";
        int zone = 0;
        int group = 0;
        QDateTime start = QDateTime::currentDateTime();
        QDateTime end = QDateTime::currentDateTime();
    };

    struct group
    {
        int id = 0;
        QString name = "0";
        QList<QString> objects;
    };

    static const QString toString(passport doc)
    {
        QString string = QString::number(doc.id) + ", "
                + doc.callSign + ", "
                + doc.firstName + ", "
                + doc.lastName + ", "
                + doc.birthday.toString();
        return(string);
    }
    static const QString toString(zone doc)
    {
        QString string = QString::number(doc.id) + ", "
                + doc.name + ", "
                + doc.polygon + ", "
                + doc.color + ", "
                + doc.lineColor + ", "
                + doc.lineWidth;
        return(string);
    }
    static const QString toString(object doc)
    {
        QString string = doc.id + ", "
                + doc.role + ", "
                + QString::number(doc.passport) + ", "
                + doc.speedLimit + ", "
                + doc.link;
        return(string);
    }
}

#endif // SEYESTRUCT_H
