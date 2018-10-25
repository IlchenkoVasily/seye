/************************************************
** ObjectsModel -- класс-модель, для передачи
** данных об объекте в представление
************************************************/

#ifndef OBJECTSMODEL_H
#define OBJECTSMODEL_H

// Qt includes
#include <QAbstractListModel>
#include <QTimer>

// other includes
#include "object.h"

namespace seye
{
    class ObjectModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        enum ObjectRoles {
            IdRole = Qt::UserRole + 1,
            CoordinateRole
        };

        explicit ObjectModel(QObject* parent = nullptr);
        ~ObjectModel() override;

        // form QAbstractListModel
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        // own
        void addObject(Object* newObj);

    // need for test. delete it after
    public slots:
        void update();

    protected:
        QHash<int, QByteArray> roleNames() const override;

    private:
        QList<Object*> _objects;
        QTimer* timer;  // for test
    };
}

#endif // OBJECTSMODEL_H
