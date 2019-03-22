#include "Crosshair.hpp"
namespace asteroids
{
Crosshair::Crosshair(float red, float green, float blue, int width, int height)
{
    r = (red > 255) ? 255 : red;
    g = (green > 255) ? 255 : green;
    b = (blue > 255) ? 255 : blue;
    m_width = width;
    m_height = height;
    c_dimension1 = width/200;
    c_dimension2 = height/15 - height/30;
    offset = height/60;
}

void Crosshair::render()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); 
    glLoadIdentity();
    glOrtho(0, m_width, 0, m_height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(r, g, b);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glVertex3f(m_width/2 + c_dimension1, m_height/2 + offset, 0.0f);
        glVertex3f(m_width/2 + c_dimension1, m_height/2 + offset + c_dimension2, 0.0f);
        glVertex3f(m_width/2 - c_dimension1, m_height/2 + offset + c_dimension2, 0.0f);
        glVertex3f(m_width/2 - c_dimension1, m_height/2 + offset, 0.0f);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(m_width/2 + c_dimension1, m_height/2 - offset, 0.0f);
        glVertex3f(m_width/2 + c_dimension1, m_height/2 - (offset + c_dimension2), 0.0f);
        glVertex3f(m_width/2 - c_dimension1, m_height/2 - (offset + c_dimension2), 0.0f);
        glVertex3f(m_width/2 - c_dimension1, m_height/2 - offset, 0.0f);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(m_width/2 + offset, m_height/2 + c_dimension1, 0.0f);
        glVertex3f(m_width/2 + offset + c_dimension2, m_height/2 + c_dimension1, 0.0f);
        glVertex3f(m_width/2 + offset + c_dimension2, m_height/2 - c_dimension1, 0.0f);
        glVertex3f(m_width/2 + offset, m_height/2 - c_dimension1, 0.0f);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(m_width/2 - offset, m_height/2 + c_dimension1, 0.0f);
        glVertex3f(m_width/2 - (offset + c_dimension2), m_height/2 + c_dimension1, 0.0f);
        glVertex3f(m_width/2 - (offset + c_dimension2), m_height/2 - c_dimension1, 0.0f);
        glVertex3f(m_width/2 - offset, m_height/2  - c_dimension1, 0.0f);
    glEnd();
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
}