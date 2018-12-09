#ifndef SCENARIO_H
#define SCENARIO_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class Scenario;
}

class Scenario : public QDialog
{
    Q_OBJECT

public:
    explicit Scenario(QWidget *parent = nullptr);
    ~Scenario();
//    QString timeInterval(const QString& from, const QString& to, const QString& format = "hh.mm.ss-dd.MM.yyyy");

private slots:
    void on_buttonBox_rejected();

    void on_dateTimeEnd_dateTimeChanged(const QDateTime &dateTime);

    void on_dateTimeStart_dateTimeChanged(const QDateTime &dateTime);

    void on_buttonBox_accepted();

private:
    Ui::Scenario *ui;
};

#endif // SCENARIO_H
