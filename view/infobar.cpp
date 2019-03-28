#include "infobar.h"
#include "ui_infobar.h"

Infobar::Infobar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Infobar)
{

    m_time = setting.value("Resourcen/Rundenzeit").toInt();

    ui->setupUi(this);

    //Set singelton instances
    asteroids::MapFactory& factory = asteroids::MapFactory::getinstance();
    m_manage_game = asteroids::ManageGame::getinstance();

    //Set map and planets
    m_planetmap = factory.getMap(setting.value("Dateipfade/Map").toString().toStdString());
    m_planets = m_planetmap->getPlanets();
    m_selected_planet = -1;

    //Draw default ressources on infobar
    updateInfobar();

    //Connects

    connect(this, &Infobar::next_round,m_manage_game, &asteroids::ManageGame::next_round);
    connect(this, &Infobar::build_mine,m_manage_game, &asteroids::ManageGame::build_mine);
    connect(this, &Infobar::build_fighter,m_manage_game, &asteroids::ManageGame::build_fighter);
    connect(this, &Infobar::build_factory,m_manage_game, &asteroids::ManageGame::build_factory);
    connect(this, &Infobar::build_transporter,m_manage_game, &asteroids::ManageGame::build_transporter);

    connect(m_manage_game, &asteroids::ManageGame::updateInfobar,this,&Infobar::updateInfobar);

    connect(m_manage_game, &asteroids::ManageGame::no_resources, this, &Infobar::no_resources); 
    connect(m_manage_game, &asteroids::ManageGame::not_ur_planet, this, &Infobar::not_ur_planet);
    connect(m_manage_game, &asteroids::ManageGame::not_ur_ship, this, &Infobar::not_ur_ship);
    connect(m_manage_game, &asteroids::ManageGame::already_exist, this, &Infobar::already_exist);
    connect(this, &Infobar::nothingSelected, this, &Infobar::nothingSelectedReceiver);

    connect( &m_takt, &QTimer::timeout, [this](){set_time(m_timer.remainingTime()/1000);});
    connect( &m_timer, &QTimer::timeout, m_manage_game, &asteroids::ManageGame::next_round);

    connect( m_manage_game, &asteroids::ManageGame::stopTimer, this, &Infobar::stop_timer);
    connect( m_manage_game, &asteroids::ManageGame::resetTimer, this, &Infobar::reset_timer);

    //Initialize popup
    m_popup.setParent(this);
    m_popup.setGeometry(400, 0, 250, 200);
    m_popup.setStyleSheet("background: rgb(46, 52, 54); color: rgb(255, 255, 255)");
    m_popup.setStandardButtons(0);

    m_popup.setWindowFlags(Qt::FramelessWindowHint);
    m_popup.setAttribute(Qt::WA_NoSystemBackground);
    m_popup.setAttribute(Qt::WA_TranslucentBackground);
    m_popup.setAttribute(Qt::WA_TransparentForMouseEvents);
}

void Infobar::updateInfobar()
{
    ui->kampschiffe_value->setNum(m_manage_game->get_attackSpaceCraft_number());
    ui->transporter_value->setNum(m_manage_game->get_transportCpaceCraft_number());
    ui->erz_value->setNum(m_manage_game->get_current_resource());
    ui->ertrag_value->setNum(m_manage_game->get_resource_per_time());

    if(m_selected_planet > 0 && m_selected_planet < m_planetmap->getNumberOfPlanets())
    {
        ui->erzvorkommen_value->setNum(m_planets.at((unsigned long) m_selected_planet)->getOre());
        ui->minenanzahl_value->setNum(m_planets.at((unsigned long)m_selected_planet)->getMine());
        ui->erzlager_value->setNum(m_planets.at((unsigned long)m_selected_planet)->getStoredOre());
        ui->planetname->setText(QString::fromStdString(m_planets.at((unsigned long)m_selected_planet)->getname()));
        set_selected_planet(m_selected_planet);
    }
}

void Infobar::set_selected_planet(int planet_id)
{
    m_selected_planet = planet_id;

    ui->erzvorkommen_value->setNum(m_planets.at((unsigned long) m_selected_planet)->getOre());
    ui->minenanzahl_value->setNum(m_planets.at((unsigned long)m_selected_planet)->getMine());
    ui->erzlager_value->setNum(m_planets.at((unsigned long)m_selected_planet)->getStoredOre());
    ui->planetname->setText(QString::fromStdString(m_planets.at((unsigned long)m_selected_planet)->getname()));

}

void Infobar::set_time(double time)
{
    ui->timer->display(time);
    m_takt.start(100);
}

void Infobar::reset_timer()
{
    m_takt.start(100);
    m_timer.start(m_time);
}

void Infobar::stop_timer()
{
    m_timer.stop();
}

//For popup
void Infobar::no_resources(int num)
{
    if(num == 0)
    {
        m_popup.setText("Du besitzt nicht genügend Erz um diese Aktion durchzuführen!");
    }
    else
    {
        m_popup.setText("Einige Minenen haben kein Erz mehr");
    }
    QTimer::singleShot(2000, &m_popup, &QMessageBox::hide);
    m_popup.exec();
}

void Infobar::not_ur_planet()
{
    m_popup.setText("Dieser Planet gehört dir noch nicht!");
    QTimer::singleShot(2000, &m_popup, &QMessageBox::hide);
    m_popup.exec();
}

void Infobar::not_ur_ship()
{
    m_popup.setText("Dieses Raumschiff gehört dir nicht und du kannst es auch nicht stehlen, weil stehlen böse ist!");
    QTimer::singleShot(3000, &m_popup, &QMessageBox::hide);
    m_popup.exec();
}

void Infobar::already_exist()
{
    m_popup.setText("Du besitzt auf diesem Planeten bereits eine Werft!");
    QTimer::singleShot(2000, &m_popup, &QMessageBox::hide);
    m_popup.exec();
}

void Infobar::nothingSelectedReceiver()
{
    m_popup.setText("Du hast kein Raumschiff oder Planenten ausgewählt!");
    QTimer::singleShot(2000, &m_popup, &QMessageBox::hide);
    m_popup.exec();
}

//For button clicks
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

void Infobar::on_weiter_clicked()
{
    emit this->next_round();
}

void Infobar::on_aufgeben_clicked()
{
    QMessageBox surrender;
    surrender.setParent(this);
    surrender.setGeometry(60, 34, 0, 0);
    surrender.setStyleSheet("border: 3px solid rgb(30, 40, 40); background: rgb(30, 40, 40); color: rgb(255, 255, 255)");
    surrender.setText("Bist du sicher, dass du aufgeben willst?");

    QAbstractButton* pButtonYes = surrender.addButton(tr("Ja"), QMessageBox::YesRole);
    surrender.addButton(tr("Nein"), QMessageBox::NoRole);

    surrender.exec();

    if (surrender.clickedButton() == pButtonYes) {
        m_manage_game->end_game(false);
    }
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

