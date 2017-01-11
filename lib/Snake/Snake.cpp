#include "Snake.h"

int Snake::getSize() const {
  return m_size;
}

bool Snake::isDead() const {
  return m_hasDied;
}

void Snake::dies() {
  m_hasDied = true;
  flash();
}

void Snake::flash() {
  delay(800);

  for(int ii=0; ii<m_size; ++ii)
    m_tft->fillRect(m_gridSnakePosition[ii].gridX*m_scale, m_gridSnakePosition[ii].gridY*m_scale, m_scale, m_scale, ILI9340_BLACK);
  delay(50);

  for(int ii=0; ii<m_size; ++ii) {
    m_tft->fillRect(m_gridSnakePosition[ii].gridX*m_scale, m_gridSnakePosition[ii].gridY*m_scale, m_scale, m_scale, ILI9340_RED);

    //Print black line separating each part
    if(ii == m_size-1) { }//If in last position, do nothing
    else if(m_gridSnakePosition[ii].gridX > m_gridSnakePosition[ii+1].gridX)
      m_tft->drawFastVLine(m_gridSnakePosition[ii].gridX*m_scale, m_gridSnakePosition[ii].gridY*m_scale, m_scale, ILI9340_BLACK);
    else if(m_gridSnakePosition[ii].gridX < m_gridSnakePosition[ii+1].gridX)
      m_tft->drawFastVLine((m_gridSnakePosition[ii].gridX+1)*m_scale-1, m_gridSnakePosition[ii].gridY*m_scale, m_scale, ILI9340_BLACK);
    else if(m_gridSnakePosition[ii].gridY > m_gridSnakePosition[ii+1].gridY)
      m_tft->drawFastHLine(m_gridSnakePosition[ii].gridX*m_scale, m_gridSnakePosition[ii].gridY*m_scale, m_scale, ILI9340_BLACK);
    else if(m_gridSnakePosition[ii].gridY < m_gridSnakePosition[ii+1].gridY)
      m_tft->drawFastHLine(m_gridSnakePosition[ii].gridX*m_scale, (m_gridSnakePosition[ii].gridY+1)*m_scale-1, m_scale, ILI9340_BLACK);

    delay(20);
  }
  delay(1000);
}

bool Snake::playAgain() {
  m_tft->fillScreen(ILI9340_BLACK);

  cleanInput();

  m_tft->setCursor(40, 70);
  m_tft->setTextColor(ILI9340_RED); m_tft->setTextSize(3);
  m_tft->print("Game Over");
  m_tft->setCursor(80, 140);
  m_tft->setTextColor(ILI9340_CYAN); m_tft->setTextSize(1.5);
  m_tft->print("Your Score: "); m_tft->print(m_score);
  m_tft->setCursor(18, 200);
  m_tft->setTextColor(ILI9340_WHITE); m_tft->setTextSize(2);
  m_tft->print("Wanna play again?");
  m_tft->setCursor(60, 230);
  m_tft->setTextColor(ILI9340_WHITE); m_tft->setTextSize(2);
  m_tft->print("(UP)   Yes");
  m_tft->setCursor(60, 250);
  m_tft->setTextColor(ILI9340_WHITE); m_tft->setTextSize(2);
  m_tft->print("(DOWN) No");

  while(1) {
    if(Serial.available()) {
      g_buffer = ' ';
      g_buffer = Serial.read();
    }

    if(!digitalRead(g_upArrow) || g_buffer=='w' || g_buffer=='W') {
      g_buffer = ' ';
      return true;
    }
    else if(!digitalRead(g_downArrow) || g_buffer=='s' || g_buffer=='S') {
      g_buffer = ' ';
      return false;
    }
  }

  g_buffer = ' ';
}

void Snake::updateHeadAndCheckWallHit() {
  if((m_gridHeadX+m_directionX < 0 || m_gridHeadX+m_directionX > m_gridWidth-1) && m_hardMode)
    m_hasDied = true;
  else if(m_gridHeadX+m_directionX < 0)
    m_gridHeadX = m_gridWidth-1;
  else if(m_gridHeadX+m_directionX > m_gridWidth-1)
    m_gridHeadX = 0;
  else
    m_gridHeadX += m_directionX;

  if((m_gridHeadY+m_directionY < 0 || m_gridHeadY+m_directionY > m_gridHeight-1) && m_hardMode)
    m_hasDied = true;
  else if(m_gridHeadY+m_directionY < 0)
    m_gridHeadY = m_gridHeight-1;
  else if(m_gridHeadY+m_directionY > m_gridHeight-1)
    m_gridHeadY = 0;
  else
    m_gridHeadY += m_directionY;
}

void Snake::cleanTail() {
  m_tft->fillRect(m_gridTailX*m_scale, m_gridTailY*m_scale, m_scale, m_scale, ILI9340_BLACK);
}

void Snake::drawHead() {
  m_tft->fillRect(m_gridHeadX*m_scale, m_gridHeadY*m_scale, m_scale, m_scale, ILI9340_GREEN);
  //Evalute side to leave black line
  if(m_gridHeadX > m_gridSnakePosition[0].gridX)
    m_tft->drawFastVLine(m_gridHeadX*m_scale, m_gridHeadY*m_scale, m_scale, ILI9340_BLACK);
  else if(m_gridHeadX < m_gridSnakePosition[0].gridX)
    m_tft->drawFastVLine((m_gridHeadX+1)*m_scale-1, m_gridHeadY*m_scale, m_scale, ILI9340_BLACK);
  else if(m_gridHeadY > m_gridSnakePosition[0].gridY)
    m_tft->drawFastHLine(m_gridHeadX*m_scale, m_gridHeadY*m_scale, m_scale, ILI9340_BLACK);
  else if(m_gridHeadY < m_gridSnakePosition[0].gridY)
    m_tft->drawFastHLine(m_gridHeadX*m_scale, (m_gridHeadY+1)*m_scale-1, m_scale, ILI9340_BLACK);
}

void Snake::drawAndClean() {
  drawHead();
  cleanTail();
  m_gridTailX = m_gridSnakePosition[m_size-2].gridX;
  m_gridTailY = m_gridSnakePosition[m_size-2].gridY;

  pushToVector();
}

void Snake::printScore() {
  m_tft->fillRect(135, 304, 35, 10, ILI9340_BLACK);
  m_tft->setCursor(135, 304);
  m_tft->print(m_score);
}

void Snake::getDirection() {
  if (Serial.available()) {
    g_buffer = Serial.read();
  }

  if((!digitalRead(g_rightArrow) || g_buffer=='d' || g_buffer=='D') && !m_directionX) {
    m_directionX = 1;
    m_directionY = 0;
  }
  else if((!digitalRead(g_leftArrow) || g_buffer=='a' || g_buffer=='A') && !m_directionX) {
    m_directionX = -1;
    m_directionY = 0;
  }
  else if((!digitalRead(g_upArrow) || g_buffer=='w' || g_buffer=='W') && !m_directionY) {
    m_directionX = 0;
    m_directionY = -1;
  }
  else if((!digitalRead(g_downArrow) || g_buffer=='s' || g_buffer=='S') && !m_directionY) {
    m_directionX = 0;
    m_directionY = 1;
  }

  g_buffer = ' ';
}

void Snake::pushToVector() {
  if(m_size>m_vectorSize) {
    m_vectorSize = m_size*2;
    m_gridSnakePosition = (GridPosition*)realloc(m_gridSnakePosition, sizeof(GridPosition)*m_vectorSize);
  }

  for(int ii=m_size-1; ii>0; --ii) {
    m_gridSnakePosition[ii].gridX = m_gridSnakePosition[ii-1].gridX;
    m_gridSnakePosition[ii].gridY = m_gridSnakePosition[ii-1].gridY;
  }

  m_gridSnakePosition[0].gridX = m_gridHeadX;
  m_gridSnakePosition[0].gridY = m_gridHeadY;
}

void Snake::ateItsOwnTail() {
  //Starts at 4, it can't eat its own tail if its size is 4 or less
  for(int ii=4; ii<m_size; ++ii)
    if(m_gridHeadX == m_gridSnakePosition[ii].gridX && m_gridHeadY == m_gridSnakePosition[ii].gridY) {
      m_hasDied = true;
      return;
    }
}

Snake& Snake::operator++() {
  ++m_score;
  ++m_size;
  printScore();

  return *this;
}
bool operator==(const Snake &snake, const Food &food) {
  return (snake.m_gridHeadX == food.m_gridX && snake.m_gridHeadY == food.m_gridY);
}
bool operator==(const Food &food, const Snake &snake) {
  return (snake.m_gridHeadX == food.m_gridX && snake.m_gridHeadY == food.m_gridY);
}
bool operator!=(const Food &food, const Snake &snake) {
  for(int ii=0; ii<snake.m_vectorSize; ++ii) //for(const GridPosition &position : snake.m_gridSnakePosition)
    if(snake.m_gridSnakePosition[ii].gridX == food.m_gridX && snake.m_gridSnakePosition[ii].gridY == food.m_gridY)
      return true;

  return false;
}
bool operator!=(const Snake &snake, const Food &food) {
  for(int ii=0; ii<snake.m_vectorSize; ++ii) //for(const GridPosition &position : snake.m_gridSnakePosition)
    if(snake.m_gridSnakePosition[ii].gridX == food.m_gridX && snake.m_gridSnakePosition[ii].gridY == food.m_gridY)
      return true;

  return false;
}
