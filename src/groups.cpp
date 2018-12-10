#include "groups.h"
#include "ui_groups.h"
#include "mainwindow.h"
#include "adddevice.h"

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
    ui->tableView->hideColumn(3);
    // change resize mode
    auto header = ui->tableView->horizontalHeader();
    header->setSectionsClickable(false);
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    header->setSectionResizeMode(0, QHeaderView::Stretch);

    connect(groupModel, SIGNAL(itemChanged(QStandardItem*)),
            this, SLOT(groupNameChanged(QStandardItem*)));
}

Groups::~Groups()
{
    delete ui;
}

void Groups::groupNameChanged(QStandardItem* group)
{
    seye::Group gr;
    gr.id = group->accessibleDescription().toInt();
    gr.name = group->text();

    if (db->update(QList<seye::Group>() << gr))
        return;

    // if cant change group name
    int idx = group->index().row();
    group->setText(groups[idx].name);

    QMessageBox::critical(this, "Изменение имени группы", "Не удалось сохранить изменения\n"
                          "Попробуйте позже.");
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
    groups.append(group);
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
    message += "Вы уверены, что хотите удалить " + name + " группу?";

    QMessageBox::StandardButton reply = QMessageBox::warning(this,
        "Удаление группы", message, QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        int i = 0;

        //
        for (i = 0; i < groups.length(); i++)
        {
            if (groups[i].name == name)
            {
                break;
            }
        }

        if (db->drop(groups[i]))
        {
            // drop from list
            groups.removeAt(i);
            // drop from model
            groupModel->removeRows(idx[0].row(), 1);
        }
        else
        {
            QMessageBox::critical(this, "Удаление группы",
                                  "Не удалось удалить группу.\n"
                                  "Попробуйте в другой раз.");
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
            seye::Object object(device, 0, 0);
            auto objIdx = objList.indexOf(object);

            // Получаем позывной устройства
            QString callSign = object.name();
            // Чекаем, если паспорт не связан
            if (callSign.isEmpty())
                callSign = device;
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
    // то перемещаем его выделение на группу
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
    // if group not selected
    if (!ui->treeView->selectionModel()->hasSelection()) return;

    ui->treeView->setEnabled(false);
    ui->tableView->setEnabled(false);

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

        if (object_in_group->accessibleDescription() == object_id)
        {
            if (db->dropReference(group_item->accessibleDescription().toInt(), object_id))
                group_item->removeRow(i);

            ui->treeView->setEnabled(true);
            ui->tableView->setEnabled(true);

            return;
        }
    }

    // We will be here if object not in group
    auto new_object = new QStandardItem(object_name);
    new_object->setAccessibleDescription(object_id);

    seye::Group gr;
    gr.id = group_item->accessibleDescription().toInt();
    gr.devices.append(object_id);

    if (db->reference(gr))
        group_item->appendRow(new_object);

    ui->treeView->setEnabled(true);
    ui->tableView->setEnabled(true);
}

void Groups::on_buttonBox_accepted()
{
    this->close();
}

void Groups::on_lineEdit_textChanged(const QString &arg1)
{
    objectProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    objectProxy->setFilterRegExp(arg1);
//    on_treeView_clicked(ui->treeView->currentIndex());
}

void Groups::on_pushButton_2_clicked()
{
    AddDevice temp(this->parentWidget());
    temp.setModal(true);
    temp.exec();
}
