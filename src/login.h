#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QCloseEvent>
#include <QDialog>
#include <dbservice.h>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(seye::DBService* link, QString* role, QWidget *parent = nullptr);
    ~login();
    bool enter(QObject *parent = 0);

private slots:
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *event);


private:
    seye::DBService* dblink;
    QString* role;
    Ui::login *ui;
};

#endif // LOGIN_H
