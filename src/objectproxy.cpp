#include "objectproxy.h"
#include "objectsmodel.h"

#include <QDebug>

using namespace seye;

ObjectProxy::ObjectProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{}

void ObjectProxy::objectSelected(const QModelIndex& index)
{
    auto correctIdx = mapToSource(index);

    auto objModel = (ObjectModel*)sourceModel();
    objModel->objectSelected(correctIdx);
}

bool ObjectProxy::filterAcceptsRow(int sourceRow, const QModelIndex &parent) const
{
    auto index = sourceModel()->index(sourceRow, 0, parent);
    auto data = sourceModel()->data(index, ObjectModel::StateRole);

    if (data.toInt() < State::OutOfAttention)
        return false;
    else
        return true;
}

bool ObjectProxy::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    // Получаем из модели статусы
    QVariant leftStatus = sourceModel()->data(left, ObjectModel::StateRole);
    QVariant rightStatus = sourceModel()->data(right, ObjectModel::StateRole);

    // Сравниваем
    if (leftStatus.toInt() < rightStatus.toInt())
        return true;
    else
        return false;
}
