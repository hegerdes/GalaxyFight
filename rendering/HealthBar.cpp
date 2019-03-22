#include "HealthBar.hpp"

namespace asteroids
{
HealthBar::HealthBar(Player p, int width, int height, int maxHP)
{
    m_player = p;
    m_width = width;
    m_height = height;
    m_maxHP = m_currentHP = maxHP;

}

void HealthBar::setHP(int newHP)
{
    m_currentHP = newHP;
}

void HealthBar::render()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); 
    glLoadIdentity();
    glOrtho(0, m_width, 0, m_height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(0.5f, 1.0f, 0.5f);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        if(m_player == PLAYER_1)
        {
            glVertex3f(m_width/2.2 * m_currentHP/m_maxHP, m_height * 9/10, 0.0f);
            glVertex3f(m_width/2.2 * m_currentHP/m_maxHP, m_height, 0.0f);
            glVertex3f(0.0f, m_height, 0.0f);
            glVertex3f(0.0f, m_height * 9/10, 0.0f);
        } else
        {
            glColor3f(1.0f, 0.5f, 0.5f);
            glVertex3f(m_width, m_height * 9/10, 0.0f);
            glVertex3f(m_width, m_height, 0.0f);
            glVertex3f(m_width-m_width/2.2 * m_currentHP/m_maxHP, m_height, 0.0f);
            glVertex3f(m_width-m_width/2.2 * m_currentHP/m_maxHP, m_height * 9/10, 0.0f);
        }
        
    glEnd();
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
}