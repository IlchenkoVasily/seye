#ifndef FAKECONNECTOR_H
#define FAKECONNECTOR_H

#include <QObject>
#include <QTimer>
#include "iconnector.h"

namespace seye {

    class FakeConnector : public IConnector
    {
        Q_OBJECT

    public:
        explicit FakeConnector(QObject* parent = nullptr);
        ~FakeConnector() override;

        void connectTo(int port) override;
        void disconnect() override;
        void start() override;

    public slots:
        void askDevice() override;

    signals:
        void complete(ObjectsPakPtr&);

    private:
        QFile* file;
        QTimer* timer;
        QStringList list;
        int currentIndex;
    };

}

#endif // FAKECONNECTOR_H
