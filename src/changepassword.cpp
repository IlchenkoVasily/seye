#include "changepassword.h"
#include "ui_changepassword.h"

#include <QMessageBox>

ChangePassword::ChangePassword(const QString& userName, seye::DBService *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);

    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    dblink = db;
    ui->lineEditUser->setText(userName);
}

ChangePassword::~ChangePassword()
{
    delete ui;
}

void ChangePassword::on_buttonBox_accepted()
{
    QString password = ui->lineEditPassword->text();
    if (password == ui->lineEditCheckPassword->text())
        if (dblink->changePassword(ui->lineEditUser->text(), password)) accept();
        else QMessageBox::warning(this, "Неудача", "Возможно в пароле присутствуют недопустимые символы");
    else QMessageBox::warning(this, "Ошибка", "Проверьте пароль еще раз");
}
