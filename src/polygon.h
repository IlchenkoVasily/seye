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

    Q_PROPERTY(bool isSelected READ isSelected WRITE setIsSelected NOTIFY isSelectedChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    // Данные цвета для вьюх и для бд.
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    // Данные цвета только для карты и меняются взависимости от выделения
    Q_PROPERTY(QColor mapColor READ mapColor NOTIFY mapColorChanged)
    Q_PROPERTY(QColor mapBorderColorColor READ mapBorderColor NOTIFY mapBorderColorChanged)

public:
    Polygon();
    Polygon(const QList<QGeoCoordinate> &path);
    virtual ~Polygon();

    bool isSelected();
    void setIsSelected(bool newStatus);
    int id();
    void setId(int newId);

    QString name();
    void setName(const QString& newName);

    QColor color();
    void setColor(const QColor& newColor);

    QColor borderColor();
    void setBorderColor(const QColor& newBorderColor);

    QColor mapColor();
    QColor mapBorderColor();

    QString toString() const;
    void fromString(const QString& coordinates);

    QGeoCoordinate center();

signals:
    void isSelectedChanged(bool);
    void idChanged(int);
    void nameChanged(QString);
    void colorChanged(QColor);
    void borderColorChanged(QColor);
    void mapColorChanged(QColor);
    void mapBorderColorChanged(QColor);

private:
    bool _isSelected;
    int _id;
    QString _name;
    QColor _borderColor;
    QColor _color;
};

}

#endif // POLYGON_H
