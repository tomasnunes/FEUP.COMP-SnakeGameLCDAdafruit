#ifndef _FOOD_H
#define _FOOD_H

#include "Adafruit_ILI9340.h"
#include <stdlib.h>
#include <time.h>

class Snake;

class Food {
private:
  int m_gridX {0}, m_gridY {0}, m_gridWidth, m_gridHeight, m_scale; //m_radius;
  Adafruit_ILI9340 *m_tft;

  static int randNumber(int min, int max);

public:
  Food(Adafruit_ILI9340 *tft, int scale=8) : m_tft(tft), m_scale(scale) {
    srand(time(NULL));

    m_gridWidth = g_width/m_scale;
    m_gridHeight = (g_heigth-24)/m_scale;

    updateAndDraw();
  }

  void genLocation();
  void updateAndDraw();

  friend bool operator==(const Food &food, const Snake &snake);
  friend bool operator==(const Snake &snake, const Food &food);
  friend bool operator!=(const Food &food, const Snake &snake);
  friend bool operator!=(const Snake &snake, const Food &food);
};

#endif
