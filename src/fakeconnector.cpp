#include "fakeconnector.h"
#include <QtDebug>

using namespace seye;

FakeConnector::FakeConnector(QObject* parent)
    : IConnector(parent), timer(new QTimer(this)), currentIndex(0)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(askDevice()));
}

FakeConnector::~FakeConnector()
{
    delete file;
}

void FakeConnector::connectTo(int /* port */) // port -- unusalle
{
    file = new QFile(":/track.txt");
    if ((file->exists())&&(file->open(QIODevice::ReadOnly)))
    {
        while(!file->atEnd())
        {
            list << file->readLine();
        }
    }
    else
    {
        qDebug() << "Can't open track file";
    }

    file->close();
}

void FakeConnector::disconnect()
{
    // pass
}

void FakeConnector::askDevice()
{
    int deviceCount = 3; // !!!

    if (currentIndex >= 90)
        currentIndex = 0;

    ObjectsPakPtr listPak(new QList<Pak>);

    for (int i = 0; i < deviceCount; i++)
    {
        int id = -1;                // device id
        double _lat = 0, _long = 0; // x, y

        id = list[currentIndex++].toInt();
        _lat = list[currentIndex++].toDouble();
        _long = list[currentIndex++].toDouble();

        listPak->append(Pak(id, _lat, _long));
    }

    emit complete(listPak);
}

void FakeConnector::start()
{
    timer->start(5000);
}
