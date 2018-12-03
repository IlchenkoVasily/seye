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
    bool flag = false;

    // Получаем модельный индекс строки
    auto index = sourceModel()->index(sourceRow, 0, parent);
    // Получаем статус
    auto state = sourceModel()->data(index, ObjectModel::StateRole);

    // Проверка на оффлайн
    if (state.toInt() < State::OutOfAttention)
        flag |= true;
    else
        flag |= true;

    // получаем текст
    auto reg = filterRegExp();

    // Получаем строку с именем из модели
    auto name = sourceModel()->data(index, ObjectModel::NameRole).toString();

    // Проверяем
    if (name.contains(reg))
        flag &= true;
    else
        flag &= false;

    // Возвращаем флаг
    return flag;
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
