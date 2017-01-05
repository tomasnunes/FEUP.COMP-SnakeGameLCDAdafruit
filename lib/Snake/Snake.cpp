#include "Snake.h"

Snake& Snake::operator++() {
  ++m_score;
  ++m_size;
  return *this;
}

bool Snake::isNotDead() const {
  return m_notDead;
}

void Snake::hasEaten() {
  m_hasEaten = true;
}

void Snake::updateHead() {
  if(m_gridHeadX+m_directionX > m_gridWidth-1)
    m_gridHeadX = 0;
  else if(m_gridHeadX+m_directionX < 0)
    m_gridHeadX = m_gridWidth-1;
  else
    m_gridHeadX += m_directionX;

  if(m_gridHeadY+m_directionY > m_gridHeight-1)
    m_gridHeadY = 0;
  else if(m_gridHeadY+m_directionY < 0)
    m_gridHeadY = m_gridHeight-1;
  else
    m_gridHeadY += m_directionY;
}

void Snake::cleanTail() {
  m_tft->fillRect(m_gridTailX*m_scale, m_gridTailY*m_scale, m_scale, m_scale, ILI9340_BLACK);
}

void Snake::drawHead() {
  m_tft->fillRect(m_gridHeadX*m_scale, m_gridHeadY*m_scale, m_scale, m_scale, ILI9340_WHITE);
}

void Snake::drawAndClean() {
    if(!m_hasEaten) {
      cleanTail();
      m_gridTailX = m_gridPosition[m_size-2].gridX;
      m_gridTailY = m_gridPosition[m_size-2].gridY;
    }
    else
      m_hasEaten = false;

    drawHead();
    pushToVector();
}

void Snake::printScore() {
  m_tft->fillRect(135, 304, 35, 10, ILI9340_BLACK);
  m_tft->setCursor(135, 304);
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

void Snake::pushToVector() {
  if(m_size>m_vectorSize) {
    m_vectorSize = m_size*2;
    m_gridPosition = (GridPosition*)realloc(m_gridPosition, sizeof(GridPosition)*m_vectorSize);
  }

  for(int ii=m_size-1; ii>0; --ii) {
    m_gridPosition[ii].gridX = m_gridPosition[ii-1].gridX;
    m_gridPosition[ii].gridY = m_gridPosition[ii-1].gridY;
  }

  m_gridPosition[0].gridX = m_gridHeadX;
  m_gridPosition[0].gridY = m_gridHeadY;
}

bool operator==(const Snake &snake, const Food &food) {
  return (snake.m_gridHeadX == food.m_gridX && snake.m_gridHeadY == food.m_gridY);
}
bool operator==(const Food &food, const Snake &snake) {
  return (snake.m_gridHeadX == food.m_gridX && snake.m_gridHeadY == food.m_gridY);
}
bool operator!=(const Food &food, const Snake &snake) {
  for(int ii=0; ii<snake.m_vectorSize; ++ii) //for(const GridPosition &position : snake.m_gridPosition)
    if(snake.m_gridPosition[ii].gridX == food.m_gridX && snake.m_gridPosition[ii].gridY == food.m_gridY)
      return true;

  return false;
}
bool operator!=(const Snake &snake, const Food &food) {
  for(int ii=0; ii<snake.m_vectorSize; ++ii) //for(const GridPosition &position : snake.m_gridPosition)
    if(snake.m_gridPosition[ii].gridX == food.m_gridX && snake.m_gridPosition[ii].gridY == food.m_gridY)
      return true;

  return false;
}
