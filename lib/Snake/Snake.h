#ifndef _SNAKE_H
#define _SNAKE_H

#include "Adafruit_ILI9340.h"
#include "Food.h"

struct GridPosition {
  int gridX, gridY;
};

class Snake {
private:
  GridPosition *m_gridSnakePosition;
  int m_gridHeadX, m_gridHeadY, m_gridTailX, m_gridTailY, m_directionX, m_directionY, m_scale, m_score, m_size, m_gridWidth, m_gridHeight, m_vectorSize, m_lineTop, m_lineBelow, m_lineRight, m_lineLeft;
  bool m_hasDied, m_hardMode;

  Adafruit_ILI9340 *m_tft; //Pointer to our screen object (tft) so we can use it in our snake member functions.
                           //An alternative would be passing the tft object by reference to every method, if necessary.

public:
  Snake(Adafruit_ILI9340 *tft, bool hardMode=false, int gridX=3, int gridY=5, int initSize=4, int scale=8)
  : m_tft(tft), m_hardMode(hardMode), m_gridHeadX(gridX), m_gridHeadY(gridY), m_scale(scale) {
    m_directionX = 1;
    m_directionY = 0;
    m_gridTailX = -1;
    m_gridTailY = -1;
    m_score = 0;
    m_size = 0;
    m_hasDied = false;
    m_gridWidth = g_width/m_scale;
    m_gridHeight = (g_heigth-24)/m_scale;
    m_lineTop = m_lineBelow = m_lineRight = m_lineLeft = 0;

    m_vectorSize = initSize*2;
    m_gridSnakePosition = (GridPosition*)malloc(sizeof(GridPosition)*m_vectorSize);

    for(int ii=0; ii<initSize; ++ii) {
      ++m_size;
      updateHeadAndCheckWallHit();
      if(ii!=initSize-1) {
        drawHead();
        pushToVector();
      }
      else
        drawAndClean();
    }
  }

  ~Snake() {
    free(m_gridSnakePosition);
    m_gridSnakePosition = nullptr;
  }

  Snake& operator++();

  int getSize() const;
  bool isDead() const;
  void dies();
  void flash();
  bool playAgain();
  void updateHeadAndCheckWallHit();
  void drawHead();
  void cleanTail();
  void drawAndClean();
  void printScore();
  void getDirection();
  void generateFood();
  void pushToVector();
  void ateItsOwnTail();

  friend class Food;
  friend bool operator==(const Food &food, const Snake &snake);
  friend bool operator==(const Snake &snake, const Food &food);
  friend bool operator!=(const Food &food, const Snake &snake);
  friend bool operator!=(const Snake &snake, const Food &food);
};

#endif
