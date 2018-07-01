#include <Servo.h>

// Create servos
Servo servoBase;
Servo servoHead;

#define trig 9
#define echo 8

#define mic 2

#define sb 11
#define sh 12

long timeSinceLast = 0;
int c = 0;

// Define all moods
enum mood {
  neutral,
  sad,
  happy,
  angry,
  scared,
  disgusted
};

// Define distance stuff
long distance = 0;
const int numDistanceReadings = 5;
long distanceReadings[numDistanceReadings];
int distanceIndex = 0;
long totalDistance = 0;
long averageDistance = 0;
long timeSinceLastDistance = 0;

// Define sound stuff
long loudTime = 0;
long loudStart = 0;
long quietTime = 0;
long quietStart = 0;
const long scaredMaxLoudTime = 250;
const long disgustedMinLoudTime = 2000;
boolean lastLoud = false;
boolean loudBang = false;
boolean longSound = false;

// Define current mood
mood currentMood = neutral, lastMood = neutral;

// Set sad distance in cm
const int sadMinDistance = 120;

// Set happy distance in cm
const int happyDistance = 100;
const int happyMinDistance = 60;

// Set angry distance in cm
const int angryDistance = 50;

// Set time in ms of loud for being disgusted
const int disgustLoudTime = 2000;

// Set time in ms that it will accept quiet without counting it into calculations
const int maxNonLoudTime = 100;

// Set minimum times for all moods in ms
const int happyTime = 2500;
const int sadTime = 2500;
const int angryTime = 2500;
const int scaredTime = 2500;
const int disgustedTime = 2500;
int feelingTime = 0;

// Set head stuff
const int servoTurnPos = 0;
const int servoNodPos = 0;
int servoArrayPosTurn = 0;
int servoFromTurn = 0;
long servoStartTimeTurn = 0;

// Array with pos and time
const int servoTurnSad[][2] = {{40, 1000}, {0, 1000}, {40, 1000}, {0, 1000}};
const int servoTurnHappy[][2] = {{20, 500}, {0, 500}, {180, 1000}, {0, 500}};
const int servoTurnAngry[][2] = {{30, 250}, {0, 250}, {0, 500}, {0, 500}};
const int servoTurnScared[][2] = {{0, 500}, {0, 500}, {0, 500}, {0, 500}};
const int servoTurnDisgusted[][2] = {{180, 1000}, {0, 5000}, {180, 1000}, {0, 5000}};

void setup() {
  Serial.begin(9600);
  // Print a 0 to show that it is starting
  Serial.println("0");

  // Define distance ports
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Define mic port
  pinMode(mic, INPUT);

  // Set distance array values to zero
  for (int thisReading = 0; thisReading < numDistanceReadings; thisReading++) {
    distanceReadings[thisReading] = 0;
  }

  // Define the servo ports
  servoBase.attach(sb);
  servoHead.attach(sh);

  // And on the seventh day God rested
  delay(100);
  // Print a 0 to show that it is ready
  Serial.println("0");
}

void loop() {
  // Start deltaTime timer
  int deltaTime = millis();

  // Update the range of the sound sensor
  getRange(trig, echo);

  // Get the mean distance
  distance = getMeanDistance();

  // Update sound input
  getSound();

  // If the last emotion has had its full time, set the new emotion
  if (loudBang ) {
    // Be scared
    currentMood = scared;
    feelingTime = scaredTime;
  } else if (longSound) {
    // Be disgusted
    currentMood = disgusted;
    feelingTime = disgustedTime;
  } else {
    if (feelingTime < 0) {
      // Distance code
      if (distance < angryDistance) {
        // Be angry
        currentMood = angry;
        feelingTime = angryTime;
      } else if (distance < happyDistance) {
        // Be happy
        currentMood = happy;
        feelingTime = happyTime;
      } else {
        // Be sad
        currentMood = sad;
        feelingTime = sadTime;
      }
    }
  }

  // If the new emotion is not the same as last one, change
  if (currentMood != lastMood) {
    servoArrayPosTurn = 0;
    servoStartTimeTurn = millis();
    switch (currentMood) {
      case sad:
        Serial.write('1');
        break;
      case happy:
        Serial.write('2');
        break;
      case angry:
        Serial.write('3');
        break;
      case scared:
        Serial.write('4');
        break;
      case disgusted:
        Serial.write('5');
        break;
    }
  }

  moveServoTurn();

  // Set last emotion to current one
  lastMood = currentMood;

  // Set deltaTime to time of the loop
  deltaTime = millis() - deltaTime;
  feelingTime = feelingTime - deltaTime;
  timeSinceLast += deltaTime;
}

// Return 0 if not loud, 1 if short loud, 2 if long loud
void getSound() {
  boolean val = digitalRead(mic);
  longSound = false;
  loudBang = false;

  boolean loud = false;

  if (val == 0) {
    loud = true;
  } else {
    loud = false;
  }

  if (loud == true) {
    // This runs if it is currently loud
    if (loudStart > 0) {
      loudTime = millis() - loudStart;
    } else {
      // If loud now but not before, start loud timer
      loudStart = millis();
    }
    quietStart = 0;
  } else {
    // This runs if it is currently not loud
    if (quietStart > 0) {
      quietTime = millis() - quietStart;
      // Check if it is just a minor temporary quite time
      if (quietTime > maxNonLoudTime) {
        if (loudTime < scaredMaxLoudTime && loudTime > 0) {
          loudBang = true;
        }
        loudTime = 0;
        loudStart = 0;
      }
    } else {
      // If quiet now but not before, start quiet timer
      quietStart = millis();
    }
  }

  if (loudTime > disgustLoudTime) {
    longSound = true;
  }

  lastLoud = loud;
}

void getRange(int t, int e) {
  long d = distance;
  if (millis() - timeSinceLastDistance > 50) {
    digitalWrite(t, LOW);
    delayMicroseconds(5);
    digitalWrite(t, HIGH);
    delayMicroseconds(5);
    digitalWrite(t, LOW);

    long duration = pulseIn(e, HIGH);
    delay(5);

    d = duration * 0.034 / 2;
    timeSinceLastDistance = millis();

    totalDistance = totalDistance - distanceReadings[distanceIndex];

    distanceReadings[distanceIndex] = d;

    totalDistance = totalDistance + distanceReadings[distanceIndex];

    distanceIndex = distanceIndex + 1;

    if (distanceIndex >= numDistanceReadings) {
      distanceIndex = 0;
    }
  }
}

long getMeanDistance() {
  return averageDistance = totalDistance / numDistanceReadings;
}

void moveServoTurn() {
  long bm = 0;
  long bt = 0;
  long a = 0;
  long t = millis() - servoStartTimeTurn;


  switch (currentMood) {
    case sad:
      bm = servoTurnSad[servoArrayPosTurn][0];
      bt = servoTurnSad[servoArrayPosTurn][1];
      a = 4;
      break;
    case happy:
      bm = servoTurnHappy[servoArrayPosTurn][0];
      bt = servoTurnHappy[servoArrayPosTurn][1];
      a = 4;
      break;
    case angry:
      bm = servoTurnAngry[servoArrayPosTurn][0];
      bt = servoTurnAngry[servoArrayPosTurn][1];
      a = 4;
      break;
    case scared:
      bm = servoTurnScared[servoArrayPosTurn][0];
      bt = servoTurnScared[servoArrayPosTurn][1];
      a = 4;
      break;
    case disgusted:
      bm = servoTurnDisgusted[servoArrayPosTurn][0];
      bt = servoTurnDisgusted[servoArrayPosTurn][1];
      a = 4;
      break;
  }
  
  if (t > bt) {
    servoFromTurn = bm;
    servoArrayPosTurn = (servoArrayPosTurn + 1) % a;
    servoStartTimeTurn = millis();
  } else {
    long m = bm * t / bt;
    long f = servoFromTurn * (1 - ((float)t / (float)bt));
    servoBase.write(m + f);
  }
}
