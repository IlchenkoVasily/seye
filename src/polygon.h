#ifndef POLYGON_H
#define POLYGON_H

#include <QGeoPolygon>
#include <QGeoPolygonPrivate>
#include <QColor>
#include <QObject>

namespace seye
{

class Polygon : public QObject, public QGeoPolygon
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)

public:
    Polygon();
    Polygon(const QList<QGeoCoordinate> &path);
//    Polygon(const QGeoPolygon &other);
//    Polygon(const QGeoShape &other);
    virtual ~Polygon();

    int id();
    void setId(int newId);

    QString name();
    void setName(const QString& newName);

    QColor color();
    void setColor(const QColor& newColor);

    QColor borderColor();
    void setBorderColor(const QColor& newBorderColor);

signals:
    void idChanged(int);
    void nameChanged(QString);
    void colorChanged(QColor);
    void borderColorChanged(QColor);

private:
    int _id;
    QString _name;
    QColor _borderColor;
    QColor _color;
};

}

#endif // POLYGON_H
