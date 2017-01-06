#include "Food.h"

int Food::randNumber(int min, int max) {
    return (rand() % (max-min+1)) + min;
}

void Food::genLocation() {
    m_gridX = randNumber(0, m_gridWidth-1);
    m_gridY = randNumber(0, m_gridHeight-1);
}

void Food::updateAndDraw() {
  //m_tft->fillRect(m_gridX*m_scale, m_gridY*m_scale, m_scale, m_scale, ILI9340_BLACK);
  genLocation();
  m_tft->fillRect(m_gridX*m_scale, m_gridY*m_scale, m_scale, m_scale, ILI9340_CYAN);
}
