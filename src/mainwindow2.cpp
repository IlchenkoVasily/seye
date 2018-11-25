#include "mainwindow2.h"
#include "ui_mainwindow2.h"

Mainwindow2::Mainwindow2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mainwindow2)
{
    ui->setupUi(this);
}

Mainwindow2::~Mainwindow2()
{
    delete ui;
}
