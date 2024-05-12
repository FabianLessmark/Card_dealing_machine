/*
fil: servo_and_dealermotor.ino
skapare: Fabian Lessmark
datum: 2024-05-12
beskrivning: Programmet får en servo att snurra runt till angivet antal spelare och en motor att dela ut sju kort var till de angivna antalet speare.
*/

/*
biblotek inkluderas
*/
#include <Servo.h>

Servo myServo;  // Skapa ett servoobjekt

const int buttonPin = 2;  // Knappen är ansluten till pin 2
int buttonState = 0;      // Variabel för att lagra knappens tillstånd
bool state = false;       // Variabel för att hålla koll på knappens tillstånd (på/av)
int inputValue = 0;
int i = 0;                // sätta variabeln i till 0
int k = 0;                // sätta variabeln k till 0

int receivedChar;         // Variabel för att lagra mottagna tecken från serien
bool newData = false;     // Flagga för ny data från serien

int motorPin = 7;         // Pin för motorstyrning

void setup() {
  Serial.begin(9600);              
  myServo.attach(9);                // Koppla servot till pin 9
  pinMode(buttonPin, INPUT);        // Sätt knappens pin till input
  pinMode(motorPin, OUTPUT);        // Sätt motorns pin till output
  Serial.println("<Arduino is ready>");  // Skriv ut att Arduino är redo för att ta emot input
}

void loop() {
  recvOneChar();  // Kolla efter och läs in ett tecken från serien
  showNewData();  // Visa det mottagna tecknet på serien

  while (newData) {  // Om ny data mottogs
    if (digitalRead(buttonPin)) {  // Läs av knappens tillstånd
      state = !state;  // Ändra knappens tillstånd (på/av)
      newData = false;  // Återställ flaggan för ny data
    }
  }
/*
  Tar inputvärdet och upprepar processen att dela ut kort, aktivera motorn, 7 gånger till så många personer som inputvärdet gav
*/
  // Om knappen trycks ned (lägre än HIGH), aktivera servot
  if (state) {
    myServo.write(180);  // Rör servot till 180 grader
    while (k < 7) {  // Upprepa processen 7 gånger
      while (i < receivedChar) {  // Upprepa rörelsen baserat på mottagna tecknet
        Serial.println(receivedChar);  // Skriv ut mottaget tecken på serien
        myServo.write((180 / (receivedChar - 1)) * i);  // Rör servot till rätt position
        delay(2000);  // Vänta 2 sekunder
        digitalWrite(motorPin, HIGH);  // Slå på motorn
        delay(50);  // Vänta 50 millisekunder
        digitalWrite(motorPin, LOW);  // Stäng av motorn
        delay(1000);  // Vänta 1 sekund
        i += 1;  // Öka räknaren för rörelser
      }
      k += 1;  // Öka räknaren för upprepningar
      i = 0;   // Återställ räknaren för rörelser
    }
  } else {
    i = 0;  // Återställ räknaren för rörelser
    myServo.write(0);  // Återställ servot till 0 grader
  }
}

// Funktion för att ta emot ett tecken från serien
void recvOneChar() {
  if (Serial.available() > 0) {
    receivedChar = Serial.read() - '0';  // Läs in och konvertera tecknet till ett heltal
    newData = true;  // Sätt flaggan för ny data till sant
  }
}

// Funktion för att visa ny mottagen data på serien
void showNewData() {
  if (newData == true) {
    Serial.print("This just in ... ");  // Skriv ut meddelande om ny data
    Serial.println(receivedChar);       // Skriv ut det mottagna tecknet
    //newData = false;  // Återställ flaggan för ny data
  }
}
