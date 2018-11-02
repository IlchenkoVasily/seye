/************************************************
** Класс IConnector сделан на основе класса
** Connector, созданным Васиным Василием.
** Структура Pak взята из реализации Ильи Чащина.
************************************************/

#ifndef ICONNECTOR_H
#define ICONNECTOR_H

#include <QList>

namespace seye {

    struct Pak {
        Pak(int id, double x, double y, bool v = true)
        {
            devId = id;
            latitude = x;
            longitude = y;
            valid = v;
        }

        int devId;
        double latitude;
        double longitude;
        bool valid;
    };

    class IConnector
    {
    public:
        virtual ~IConnector();

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
        virtual void complete(QList<Pak>*) = 0;
    };
}

Q_DECLARE_INTERFACE(seye::IConnector, "seye.IConnector/1.0")

#endif // ICONNECTOR_H
