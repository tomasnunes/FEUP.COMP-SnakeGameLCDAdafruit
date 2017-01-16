#include "SPI.h"
#include "Adafruit_ILI9340.h"
#include "Snake.h"
#include "Wire.h"

bool g_menuFlag(true), g_playGame(false), g_hardMode(false);

void printMenu();
void playGame();
void lockScreen();

//Demo from Adafruit Industries, copied from Adafruit_ILI9340 files!
void playDemo();
unsigned long testFillScreen();
unsigned long testText();
unsigned long testLines(uint16_t color);
unsigned long testFastLines(uint16_t color1, uint16_t color2);
unsigned long testRects(uint16_t color);
unsigned long testFilledRects(uint16_t color1, uint16_t color2);
unsigned long testFilledCircles(uint8_t radius, uint16_t color);
unsigned long testCircles(uint8_t radius, uint16_t color);
unsigned long testTriangles();
unsigned long testFilledTriangles();
unsigned long testRoundRects();
unsigned long testFilledRoundRects();

Adafruit_ILI9340 tft = Adafruit_ILI9340(g_cs, g_dc, g_mosi, g_sclk, g_rst, g_miso);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(g_leftArrow, INPUT);
  pinMode(g_rightArrow, INPUT);
  pinMode(g_upArrow, INPUT);
  pinMode(g_downArrow, INPUT);
  pinMode(g_ss, OUTPUT);
  digitalWrite(g_ss, HIGH);

  Wire.begin();
  tft.begin();

  tft.setTextColor(ILI9340_BLACK);
  tft.setTextSize(4);
  tft.setCursor(75, 120);
  tft.print("COMP");
  tft.setCursor(40, 160);
  tft.print("PROJECT");

  lockScreen();
}

void loop(void) {
  if(Serial.available()) {
    g_buffer = ' ';
    g_buffer = Serial.read();
  }

  if(!digitalRead(g_leftArrow) || g_buffer=='a' || g_buffer=='A') {
    playDemo();
    g_menuFlag = true;
  }
  else if(!digitalRead(g_upArrow) || g_buffer=='w' || g_buffer=='W' || (g_playGame && !g_hardMode)) {
    g_hardMode = false;
    g_menuFlag = true;
    playGame();
  }
  else if(!digitalRead(g_downArrow) || g_buffer=='s' || g_buffer=='S' || g_playGame) {
    g_hardMode = true;
    g_menuFlag = true;
    playGame();
  }
  else if(!digitalRead(g_downArrow) || g_buffer=='d' || g_buffer=='D') {
    g_menuFlag = true;
    lockScreen();
  }
  else if(g_menuFlag) {
    g_menuFlag = false;
    g_hardMode = false;
    printMenu();
  }

  g_buffer = ' ';
}

void printMenu() {
  tft.fillScreen(ILI9340_BLACK);

  cleanInput();

  tft.setCursor(80, 70);
  tft.setTextColor(ILI9340_RED); tft.setTextSize(3);
  tft.print("MENU");

  tft.setCursor(7, 190);
  tft.setTextColor(ILI9340_WHITE); tft.setTextSize(2);
  tft.println("   (UP) Game");
  tft.setCursor(7, 210);
  tft.println(" (DOWN) Hard Mode");
  tft.setCursor(7, 230);
  tft.println(" (LEFT) Demo");
  tft.setCursor(7, 250);
  tft.println("(RIGHT) Lock Screen");
}

void playGame() {
  tft.fillScreen(ILI9340_BLACK);

  tft.drawFastHLine(0, 296, g_width, ILI9340_WHITE);
  tft.setCursor(90, 304);
  tft.setTextColor(ILI9340_RED); tft.setTextSize(1);
  tft.print("Score: ");

  Snake snake(&tft, g_hardMode);
  Food food(&tft);
  int gameSpeed(100);

  snake.printScore();

  cleanInput();

  while(!snake.isDead()) {
    snake.getDirection();
    snake.updateHeadAndCheckWallHit(); //Check if hit wall, on Hard Mode.
    if(snake == food) {
      ++snake;
      do {
        food.genLocation();
      } while(food != snake);

      food.draw();

      //Updates Speed when snake grows
      if(!(snake.getSize()%5) && gameSpeed>30)
        gameSpeed -= 20;

      snake.drawHead();
      snake.pushToVector();
    }
    else {
      snake.ateItsOwnTail();
      if(!snake.isDead()) {
        snake.drawAndClean();
        delay(gameSpeed); //Controls the SPEED!
      }
    }
  }

  snake.dies();
  g_playGame = snake.playAgain();
}

void lockScreen() {
  tft.fillScreen(ILI9340_BLACK);

  //LOCK SCREEN (create function, put in lock screen after some time && eliminate time && add feup logo, micro sd)

  cleanInput();

  tft.setTextColor(ILI9340_RED);
  tft.setTextSize(4);
  tft.setCursor(77, 30);
  tft.print("FEUP");
  tft.setTextColor(ILI9340_WHITE);
  tft.setTextSize(2);
  tft.setCursor(38, 120);
  tft.print("COMP 2016/2017");
  tft.setCursor(38, 170);
  tft.print("Mafalda Varela");
  tft.setCursor(53, 190);
  tft.print("Tomas Nunes");
  tft.setTextColor(ILI9340_GREEN);
  tft.setCursor(25, 280);
  tft.print("Press any key...");

  do {
    if(Serial.available()) {
      g_buffer = ' ';
      g_buffer = Serial.read();
    }
  } while((g_buffer==' ' || g_buffer=='\0' || g_buffer=='\n') && digitalRead(g_leftArrow) && digitalRead(g_rightArrow) && digitalRead(g_upArrow) && digitalRead(g_downArrow));

  g_buffer = ' ';
}

void playDemo() {
  Serial.println(F("Benchmark                Time (microseconds)"));
  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
  delay(500);

  Serial.print(F("Text                     "));
  Serial.println(testText());
  delay(3000);

  Serial.print(F("Lines                    "));
  Serial.println(testLines(ILI9340_CYAN));
  delay(500);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(ILI9340_RED, ILI9340_BLUE));
  delay(500);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(ILI9340_GREEN));
  delay(500);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(ILI9340_YELLOW, ILI9340_MAGENTA));
  delay(500);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, ILI9340_MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, ILI9340_WHITE));
  delay(500);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());
  delay(500);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
  delay(500);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
  delay(500);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
  delay(500);

  Serial.println(F("Done!"));
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9340_BLACK);
  tft.fillScreen(ILI9340_RED);
  tft.fillScreen(ILI9340_GREEN);
  tft.fillScreen(ILI9340_BLUE);
  tft.fillScreen(ILI9340_BLACK);
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(ILI9340_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9340_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9340_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9340_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9340_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(ILI9340_BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(ILI9340_BLACK);

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(ILI9340_BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(ILI9340_BLACK);

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(ILI9340_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(ILI9340_BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9340_BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(ILI9340_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9340_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.Color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9340_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.Color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.Color565(i, i, 0));
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9340_BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.Color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9340_BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.Color565(0, i, 0));
  }

  return micros() - start;
}
