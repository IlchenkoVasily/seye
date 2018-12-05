#include "newuser.h"
#include "ui_newuser.h"

#include "mainwindow.h"
#include <QMessageBox>

NewUser::NewUser(QWidget *parent, QObject *object, QString currentRole, QStandardItemModel *modelTab) :
    QDialog(parent),
    ui(new Ui::NewUser)
{
    ui->setupUi(this);

    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    model = modelTab;
    dblink = qobject_cast<MainWindow*>(object)->database();
    if (currentRole == "admin") ui->comboBox->insertItem(3, "admin");
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
        QStandardItem *item;
        int i = model->rowCount();

        user.id = dblink->add(user, password);
        if (user.id == 0) QMessageBox::warning(this, "Ошибка", "Не удалось создать пользователя");
        else
        {
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
