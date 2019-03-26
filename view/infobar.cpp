#include "infobar.h"
#include "ui_infobar.h"

Infobar::Infobar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Infobar)
{
    ui->setupUi(this);

    //Set singelton instances
    asteroids::MapFactory& factory = asteroids::MapFactory::getinstance();
    m_manage_game = asteroids::ManageGame::getinstance();

    //Set map and planets
    m_planetmap = factory.getMap("models/01.map");
    m_planets = m_planetmap->getPlanets();
    m_selected_planet = -1;

    //Draw default ressources on infobar
    updateInfobar();

    //Connects
    connect(this, &Infobar::end_game, m_manage_game, &asteroids::ManageGame::end_game);

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

    //connect( m_manage_game, &asteroids::ManageGame::startTimer, this, &Infobar::resettime); //TODO Fix and use this line instead of the next 3 lines
    connect( &m_timer, &QTimer::timeout, this, &Infobar::resettime);
    m_takt.start(1000);
    m_timer.start(6000);
}

void Infobar::set_time(int time)
{
    ui->timer->display(time);
    m_takt.start(1000);
}

void Infobar::resettime()
{
    m_takt.start(1000);
    m_timer.start(60000);
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

void Infobar::no_resources()
{
    m_popup.setWindowTitle("Fehler");
    m_popup.setText("Du besitzt nicht genügend Erz um diese Aktion durchzuführen!");

    m_popup.setGeometry(0, 0, 250, 200);

    m_popup.setWindowFlags(Qt::FramelessWindowHint);
    m_popup.setAttribute(Qt::WA_NoSystemBackground);
    m_popup.setAttribute(Qt::WA_TranslucentBackground);
    m_popup.setAttribute(Qt::WA_TransparentForMouseEvents);

    m_popup.setStyleSheet("color: rgb(255, 255, 255)");

    m_popup.setStandardButtons(0);

    QTimer::singleShot(2000, &m_popup, SLOT(hide()));

    m_popup.exec();
}

void Infobar::not_ur_planet()
{
    m_popup.setWindowTitle("Fehler");
    m_popup.setText("Dieser Planet gehört dir noch nicht!");

    m_popup.setGeometry(0, 0, 250, 200);

    m_popup.setWindowFlags(Qt::FramelessWindowHint);
    m_popup.setAttribute(Qt::WA_NoSystemBackground);
    m_popup.setAttribute(Qt::WA_TranslucentBackground);
    m_popup.setAttribute(Qt::WA_TransparentForMouseEvents);

    m_popup.setStyleSheet("color: rgb(255, 255, 255)");

    m_popup.setStandardButtons(0);

    QTimer::singleShot(2000, &m_popup, SLOT(hide()));

    m_popup.exec();
}

void Infobar::not_ur_ship()
{
    m_popup.setWindowTitle("Fehler");
    m_popup.setText("Dieses Raumschiff gehört dir nicht und du kannst es auch nicht stehlen, weil stehlen böse ist!");

    m_popup.setGeometry(0, 0, 250, 200);

    m_popup.setWindowFlags(Qt::FramelessWindowHint);
    m_popup.setAttribute(Qt::WA_NoSystemBackground);
    m_popup.setAttribute(Qt::WA_TranslucentBackground);
    m_popup.setAttribute(Qt::WA_TransparentForMouseEvents);

    m_popup.setStyleSheet("color: rgb(255, 255, 255)");

    m_popup.setStandardButtons(0);

    QTimer::singleShot(3000, &m_popup, SLOT(hide()));

    m_popup.exec();
}

void Infobar::already_exist()
{
    m_popup.setWindowTitle("Fehler");
    m_popup.setText("Du besitzt auf diesem Planeten bereits eine Werft!");

    m_popup.setGeometry(0, 0, 250, 200);

    m_popup.setWindowFlags(Qt::FramelessWindowHint);
    m_popup.setAttribute(Qt::WA_NoSystemBackground);
    m_popup.setAttribute(Qt::WA_TranslucentBackground);
    m_popup.setAttribute(Qt::WA_TransparentForMouseEvents);

    m_popup.setStyleSheet("color: rgb(255, 255, 255)");

    m_popup.setStandardButtons(0);

    QTimer::singleShot(2000, &m_popup, SLOT(hide()));

    m_popup.exec();
}

void Infobar::nothingSelectedReceiver()
{
    m_popup.setWindowTitle("Fehler");
    m_popup.setText("Du hast kein Raumschiff oder Planenten ausgewählt!");

    m_popup.setGeometry(0, 0, 250, 200);

    m_popup.setWindowFlags(Qt::FramelessWindowHint);
    m_popup.setAttribute(Qt::WA_NoSystemBackground);
    m_popup.setAttribute(Qt::WA_TranslucentBackground);
    m_popup.setAttribute(Qt::WA_TransparentForMouseEvents);

    m_popup.setStyleSheet("color: rgb(255, 255, 255)");

    m_popup.setStandardButtons(0);

    QTimer::singleShot(2000, &m_popup, SLOT(hide()));

    m_popup.exec();
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

void Infobar::on_weiter_clicked()
{
    emit this->next_round();
}

void Infobar::on_aufgeben_clicked()
{
    m_popup.setWindowTitle("Nimm dir einen Moment Zeit");
    m_popup.setText("Bist du sicher, dass du aufgeben willst?");

    //m_fehler.setStyleSheet("color: rgb(255, 255, 255)");

    m_popup.setStandardButtons(QMessageBox::No);
    m_popup.addButton(QMessageBox::Yes);

    m_popup.exec();
}

void Infobar::on_yes_clicked()
{
    emit this->end_game();
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

