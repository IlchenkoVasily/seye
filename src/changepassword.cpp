#include "changepassword.h"
#include "ui_changepassword.h"

#include "mainwindow.h"
#include <QMessageBox>

ChangePassword::ChangePassword(QWidget *parent, QObject *object, QString role, QString userName, QString userRole) :
    QDialog(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);

    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    currentRole = role;
    changeUserRole = userRole;
    dblink = qobject_cast<MainWindow*>(object)->database();
    ui->lineEditUser->setText(userName);
}

ChangePassword::~ChangePassword()
{
    delete ui;
}

void ChangePassword::on_buttonBox_accepted()
{
    QString userName = ui->lineEditUser->text();
    QString password = ui->lineEditPassword->text();
    if (currentRole == "supervisor" && changeUserRole == "operator" || currentRole == "admin" && changeUserRole != "admin")
        if (password == ui->lineEditCheckPassword->text())
            if (!dblink->changePassword(userName, password)) QMessageBox::warning(this, "Ошибка", "Не удалось сменить пароль");
            else accept();
        else QMessageBox::warning(this, "Ошибка", "Проверьте пароль еще раз");
    else QMessageBox::warning(this, "Ошибка", "Недостаточно прав");
}
