// Define left and right wheel ports
#define N1 2
#define N2 3
#define N3 4
#define N4 5

// Define left and right speed ports
#define LS 9
#define RS 10

int emotion = 0;

void setup() {
  // Initiate ports
  pinMode(N1, OUTPUT);
  pinMode(N2, OUTPUT);
  pinMode(N3, OUTPUT);
  pinMode(N4, OUTPUT);
  pinMode(LS, OUTPUT);
  pinMode(RS, OUTPUT);

  // Set wheel speeds from 0 to 255
  analogWrite(LS, 180);
  analogWrite(RS, 180);

  Serial.begin(9600);
  Serial.println("I am ready");
}

void loop() {
  char in = '9';

  // Look for new command
  if (Serial.available() > 0) {
    in = Serial.read();
    Serial.print("Recieved ");
    Serial.println(in);
  }

  switch (in) {
    case '0':
      neutral();
      emotion = 0;
      break;
    case '1':
      sad();
      emotion = 1;
      break;
    case '2':
      happy();
      emotion = 2;
      break;
    case '3':
      angry();
      emotion = 3;
      break;
    case '4':
      scared();
      emotion = 4;
      break;
    case '5':
      disgusted();
      emotion = 5;
      break;
    default:
      if (emotion == 2) {
        happy();
      }
      break;
  }
}

void neutral() {
  stopAll();
}

void sad() {
  // Do nothing
}

void happy() {
  leftReverse();
  rightForward();
  delay(250);
  rightReverse();
  leftForward();
  delay(500);
  leftReverse();
  rightForward();
  delay(250);
  stopAll();
}

void angry() {
  forward();
  delay(500);
  forward();
  delay(1000);
  stopAll();
}

void scared() {
  for (int i = 0; i < 5; i++) {
    rightReverse();
    delay(100);
    rightStop();
    leftReverse();
    delay(100);
    leftStop();
  }
}

void disgusted() {
  rightReverse();
  delay(250);
  stopAll();
  delay(1000);
  rightForward();
  delay(250);
  stopAll();
}

void forward() {
  leftForward();
  rightForward();
}

void reverse() {
  leftReverse();
  rightReverse();
}

void stopAll() {
  leftStop();
  rightStop();
}

void leftStop() {
  digitalWrite(N1, LOW);
  digitalWrite(N2, LOW);
  Serial.println("Stop left");
}

void rightStop() {
  digitalWrite(N3, LOW);
  digitalWrite(N4, LOW);
  Serial.println("Stop right");
}

void leftForward() {
  digitalWrite(N1, HIGH);
  digitalWrite(N2, LOW);
  Serial.println("Forward left");
}

void rightForward() {
  digitalWrite(N3, HIGH);
  digitalWrite(N4, LOW);
  Serial.println("Forward right");
}

void leftReverse() {
  digitalWrite(N1, LOW);
  digitalWrite(N2, HIGH);
  Serial.println("Reverse left");
}

void rightReverse() {
  digitalWrite(N3, LOW);
  digitalWrite(N4, HIGH);
  Serial.println("Reverse right");
}
