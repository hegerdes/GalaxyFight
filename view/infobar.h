#ifndef INFOBAR_H
#define INFOBAR_H

#include <QWidget>

namespace Ui {
class Infobar;
}

class Infobar : public QWidget
{
    Q_OBJECT

public:
    explicit Infobar(QWidget *parent = nullptr);
    ~Infobar();

public slots:
    void mine_bauen_disable();

    void mine_bauen_enable();

    void werft_bauen_disable();

    void werft_bauen_enable();

    void transporter_bauen_disable();

    void transporter_bauen_enable();

    void kampfschiff_bauen_disable();

    void kampfschiff_bauen_enable();

    void weiter_disable();

    void weiter_enable();

private:
    Ui::Infobar *ui;
};

#endif // INFOBAR_H
