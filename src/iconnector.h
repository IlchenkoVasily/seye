/************************************************
** Класс IConnector сделан на основе класса
** Connector, созданным Васиным Василием.
** Структура Pak взята из реализации Ильи Чащина.
************************************************/

#ifndef ICONNECTOR_H
#define ICONNECTOR_H

#include <QList>
#include <QObject>

namespace seye {

    struct Pak;

    typedef QScopedPointer<QList<Pak>> ObjectsPakPtr;

    struct Pak {
        Pak(QString id, double x, double y)
        {
            devId = id;
            latitude = x;
            longitude = y;
        }

        QString devId;
        double latitude;
        double longitude;
    };

    class IConnector : public QObject
    {
        Q_OBJECT

    public:
        explicit IConnector(QObject* parent = nullptr);
//        virtual ~IConnector();

        /*
            Метод подключения к источнику данных
        */
        virtual void connectTo(int) = 0;

        /*
            Метод отключения от источника данных
        */
        virtual void disconnect() = 0;

        /*
            Метод запускает таймер
        */
        virtual void start() = 0;

    public slots:
        /*
            Слот-метод получения данных в виде
            структуры Pak
        */
        virtual void askDevice() = 0;

    signals:
        /*
            Сигнал для уведомления, что все данные
            от источника на данный момент получены.
        */
        void complete(QList<Pak>*);
    };
}

#endif // ICONNECTOR_H
