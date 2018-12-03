#ifndef GROUPS_H
#define GROUPS_H

#include <QWidget>
#include <QDialog>
#include <QTreeView>
#include <QTableView>

namespace Ui {
class Groups;
}

class Groups : public QDialog
{
    Q_OBJECT

public:
    explicit Groups(QWidget *parent = nullptr);
    ~Groups();

private:
    Ui::Groups *ui;
    QTreeView* groupsView;
};

#endif // GROUPS_H
