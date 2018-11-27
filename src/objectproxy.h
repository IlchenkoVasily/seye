#ifndef OBJECTPROXY_H
#define OBJECTPROXY_H

#include <QSortFilterProxyModel>

namespace seye
{

class ObjectProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    ObjectProxy(QObject* parent = nullptr);

public slots:
    /*
        Метод принимает индекс клика во вью.
        Получает модельный индекс и вызывает
        соответствующий слот у самой моделе.
     */
    void objectSelected(const QModelIndex&);

protected:
    /*
        TODO comment filterAcceptsRow
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex& parent) const override;

    /*
        TODO comment lessThan
     */
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
};

}

#endif // OBJECTPROXY_H
