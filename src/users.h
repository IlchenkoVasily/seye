#ifndef USERS_H
#define USERS_H

#include <QDialog>

#include "dbservice.h"
#include "QStandardItemModel"

namespace Ui {
class Users;
}

class Users : public QDialog
{
    Q_OBJECT

public:
    explicit Users(const QString& currentRole, seye::DBService *db, QWidget *parent = nullptr);
    ~Users();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButtonDelete_clicked();

private:
    Ui::Users *ui;
    seye::DBService *dblink;
    QString currentRole;
    QStandardItemModel *model;
};

#endif // USERS_H
