#include <TFT.h> // Hardware-specific library
#include <SPI.h>

#define LCS   8
#define LDC   7
#define LRESET 6

#define RCS   4
#define RDC   3
#define RRESET 2

TFT screenLeft = TFT(LCS, LDC, LRESET);
TFT screenRight = TFT(RCS, RDC, RRESET);

int emotionTime = 500;

void setBackground(int r, int g, int b) {
  screenLeft.background(b, g, r);
  screenRight.background(b, g, r);
}

void setColor(int r, int g, int b) {
  screenLeft.fill(b, g, r);
  screenLeft.stroke(b, g, r);
  screenRight.fill(b, g, r);
  screenRight.stroke(b, g, r);
}

void printCircle(float x, float y, int r) {
  int width = screenLeft.width(), height = screenLeft.height();
  screenLeft.circle(width - width * x, height * y, r);
  screenRight.circle(width - width * x, height - height * y, r);
}

void setup() {
  Serial.begin(9600);
  screenLeft.begin();
  screenRight.begin();
  neutral();
  Serial.println("I am ready");
}

void loop() {
  char in = '9';

  if (Serial.available() > 0) {
    in = Serial.read();
    Serial.print("Recieved ");
    Serial.println(in);
  }

  switch (in) {
    case '0':
      neutral();
      break;
    case '1':
      sad();
      break;
    case '2':
      happy();
      break;
    case '3':
      angry();
      break;
    case '4':
      scared();
      break;
    case '5':
      disgusted();
      break;
  }
}

void neutral () {
  setBackground(0, 0, 0);
  setColor(255, 255, 255);
  printCircle(0.5, 0.5, 50);
  setColor(0, 0, 0);
  printCircle(0.5, 0.5, 10);
}

// Input 1
void sad () {
  Serial.println("I am sad...");
  setBackground(0, 0, 0);
  int a = 80, b = 50;
  int as = 80 * 80, bs = 50 * 50;
  setColor(255, 255, 255);
  printCircle(0.5, 0.42, 30);
  setColor(0, 0, 255);
  printCircle(0.5, 0.35, 20);
  setColor(255, 255, 255);
  printCircle(0.48, 0.3, 5);
  printCircle(0.53, 0.3, 5);
  setColor(100, 100, 255);
  printCircle(0.55, 0.62, 5);
  printCircle(0.45, 0.62, 5);
  printCircle(0.5, 0.63, 5);
  printCircle(0.6, 0.7, 5);
  printCircle(0.61, 0.8, 5);
  delay(emotionTime);
}

// Input 2
void happy () {
  Serial.println("I am happy!");
  setBackground(0, 0, 0);
  setColor(255, 255, 0);
  printCircle(0.5, 0.5, 50);
  setColor(255, 255, 255);
  printCircle(0.5, 0.52, 45);
  setColor(255, 255, 0);
  printCircle(0.5, 0.6, 50);
  setColor(0, 0, 0);
  printCircle(0.5, 0.8, 60);
  delay(emotionTime);
}

// Input 3
void angry () {
  Serial.println("I AM ANGRY!!!!!!");
  setBackground(0, 0, 0);
  setColor(255, 255, 255);
  printCircle(0.4, 0.3, 55);
  setColor(255, 0, 0);
  printCircle(0.5, 0.4, 30);
  setColor(0, 0, 0);
  printCircle(0.3, 0, 70);
  delay(emotionTime);
}

// Input 4
void scared () {
  Serial.println("I'm scared");
  setBackground(0, 0, 0);
  setColor(255, 255, 255);
  printCircle(0.5, 0.5, 40);
  setColor(128, 0, 128);
  printCircle(0.5, 0.5, 35);
  setColor(0, 0, 0);
  printCircle(0.5, 0.5, 25);
  delay(emotionTime);
}

// Input 5
void disgusted () {
  Serial.println("I'm... dis-gusted...");
  setBackground(0, 0, 0);
  setColor(255, 255, 255);
  printCircle(0.666, 0.5, 40);
  setColor(0, 255, 0);
  printCircle(0.666, 0.5, 30);
  delay(emotionTime);
}
