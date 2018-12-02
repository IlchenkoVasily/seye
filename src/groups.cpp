#include "groups.h"
#include "ui_groups.h"

Groups::Groups(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Groups)
{
    ui->setupUi(this);
}

Groups::~Groups()
{
    delete ui;
}
