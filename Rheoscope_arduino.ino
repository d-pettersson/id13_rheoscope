#include "LiquidCrystal.h"
#include "DHT.h"

// Sensor
#define DHTPIN 10
#define DHTTYPE DHT11

// Motor
#define FORWARD 256
#define BACKWARD 257
#define MOTORPIN 3
#define MOTORBRAKE 9
#define MOTORREVERSE 12

// LED
#define GREENLED 2

// Screen
const int rs = A5, en = A4, d4 = A3, d5 = A2, d6 = A1, d7 = A0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char line0[16];
char line1[16];

// Sensor
DHT dht(DHTPIN, DHTTYPE);

// Motor 
bool isMotorInverted = false;
bool isBrakeActive = false;

// Serial
const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;

int dataNumber = 0;

// Setup && Loop
void setup() {
  pinMode(MOTORPIN, OUTPUT);
  pinMode(MOTORBRAKE, OUTPUT);
  pinMode(MOTORREVERSE, OUTPUT);

  pinMode(GREENLED, OUTPUT);

  while (!Serial);
  Serial.begin(9600);
  Serial.println("Arduino ready.");

  lcd.begin(16, 2);
  lcd.clear();
  Serial.println("LCD initialised.");
  
  dht.begin();
  Serial.println("Sensors activated.");

  digitalWrite(GREENLED, HIGH);
}

void loop() {
  recvWithEndMarker();
  
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  if (newData == true) {
    runMotor(temperature, humidity);
  }

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  char float_str[8];
  dtostrf(temperature,4,2,float_str);
  sprintf(line0, "Temp: %-7sC", float_str);
  
  dtostrf(humidity,4,2,float_str);
  sprintf(line1, "Hum: %-8sC", float_str);
  
  updateDisplay();
}

// Custom functions
// Motor
void runMotor(float tempCoef, float humCoef) {
  dataNumber = 0;
  dataNumber = atoi(receivedChars);
  dataNumber -= int(dataNumber * (tempCoef * 0.01) * (humCoef * 0.01));

  switch (dataNumber) {
    case FORWARD:
      digitalWrite(MOTORREVERSE, LOW);
      break;
    case BACKWARD:               
      digitalWrite(MOTORREVERSE, HIGH);
      break;
  }

  analogWrite(MOTORPIN, dataNumber);
}

// LCD
void updateDisplay() {
  lcd.setCursor(0,0);
  lcd.print(line0);
  lcd.setCursor(0,1);
  lcd.print(line1);
}
 
// Serial
void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  if (Serial.available() > 0) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

// Debugging function for Serial Monitor
void showNewNumber() {
  if (newData == true) {
    dataNumber = 0;             // new for this version
    dataNumber = atoi(receivedChars);   // new for this version
    Serial.print("This just in ... ");
    Serial.println(receivedChars);
    Serial.print("Data as Number ... ");    // new for this version
    Serial.println(dataNumber);     // new for this version
    newData = false;
  }
}
