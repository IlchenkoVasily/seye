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
            StateRole,
            RoleRole,
            NameRole
        };

        explicit ObjectModel(QObject* parent = nullptr);
        ~ObjectModel() override;

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        // Для изменения уже имеющихся данных:
        bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole) override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;

        // own
        void addObject(Object& newObj);

        const QList<Object>& toList() const;

    signals:
        /*
            Сигнал для уведомлений.
            Передаётся: айди, позывной, статус.
        */
        void noticePushed(int, QString, State);

        /*
            Сигнал пересылает координату в гис
            вьюху для центрирования на объекте.
         */
        void objectCentering(const QGeoCoordinate& coordinate);

    public slots:
        /*
            Метод принимает модельный индекс и
            по нему ищет объект. После отправляет
            сигнал для центрирования.
         */
        void objectSelected(const QModelIndex&);

    protected:
        QHash<int, QByteArray> roleNames() const override;

    private:
        QList<Object> _objects;
    };
}

#endif // OBJECTSMODEL_H
