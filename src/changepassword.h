#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>

#include "dbservice.h"

namespace Ui {
class ChangePassword;
}

class ChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePassword(QWidget *parent = nullptr, QObject *other = nullptr, QString role = "supervisor", QString userName = nullptr, QString userRole = "operator");
    ~ChangePassword();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ChangePassword *ui;
    seye::DBService *dblink;
    QString currentRole;
    QString changeUserRole;
};

#endif // CHANGEPASSWORD_H
