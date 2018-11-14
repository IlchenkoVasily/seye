/************************************************
** ObjectsModel -- класс-модель, для передачи
** данных об объекте в представление
************************************************/

#ifndef OBJECTSMODEL_H
#define OBJECTSMODEL_H

// Qt includes
#include <QAbstractTableModel>
#include <QTimer>

// other includes
#include "object.h"
#include "enums.h"

namespace seye
{
    class ObjectModel : public QAbstractTableModel
    {
        Q_OBJECT

    public:
        enum ObjectRoles {
            IdRole = Qt::UserRole + 1,
            CoordinateRole,
            StateRole
        };

        explicit ObjectModel(QObject* parent = nullptr);
        ~ObjectModel() override;

        // form QAbstractListModel
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        // own
        void addObject(Object& newObj);

        const QList<Object>& toList() const;

    protected:
        QHash<int, QByteArray> roleNames() const override;

    private:
        QList<Object> _objects;
    };
}

#endif // OBJECTSMODEL_H
