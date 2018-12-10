#include "groups.h"
#include "ui_groups.h"
#include "mainwindow.h"
#include <QMessageBox>

Groups::Groups(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Groups)
{
    ui->setupUi(this);

    // get database pointer
    db = qobject_cast<MainWindow*>(parent)->database();

    // get object model (proxt) pointer
    objectModel = qobject_cast<MainWindow*>(parent)->getObjectModel();

    // setup object proxy for searching
    objectProxy = new seye::ObjectProxy(this);
    objectProxy->setSourceModel(objectModel);

    // setup group model
    setupGroupModel();

    // set created group model to the tree view
    ui->treeView->setModel(groupModel);

    // Ставим в таблицу модель объектов
    objectProxy->setFilteringState(false);
    objectProxy->setSortingState(false);
    ui->tableView->setModel(objectProxy);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
}

Groups::~Groups()
{
    delete ui;
}

void Groups::on_pushButton_3_clicked()
{
    // Создаём группу
    seye::Group group;
    group.name = "новая группа";
    group.id = db->add(group);

    if (group.id == 0)
        return;

    auto newGroup = new QStandardItem(group.name);
    newGroup->setAccessibleDescription(QString::number(group.id));
    groupModel->setItem(groupModel->rowCount(), 0, newGroup);
}

void Groups::on_pushButton_clicked()
{
    auto selection = ui->treeView->selectionModel();

    if (!selection->hasSelection())
    {
        QMessageBox::information(this, "Удаление групп", "Нечего удалять.");
        return;
    }

    auto idx = selection->selectedRows();
    auto name = groupModel->data(idx[0]).toString();

    QString message;
    message += "Вы уверены, что хотите удалить " + name + " группу";

    QMessageBox::StandardButton reply = QMessageBox::warning(this,
        "Удаление группы", message, QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        int i = 0;
        seye::Group drop;

        for (i = 0; i < groups.length(); i++)
        {
            if (groups[i].name == name)
            {
                break;
            }
        }

        if (db->drop(groups[i]))
        {
            groups.removeAt(i);
            groupModel->removeRows(idx[0].row(), 1);
        }
    }
}

void Groups::setupGroupModel()
{
    // setup model
    groupModel = new QStandardItemModel;

    // получаем список объектов
    auto objList = objectModel->toList();

    // fill tree model for groups
    groups = db->getAllGroups();
    int row = 0;
    foreach (auto gr, groups)
    {
        // Будущая группа в дереве
        auto gr_name = new QStandardItem(QString(gr.name));
        gr_name->setAccessibleDescription(QString::number(gr.id));

        // Наполняем группы объектами (их позывными)
        foreach (auto device, gr.devices)
        {
            auto objIdx = objList.indexOf(seye::Object(device, 0, 0));

            // Получаем с бд позывной по айди устройства
            auto callSign = db->getCallSignFor(device);
            // Создаём
            auto obj = new QStandardItem(callSign);
            // Прячем айди
            obj->setAccessibleDescription(objList[objIdx].id());
            // Отключаем изменения позывного объекта
            obj->setEditable(false);
            // Добавляем
            gr_name->appendRow(obj);
        }

        // Добавляем группу в дерево
        groupModel->setItem(row++, 0, gr_name);
    }

    // СТавим название столбца
    groupModel->setHorizontalHeaderLabels(QStringList() << "Groups");
}

void Groups::on_treeView_clicked(const QModelIndex &index)
{
    auto objList = objectModel->toList();

    // Если тыкнули по конкретному объекту в ДЕРЕВЕ
    if (index.parent().isValid())
    {
        auto selection = ui->treeView->selectionModel();
        selection->select(index.parent(), QItemSelectionModel::ClearAndSelect);
        ui->treeView->setCurrentIndex(index.parent());
    }

    auto current = ui->treeView->currentIndex();
    auto item = groupModel->itemFromIndex(current);
    QList<int> rows;

    for (int i = 0; i < item->rowCount(); i++)
    {
        rows << objList.indexOf(seye::Object(item->child(i)->accessibleDescription(), 0, 0));
    }

    auto tableSelection = ui->tableView->selectionModel();
    tableSelection->clearSelection();
    foreach (int row, rows)
    {
        auto idx = objectModel->index(row, 0);
        tableSelection->select(idx, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void Groups::on_tableView_clicked(const QModelIndex &index)
{
    // get index which was clicked
    auto current = ui->tableView->currentIndex();
    // get name object from model
    auto object_name = objectProxy->data(current, seye::ObjectModel::NameRole).toString();
    auto object_id   = objectProxy->data(current, seye::ObjectModel::IdRole).toString();

    // get index of group which active at this moment
    auto group_current = ui->treeView->currentIndex();
    // get group item from model
    auto group_item = groupModel->itemFromIndex(group_current);

    // check for every child item in group
    for (int i = 0; i < group_item->rowCount(); i++)
    {
        auto object_in_group = group_item->child(i);

        if (object_in_group->text() == object_name)
        {
            group_item->removeRow(i);
            return;
        }
    }

    // We will be here if object not in group
    auto new_object = new QStandardItem(object_name);
    new_object->setAccessibleDescription(object_id);

    group_item->appendRow(new_object);
}

void Groups::on_buttonBox_accepted()
{
    /* Обновление объектов в бд.
     * 1. Получить с модели групп полноценные структуры групп.
     * 2. Получить с модели на каждую группу по девайсу.
     * 3. Наполнить структуры.
     * 4. Вызвать апдейт у бд для каждой группы.
     */

    // create groups list
    QList<seye::Group> groups_list;

    for (int i = 0; i < groupModel->rowCount(); i++)
    {
        // get item from model
        auto group_item = groupModel->item(i);

        // create group variable
        seye::Group group;
        // get name from model item
        group.name = group_item->text();
        // get id
        group.id = group_item->accessibleDescription().toInt();

        // get devices list from item group
        for (int j = 0; j < group_item->rowCount(); j++)
        {
            // get object item
            auto object_item = group_item->child(j);
            // add device id to devices list of the group
            group.devices.append(object_item->accessibleDescription());
        }

        // add group
        groups_list.append(group);
    }

    bool ok = db->update(groups_list);

    if (ok)
        return;
    else
        QMessageBox::warning(this, "Сохранение", "Не удалось произвести сохранение.\n"
                             "Повторите попытку позже.");
}

void Groups::on_lineEdit_textChanged(const QString &arg1)
{
    objectProxy->setFilterRegExp(arg1);
//    on_treeView_clicked(ui->treeView->currentIndex());
}
