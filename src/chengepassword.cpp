#include "chengepassword.h"
#include "ui_chengepassword.h"

ChengePassword::ChengePassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChengePassword)
{
    ui->setupUi(this);
}

ChengePassword::~ChengePassword()
{
    delete ui;
}
