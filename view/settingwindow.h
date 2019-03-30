#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QWidget>
#include <QSettings>
#include "settings/init_file.h"

namespace Ui {
class settingwindow;
}

class settingwindow : public QWidget
{
    Q_OBJECT

public:
    explicit settingwindow(QWidget *parent = nullptr);
    ~settingwindow();

signals:
    void goToStart();

private slots:
    void on_BackButton_clicked();

private:
    Ui::settingwindow *ui;
};

#endif // SETTINGWINDOW_H
