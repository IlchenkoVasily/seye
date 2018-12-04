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

private:
    Ui::Scenario *ui;
};

#endif // SCENARIO_H
