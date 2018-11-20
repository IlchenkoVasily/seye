#include "zoneinfo.h"
#include "ui_zoneinfo.h"

zoneinfo::zoneinfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::zoneinfo)
{
    ui->setupUi(this);
}

zoneinfo::~zoneinfo()
{
    delete ui;
}
