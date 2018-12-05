#ifndef NEWUSER_H
#define NEWUSER_H

#include <QDialog>

#include "dbservice.h"
#include "QStandardItemModel"

namespace Ui {
class NewUser;
}

class NewUser : public QDialog
{
    Q_OBJECT

public:
    explicit NewUser(QWidget *parent = nullptr, QObject *other = nullptr, QString = "supervisor", QStandardItemModel *modelTab = nullptr);
    ~NewUser();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewUser *ui;
    seye::DBService *dblink;
    QStandardItemModel *model;
};

#endif // NEWUSER_H
