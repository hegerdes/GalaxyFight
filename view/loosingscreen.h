#ifndef LOOSINGSCREEN_H
#define LOOSINGSCREEN_H

#include <QWidget>
#include "settings/init_file.h"

namespace Ui {
class loosingscreen;
}

class loosingscreen : public QWidget
{
    Q_OBJECT

public:
    explicit loosingscreen(QWidget *parent = nullptr);
    ~loosingscreen();

private:
    Ui::loosingscreen *ui;
};

#endif // LOOSINGSCREEN_H
