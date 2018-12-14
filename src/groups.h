#ifndef GROUPS_H
#define GROUPS_H

#include <QWidget>
#include <QDialog>
#include <QTreeView>
#include <QTableView>
#include <QStandardItemModel>

#include "dbservice.h"
#include "objectproxy.h"
#include "objectsmodel.h"

namespace Ui {
class Groups;
}

class Groups : public QDialog
{
    Q_OBJECT

public:
    explicit Groups(QWidget *parent = nullptr);
    ~Groups();

    seye::Group* selectedGroup();

private slots:
    // data changed in model
    void groupNameChanged(QStandardItem*);
    // create group button
    void on_pushButton_3_clicked();
    // delete group button
    void on_pushButton_clicked();
    // tree view click
    void on_treeView_clicked(const QModelIndex &index);
    // table view click
    void on_tableView_clicked(const QModelIndex &index);
    // button save
    void on_buttonBox_accepted();
    // whenever search box changed update proxy
    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    Ui::Groups *ui;
    seye::DBService* db;
    seye::ObjectModel* objectModel;
    seye::ObjectProxy* objectProxy;
    QStandardItemModel* groupModel;
    QList<seye::Group> groups;

    void setupGroupModel();
};

#endif // GROUPS_H
