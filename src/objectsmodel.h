/************************************************
** ObjectsModel -- класс-модель, для передачи
** данных в представление
************************************************/

#ifndef OBJECTSMODEL_H
#define OBJECTSMODEL_H

// Qt includes
#include <QAbstractListModel>

// other includes
#include "object.h"

namespace seye
{
    class ObjectModel : public QAbstractListModel
    {
        Q_OBJECT
//        Q_PROPERTY(QList<Object> data READ data NOTIFY dataChanged)

    public:
        enum ObjectRoles {
            IdRole = Qt::UserRole + 1,
            CoordinateRole
        };

        ObjectModel(QObject* parent = nullptr);

        // form QAbstractListModel
        void addObject(const Object& newObj);
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;



//    signals:
//        void dataChanged();

    protected:
        QHash<int, QByteArray> roleNames() const;

    private:
        QList<Object> _objects;
    };
}

#endif // OBJECTSMODEL_H
