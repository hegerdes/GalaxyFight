#include "infobar.h"
#include "ui_infobar.h"

Infobar::Infobar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Infobar)
{
    ui->setupUi(this);

    asteroids::MapFactory& factory = asteroids::MapFactory::getinstance();
    m_manage_game = asteroids::ManageGame::getinstance();

    m_planetmap = factory.getMap("models/01.map");
    m_planets = m_planetmap->getPlanets();
    m_selected_planet = -1;

    connect(this, &Infobar::end_game,m_manage_game, &asteroids::ManageGame::end_game);
    connect(this, &Infobar::next_round,m_manage_game, &asteroids::ManageGame::next_round);
    connect(this, &Infobar::build_mine,m_manage_game, &asteroids::ManageGame::build_mine);
    connect(this, &Infobar::build_fighter,m_manage_game, &asteroids::ManageGame::build_fighter);
    connect(this, &Infobar::build_factory,m_manage_game, &asteroids::ManageGame::build_factory);
    connect(this, &Infobar::build_transporter,m_manage_game, &asteroids::ManageGame::build_transporter);

    connect(m_manage_game, &asteroids::ManageGame::updateInfobar,this,&Infobar::updateInfobar);
}

void Infobar::updateInfobar()
{
    ui->kampschiffe_value->setNum(m_manage_game->get_attackSpaceCraft_number());
    ui->transporter_value->setNum(m_manage_game->get_transportCpaceCraft_number());
    ui->erz_value->setNum(m_manage_game->get_current_resource());
    ui->ertrag_value->setNum(m_manage_game->get_resource_per_time());
}

void Infobar::set_selected_planet(int planet_id)
{
    m_selected_planet = planet_id;

    ui->erzvorkommen_value->setNum(m_planets.at((unsigned long) m_selected_planet)->getOre());
    ui->minenanzahl_value->setNum(m_planets.at((unsigned long)m_selected_planet)->getMine());
    ui->erzlager_value->setNum(m_planets.at((unsigned long)m_selected_planet)->getStoredOre());
    ui->planetname->setText(QString::fromStdString(m_planets.at((unsigned long)m_selected_planet)->getname()));

}

//For button clicks
//TODO Set other color if presssed
//TODO Send planet_id insted of 1
void Infobar::on_mine_bauen_clicked()
{   
    if(m_selected_planet < 0 || m_selected_planet >= m_planetmap.get()->getNumberOfPlanets())
    {
        emit nothingSelected();
    }
    else
    {
        emit this->build_mine(m_selected_planet);
    }
}

void Infobar::on_werft_bauen_clicked()
{   
    if(m_selected_planet < 0 || m_selected_planet >= m_planetmap.get()->getNumberOfPlanets())
    {
        emit nothingSelected();
    }
    else
    {
        emit this->build_factory(m_selected_planet);
    }
}

void Infobar::on_kampfschiff_bauen_clicked()
{
    if(m_selected_planet < 0 || m_selected_planet >= m_planetmap.get()->getNumberOfPlanets())
    {
        emit nothingSelected();
    }
    else
    {
        emit this->build_fighter(m_selected_planet);
    }
}

void Infobar::on_transporter_bauen_clicked()
{
    if(m_selected_planet < 0 || m_selected_planet >= m_planetmap.get()->getNumberOfPlanets())
    {
        emit nothingSelected();
    }
    else
    {
        emit this->build_transporter(m_selected_planet);
    }
}

void Infobar::on_aufgeben_clicked()
{
    emit this->end_game();
}

void Infobar::on_weiter_clicked()
{
    emit this->next_round();
}



//For button enable/disable
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

Infobar::~Infobar()
{
    delete ui;
}

