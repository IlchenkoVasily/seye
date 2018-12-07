#include "newuser.h"
#include "ui_newuser.h"

#include <QMessageBox>

NewUser::NewUser(const QString& currentRole, QStandardItemModel *modelTab, seye::DBService *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewUser)
{
    ui->setupUi(this);

    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    model = modelTab;
    dblink = db;
    if (currentRole == "admin") ui->comboBox->insertItem(2, "admin");
}

NewUser::~NewUser()
{
    delete ui;
}

void NewUser::on_buttonBox_accepted()
{
    seye::User user;
    user.name = ui->lineEditName->text();
    QString password = ui->lineEditPassword->text();
    user.role = ui->comboBox->currentText();
    if (password == ui->lineEditCheckPassword->text())
    {
        user.id = dblink->add(user, password);
        if (user.id == 0) QMessageBox::warning(this, "Неудача", "Возможно такой пользователь уже существует или использовались недопустимые символы");
        else
        {
            QStandardItem *item;
            int i = model->rowCount();

            item = new QStandardItem(QString::number(user.id));
            model->setItem(i, 0, item);
            item = new QStandardItem(QString(user.name));
            model->setItem(i, 1, item);
            item = new QStandardItem(QString(user.role));
            model->setItem(i, 2, item);

            accept();
        }
    }
    else QMessageBox::warning(this, "Ошибка", "Проверьте пароль еще раз");
}
