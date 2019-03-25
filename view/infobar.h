#ifndef INFOBAR_H
#define INFOBAR_H

#pragma once
#include <QWidget>
#include "../util/ManageGame.hpp"

namespace Ui {
class Infobar;
}
class Infobar : public QWidget
{
    Q_OBJECT

public:
    explicit Infobar(QWidget *parent = nullptr);
    ~Infobar();

signals:
    //Signals to ManagGame
    void build_factory(int planet_id);
    void build_mine(int planet_id);
    void build_fighter(int planet_id);
    void build_transporter(int planet_id);
    void nothingSelected();
    void next_round();
    void end_game();

public slots:

    //update all stats in infobar
    void updateInfobar();

    //Set selected planet
    void set_selected_planet(int);

    //Slots for button klicks
    void on_mine_bauen_clicked();

    void on_werft_bauen_clicked();

    void on_kampfschiff_bauen_clicked();

    void on_transporter_bauen_clicked();

    void on_aufgeben_clicked();

    void on_weiter_clicked();

    //Slots for button enable/disable
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

    //GameManager
    asteroids::ManageGame* m_manage_game;

    //The map
    asteroids::Map::Ptr m_planetmap;

    //All planets
    asteroids::Map::VecPtr m_planets;

    //Current selected planet
    int m_selected_planet;

    Ui::Infobar *ui;
};

#endif // INFOBAR_H
