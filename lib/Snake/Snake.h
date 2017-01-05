#ifndef _SNAKE_H
#define _SNAKE_H

#include "Adafruit_ILI9340.h"
#include "Food.h"

struct GridPosition {
  int gridX, gridY;
};

class Snake {
private:
  GridPosition *m_gridPosition;
  int m_gridHeadX, m_gridHeadY, m_gridTailX, m_gridTailY, m_directionX, m_directionY, m_scale, m_score, m_size, m_gridWidth, m_gridHeight, m_vectorSize;
  bool m_notDead, m_hasEaten;

  Adafruit_ILI9340 *m_tft; //Pointer to our screen object (tft) so we can use it in our snake member functions.
                           //An alternative would be passing the tft object by reference to every method, if necessary.

public:
  Snake(Adafruit_ILI9340 *tft, int x=0, int y=0, int initSize=4, int scale=8)
  : m_tft(tft), m_gridHeadX(x), m_gridHeadY(y), m_scale(scale) {
    m_directionX = 0;
    m_directionY = 1;
    m_gridTailX = -1;
    m_gridTailY = -1;
    m_score = 0;
    m_size = 0;
    m_notDead = true;
    m_hasEaten = false;
    m_gridWidth = g_width/m_scale;
    m_gridHeight = (g_heigth-24)/m_scale;

    m_vectorSize = initSize*2;
    m_gridPosition = (GridPosition*)malloc(sizeof(GridPosition)*m_vectorSize);

    for(int ii=0; ii<initSize; ++ii) {
      ++m_size;
      if(ii!=initSize-1)
        m_hasEaten = true;
      updateHead();
      drawAndClean();
    }
  }

  ~Snake() {
    free(m_gridPosition);
  }

  Snake& operator++();

  void hasEaten();
  bool isNotDead() const;
  void updateHead();
  void drawHead();
  void cleanTail();
  void drawAndClean();
  void printScore();
  void getDirection();
  void generateFood();
  void pushToVector();

  friend class Food;
  friend bool operator==(const Food &food, const Snake &snake);
  friend bool operator==(const Snake &snake, const Food &food);
  friend bool operator!=(const Food &food, const Snake &snake);
  friend bool operator!=(const Snake &snake, const Food &food);
};

#endif
