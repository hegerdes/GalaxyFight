#ifndef LOOSINGSCREEN_H
#define LOOSINGSCREEN_H

#include <QWidget>

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
