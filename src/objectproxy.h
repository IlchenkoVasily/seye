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

    /*
        Метод изменяет состояние сортировки.
        false - объекты в списке не соритруются.
        true  - объекты в списке соритруются.
     */
    void setSortingState(bool state) { _sortingState = state; }

    /*
        Метод изменяет состояние фильтрации.
        При отключении фильтрации в списке появятся
        оффлайн устройства.
     */
    void setFilteringState(bool state) { _filteringState = state; }

public slots:
    /*
        Метод принимает индекс клика во вью.
        Получает модельный индекс и вызывает
        соответствующий слот у самой моделе.
     */
    void objectSelected(const QModelIndex&);

protected:
    /*
        Возвращает true, если объект должен отображаться
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex& parent) const override;

    /*
        Возвращает true иногда
     */
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;

private:
    bool _sortingState = true;
    bool _filteringState = true;
};

}

#endif // OBJECTPROXY_H
