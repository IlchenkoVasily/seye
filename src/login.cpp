#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include <QMessageBox>

login::login(QString* role, QWidget *parent) :
    QDialog(parent), role(role),
    ui(new Ui::login)
{
    ui->setupUi(this);

    ui->labelWarning->hide();
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_clicked()
{
    QString host = "31.211.74.221";

    QString user = ui->username->text();
    dblink = new seye::DBService(host, user, ui->password->text());

    *role = dblink->getRole(user);
    if (role->isEmpty()) ui->labelWarning->show();
    else login::close();
}

seye::DBService *login::getDatabase()
{
    return dblink;
}
