#include "Snake.h"
#include "Adafruit_ILI9340.h"

bool Snake::isNotDead() const {
  return m_notDead;
}

void Snake::update() {
  if ((m_x + m_directionX*m_scale) > 240)
    m_x = 0;
  else if (m_x < 0)
    m_x = 240-m_scale;
  else
    m_x += m_directionX*m_scale;
  if ((m_y + m_directionY*m_scale) > 300)
    m_y = 0;
  else if (m_y < 0)
    m_y = 300-m_scale;
  else
    m_y += m_directionY*m_scale;
}

//Clears the snake and prints a new one on the next position.
void Snake::drawAndUpdate() {
  m_tft->fillRect(m_x, m_y, m_scale, m_scale, ILI9340_BLACK);
  update();
  m_tft->fillRect(m_x, m_y, m_scale, m_scale, ILI9340_WHITE);
}

void Snake::printScore() {
  m_tft->fillRect(135, 308, 10, 10, ILI9340_BLACK);
  m_tft->setCursor(135, 308);
  m_tft->print(m_score);
}

void Snake::getDirection() {
  if(!digitalRead(g_rightArrow) && !m_directionX) {
    m_directionX = 1;
    m_directionY = 0;
  }
  else if(!digitalRead(g_leftArrow) && !m_directionX) {
    m_directionX = -1;
    m_directionY = 0;
  }
  else if(!digitalRead(g_upArrow) && !m_directionY) {
    m_directionX = 0;
    m_directionY = -1;
  }
  else if(!digitalRead(g_downArrow) && !m_directionY) {
    m_directionX = 0;
    m_directionY = 1;
  }
}
