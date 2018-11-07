#include "dialogadddevice.h"
#include "ui_dialogadddevice.h"

DialogAddDevice::DialogAddDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogAddDevice)
{
    ui->setupUi(this);
}

DialogAddDevice::~DialogAddDevice()
{
    delete ui;
}
