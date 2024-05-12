#include <Servo.h>

Servo myServo;  // Skapa ett servoobjekt

const int buttonPin = 2;  // Knappen är ansluten till pin 2
int buttonState = 0;      // Variabel för att lagra knappens tillstånd
bool state = false;
int inputValue = 0;
int i = 0;
int k = 0;

int receivedChar;
bool newData = false;

int motorPin = 7;

void setup() {
  Serial.begin(9600);
  myServo.attach(9);          // Koppla servot till pin 9
  pinMode(buttonPin, INPUT);  // Sätt knappens pinne till input
  pinMode(motorPin, OUTPUT);
  Serial.println("<Arduino is ready>");
}

void loop() {
  recvOneChar();
  showNewData();

  while (newData) {
    if (digitalRead(buttonPin)) {  // Läs av knappens tillstånd
      state = !state;
      newData = false;
    }
  }

  // Om knappen trycks ned (lägre än HIGH), aktivera servot
  if (state) {
    myServo.write(180);  // Rör servot till 180 grader
    while (k < 7) {
      while (i < receivedChar) {
        Serial.println(receivedChar);
        myServo.write((180 / (receivedChar - 1)) * i);
        delay(2000);
        digitalWrite(motorPin, HIGH);
        delay(50);
        digitalWrite(motorPin, LOW);
        delay(1000);
        i += 1;
      }
      k += 1;
      i = 0;
    }
  } else {
    i = 0;
    myServo.write(0);  // Återställ servot till 0 grader
  }
}

void recvOneChar() {
  if (Serial.available() > 0) {
    receivedChar = Serial.read() - '0';
    newData = true;
  }
}

void showNewData() {
  if (newData == true) {
    Serial.print("This just in ... ");
    Serial.println(receivedChar);
    //newData = false;
  }
}
