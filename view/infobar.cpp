#include "infobar.h"
#include "ui_infobar.h"

Infobar::Infobar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Infobar)
{
    ui->setupUi(this);
}

Infobar::~Infobar()
{
    delete ui;
}

void Infobar::mine_bauen_disable()
{
    ui->mine_bauen->setEnabled(false);
}

void Infobar::mine_bauen_enable()
{
    ui->mine_bauen->setEnabled(true);
}

void Infobar::werft_bauen_disable()
{
    ui->werft_bauen->setEnabled(false);
}

void Infobar::werft_bauen_enable()
{
    ui->werft_bauen->setEnabled(true);
}

void Infobar::transporter_bauen_disable()
{
    ui->transporter_bauen->setEnabled(false);
}

void Infobar::transporter_bauen_enable()
{
    ui->transporter_bauen->setEnabled(true);
}

void Infobar::kampfschiff_bauen_disable()
{
    ui->kampfschiff_bauen->setEnabled(false);
}

void Infobar::kampfschiff_bauen_enable()
{
    ui->kampfschiff_bauen->setEnabled(true);
}

void Infobar::weiter_disable()
{
    ui->weiter->setEnabled(false);
}

void Infobar::weiter_enable()
{
    ui->weiter->setEnabled(true);
}
