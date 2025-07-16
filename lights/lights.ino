
#include <Adafruit_NeoPixel.h>

#define IN_RED 2
#define IN_GREEN 3
#define IN_BLUE 4
#define IN_YELLOW 5

#define OUT_PIXELS 6

#define NUMPIXELS 8

#define SOFTPULSELEN 13

Adafruit_NeoPixel pixels(NUMPIXELS, OUT_PIXELS, NEO_GRB + NEO_KHZ800);

struct color {
  int red;
  int green;
  int blue;
};

struct color softPulseRed[SOFTPULSELEN];
struct color softPulseGreen[SOFTPULSELEN];
struct color softPulseBlue[SOFTPULSELEN];
struct color softPulseYellow[SOFTPULSELEN];

color lowRed = color{red: 5, green: 0, blue: 0};
color highRed = color{red: 150, green: 0, blue: 0};
color lowGreen = color{red: 0, green: 5, blue: 0};
color highGreen = color{red: 0, green: 150, blue: 0};
color lowBlue = color{red: 0, green: 0, blue: 5};
color highBlue = color{red: 0, green: 0, blue: 150};
color lowYellow = color{red: 3, green: 2, blue: 0};
color highYellow = color{red: 90, green: 60, blue: 0};

int fadeRedPos;
int fadeGreenPos;
int fadeBluePos;
int fadeYellowPos;

void setup() {

  pinMode(IN_RED, INPUT_PULLUP);
  pinMode(IN_GREEN, INPUT_PULLUP);
  pinMode(IN_BLUE, INPUT_PULLUP);
  pinMode(IN_YELLOW, INPUT_PULLUP);

  for (int i = 0; i < SOFTPULSELEN; i++) {
    softPulseRed[i].red = i * 3;
    softPulseGreen[i].green = i * 3;
    softPulseBlue[i].blue = i * 3;
    softPulseYellow[i].red = i * 2;
    softPulseYellow[i].green = i * 2;
  }

  pixels.begin();
  pixels.clear();

  // Starting fade positions; make it a circle!
  fadeRedPos = 0;
  fadeGreenPos = 2;
  fadeBluePos = 4;
  fadeYellowPos = 6;
}

void loop() {

  if (digitalRead(IN_RED) == HIGH) {
    selectionInit();
    selectRed();
  }

  if (digitalRead(IN_GREEN) == HIGH) {
    selectionInit();
    selectGreen();
  }

  if (digitalRead(IN_BLUE) == HIGH) {
    selectionInit();
    selectBlue();
  }

  if (digitalRead(IN_YELLOW) == HIGH) {
    selectionInit();
    selectYellow();
  }

  fadeCycle();
}

/*
  The default soft pulsating circle
*/
void fadeCycle() {
  // Fade everything up
  for (int j = 0; j < SOFTPULSELEN; j++) {
    setFades(j);    
    pixels.show();
    delay(100);
  }
  for (int j = SOFTPULSELEN-1; j >= 0; j--) {
    setFades(j);
    pixels.show();
    delay(100);
  }

  fadeRedPos = movePos(fadeRedPos);
  fadeGreenPos = movePos(fadeGreenPos);
  fadeBluePos = movePos(fadeBluePos);
  fadeYellowPos = movePos(fadeYellowPos);
}

int movePos(int pos) {
  pos++;
  if (pos >= NUMPIXELS) {
    pos = 0;
  }
  return pos;
}

void setFades(int j) {
  setColor(fadeRedPos, softPulseRed[j]);
  setColor(fadeGreenPos, softPulseGreen[j]);
  setColor(fadeBluePos, softPulseBlue[j]);
  setColor(fadeYellowPos, softPulseYellow[j]);
}

/*
  A few heavy blinks to really show:
  something is about to happen!
*/

void selectionInit() {

  for(int j = 0; j < 3; j++) {
    for(int i = 0; i < 200; i += 10) {
      color c = color{red: i, green: i, blue: i};
      setGlobalColor(c);
      pixels.show();
      delay(10);
    }

    delay(100);

    for(int i = 200; i > 0; i -= 10) {
      color c = color{red: i, green: i, blue: i};
      setGlobalColor(c);
      pixels.show();
      delay(20);
    }
  }

  
  setColor(0, lowRed);
  setColor(1, lowRed);

  
  setColor(2, lowGreen);
  setColor(3, lowGreen);

  setColor(4, lowBlue);
  setColor(5, lowBlue);

  setColor(6, lowYellow);
  setColor(7, lowYellow);

  pixels.show();
  delay(1000);

  for(int j = 6; j > 0; j--) {
    setColor(6, lowYellow);
    setColor(7, lowYellow);
    setColor(0, highRed);
    setColor(1, highRed);
    pixels.show();
    delay(150 * j);

    setColor(0, lowRed);
    setColor(1, lowRed);
    setColor(2, highGreen);
    setColor(3, highGreen);
    pixels.show();
    delay(150 * j);

    setColor(2, lowGreen);
    setColor(3, lowGreen);
    setColor(4, highBlue);
    setColor(5, highBlue);
    pixels.show();
    delay(150 * j);

    setColor(4, lowBlue);
    setColor(5, lowBlue);
    setColor(6, highYellow);
    setColor(7, highYellow);
    pixels.show();
    delay(150 * j);
  }

  setColor(6, lowYellow);
  setColor(7, lowYellow);
  pixels.show();
  delay(300);
  
  setColor(0, highRed);
  setColor(1, highRed);
  setColor(2, highGreen);
  setColor(3, highGreen);
  setColor(4, highBlue);
  setColor(5, highBlue);
  setColor(6, highYellow);
  setColor(7, highYellow);

  pixels.show();
  delay(300);  
}

void selectRed() {
  for(int i = 0; i < 15; i++) {

    color g = stepColorDown(highGreen, lowGreen, i);
    color b = stepColorDown(highBlue, lowBlue, i);
    color y = stepColorDown(highYellow, lowYellow, i);

    setColor(2, g);
    setColor(3, g);
    setColor(4, b);
    setColor(5, b);
    setColor(6, y);
    setColor(7, y);

    pixels.show();
    delay(250);
  }

  for(int i = 0; i < 15; i++) {
    color t = stepColorUp(highRed, lowRed, i);

    setColor(2, t);
    setColor(3, t);
    setColor(4, t);
    setColor(5, t);
    setColor(6, t);
    setColor(7, t);

    pixels.show();
    delay(200);
  }

  delay(5000);

  pixels.clear();
}

void selectGreen() {
  for(int i = 0; i < 15; i++) {

    color r = stepColorDown(highRed, lowRed, i);
    color b = stepColorDown(highBlue, lowBlue, i);
    color y = stepColorDown(highYellow, lowYellow, i);

    setColor(0, r);
    setColor(1, r);
    setColor(4, b);
    setColor(5, b);
    setColor(6, y);
    setColor(7, y);

    pixels.show();
    delay(250);
  }

  for(int i = 0; i < 15; i++) {
    color t = stepColorUp(highGreen, lowGreen, i);

    setColor(0, t);
    setColor(1, t);
    setColor(4, t);
    setColor(5, t);
    setColor(6, t);
    setColor(7, t);

    pixels.show();
    delay(200);
  }

  delay(5000);

  pixels.clear();
}

void selectBlue() {
  for(int i = 0; i < 15; i++) {

    color r = stepColorDown(highRed, lowRed, i);
    color g = stepColorDown(highGreen, lowGreen, i);
    color y = stepColorDown(highYellow, lowYellow, i);

    setColor(0, r);
    setColor(1, r);
    setColor(2, g);
    setColor(3, g);
    setColor(6, y);
    setColor(7, y);

    pixels.show();
    delay(250);
  }

  for(int i = 0; i < 15; i++) {
    color t = stepColorUp(highBlue, lowBlue, i);

    setColor(0, t);
    setColor(1, t);
    setColor(2, t);
    setColor(3, t);
    setColor(6, t);
    setColor(7, t);

    pixels.show();
    delay(200);
  }

  delay(5000);

  pixels.clear();
}

void selectYellow() {
  for(int i = 0; i < 15; i++) {

    color r = stepColorDown(highRed, lowRed, i);
    color g = stepColorDown(highGreen, lowGreen, i);
    color b = stepColorDown(highBlue, lowBlue, i);

    setColor(0, r);
    setColor(1, r);
    setColor(2, g);
    setColor(3, g);
    setColor(4, b);
    setColor(5, b);

    pixels.show();
    delay(250);
  }

  for(int i = 0; i < 15; i++) {
    color t = stepColorUp(highYellow, lowYellow, i);

    setColor(0, t);
    setColor(1, t);
    setColor(2, t);
    setColor(3, t);
    setColor(4, t);
    setColor(5, t);

    pixels.show();
    delay(200);
  }

  delay(5000);

  pixels.clear();
}



/*
  Helpers
*/
void setColor(int pos, color c) {
    // Yes, red and green are swapped. Some bug in either my code, or the library (must by the lib huh ;) )
    pixels.setPixelColor(pos, c.green, c.red, c.blue);
}

void setGlobalColor(color c) {
  for(int p = 0; p < NUMPIXELS; p++) {
    // Yes, red and green are swapped. Some bug in either my code, or the library (must by the lib huh ;) )
    pixels.setPixelColor(p, c.green, c.red, c.blue);
  }
}

color stepColorDown(color high, color low, int step) {
  return color{
    red: high.red - (low.red * step * 2),
    green: high.green - (low.green * step * 2),
    blue: high.blue - (low.blue * step * 2)
  };
}

color stepColorUp(color high, color low, int step) {
  return color{
    red: low.red + (low.red * step),
    green: low.green + (low.green * step),
    blue: low.blue + (low.blue * step)
  };
}