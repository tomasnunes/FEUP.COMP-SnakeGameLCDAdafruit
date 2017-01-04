#ifndef _SNAKE_H
#define _SNAKE_H

#include "Adafruit_ILI9340.h"

const uint8_t g_leftArrow(4), g_rightArrow(6), g_upArrow(5), g_downArrow(7);

class Snake {
private:
  int m_x, m_y, m_directionX, m_directionY, m_scale, m_score;
  bool m_notDead;
  Adafruit_ILI9340 *m_tft; //Pointer to our screen object (tft) so we can use it in our snake member functions.
                           //An alternative would be passing the tft object by reference to every method, if necessary.

public:
  Snake(Adafruit_ILI9340 *tft, int x=0, int y=0, int scale=7) : m_tft(tft), m_x(x), m_y(y), m_scale(scale) {
    m_directionX = 0;
    m_directionY = 1;
    m_score = 0;
    m_notDead = true;
  }

  bool isNotDead() const;
  void update();
  void drawAndUpdate();
  void printScore();
  void getDirection();
};

#endif
