#include "users.h"
#include "ui_users.h"

#include "newuser.h"
#include "changepassword.h"
#include <QMessageBox>

#include "QStandardItemModel"

Users::Users(QWidget *parent, seye::DBService *db, QString role) :
    QDialog(parent),
    ui(new Ui::Users)
{
    ui->setupUi(this);

    currentRole = role;
    model = new QStandardItemModel;

    QStringList header;
    header << "id" << "Пользователь" << "Роль";
    model->setHorizontalHeaderLabels(header);

    dblink = db;
    QList<seye::User> users = dblink->getAllUsers();

    QStandardItem *item;
    for(int i = 0; i < users.size(); ++i)
    {
        item = new QStandardItem(QString::number(users[i].id));
        model->setItem(i, 0, item);

        item = new QStandardItem(QString(users[i].name));
        model->setItem(i, 1, item);

        item = new QStandardItem(QString(users[i].role));
        model->setItem(i, 2, item);
    }
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->hideSection(0);
    ui->tableView->resizeRowsToContents();
}

Users::~Users()
{
    delete ui;
}

void Users::on_pushButton_clicked()
{
    NewUser creat(this, parent(), currentRole, model);
    creat.setModal(true);
    creat.exec();
}

void Users::on_pushButton_3_clicked()
{
    if (ui->tableView->selectionModel()->hasSelection())
    {
        int row = ui->tableView->currentIndex().row();
        ChangePassword change(this, parent(), currentRole,
                              model->item(row, 1)->text(),
                              model->item(row, 2)->text());
        change.setModal(true);
        change.exec();
    }
    else QMessageBox::warning(this, "Ошибка", "Выберите пользователя");
}

void Users::on_pushButtonDelete_clicked()
{
    seye::User user;
    int row = ui->tableView->currentIndex().row();
    user.name = model->item(row, 1)->text();
    user.role = model->item(row, 2)->text();
    if (currentRole == "supervisor" && user.role == "operator" || currentRole == "admin" && user.role != "admin")
        if (dblink->drop(user)) model->removeRow(row); else;
    else QMessageBox::warning(this, "Ошибка", "Недостаточно прав");
}
