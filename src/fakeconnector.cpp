#include "fakeconnector.h"

using namespace seye;

FakeConnector::FakeConnector(QObject* parent)
    : QObject(parent), timer(new QTimer), currentIndex(0)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(startGetting()));
}

FakeConnector::~FakeConnector()
{
    delete timer;
    delete file;
}

void FakeConnector::connectTo(int /* port */) // port -- unusalle
{
    file = new QFile("~/dev/track.txt");
    if ((file->exists())&&(file->open(QIODevice::ReadOnly)))
    {
        while(!file->atEnd())
        {
            list << file->readLine();
        }
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

    auto listPak = new QList<Pak>;

    for (int i = 0; i < deviceCount; i++)
    {
        int id = -1;                // device id
        double _lat = 0, _long = 0; // x, y

        id = list.takeAt(currentIndex++).toInt();
        _lat = list.takeAt(currentIndex++).toDouble();
        _long = list.takeAt(currentIndex++).toDouble();

        Pak temp(id, _lat, _long);

        listPak->append(temp);
    }

    emit complete(listPak);
}

void FakeConnector::start()
{
    timer->start(5000);
}
