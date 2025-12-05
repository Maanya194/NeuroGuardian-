#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// ----------------- LCD Setup -----------------
const int rs = 4, en = 5, d4 = A3, d5 = A2, d6 = A1, d7 = A0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// ----------------- DS18B20 Temperature Setup -----------------
#define ONE_WIRE_BUS 10
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float tempC = 0;

// ----------------- EEG Data -----------------
int signalQuality = 0;
int attention = 0;
int meditation = 0;

// ----------------- SoftwareSerial for ESP32 -----------------
#define ESP32_RX 2                             // Arduino RX pin (connect to ESP32 TX)
#define ESP32_TX 3                             // Arduino TX pin (connect to ESP32 RX)
SoftwareSerial espSerial(ESP32_RX, ESP32_TX);  // RX, TX
int buzzer = 12;
int relayPin = 11;  // Relay control pin
int motorIN1 = 6;
int motorIN2 = 7;
void parseEEGData(String str);
void setup() {
  Serial.begin(115200);     // Serial for Python
  espSerial.begin(115200);  // Serial to ESP32
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Realtime Fatigue");
  lcd.setCursor(0, 1);
  lcd.print("Detection Drivers");
  delay(2000);

  sensors.begin();
}

void loop() {
  // --- Read Temperature ---
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0);

  // --- Read EEG data from ESP32 ---
  while (espSerial.available()) {
    String data = espSerial.readStringUntil('\n');  // Expect format: aXXbYYcZZd
    // Serial.println(data);
    delay(1000);
    parseEEGData(data);
  }

  // --- Display on LCD ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(tempC, 1);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Signal:");
  lcd.print(signalQuality);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Attention:");
  lcd.print(attention);
  lcd.setCursor(0, 1);

  lcd.print("Meditation:");
  lcd.print(meditation);
  delay(1000);
  String data1 = "";
  while (Serial.available() > 0) {
    data1 = Serial.readString();
  }
  if (data1.indexOf('t') != -1 && data1.length() > 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("received ML data");
    delay(1000);

    int indexA = data1.indexOf("t") + 1;
    int indexB = data1.indexOf("u") + 1;
    int indexC = data1.indexOf("v") + 1;
    int indexD = data1.indexOf("w") + 1;
    int indexE = data1.indexOf("x") + 1;


    String a1 = data1.substring(indexA, indexB - 1);
    String b1 = data1.substring(indexB, indexC - 1);
    String c1 = data1.substring(indexC, indexD - 1);
    String d1 = data1.substring(indexD);




    int a = a1.toInt();
    int b = b1.toInt();
    int c = c1.toInt();
    int d = d1.toInt();


    if (a == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Head band not");
      lcd.setCursor(0, 1);
      lcd.print("Wear correctly");
      delay(1000);
      digitalWrite(relayPin, HIGH);
      digitalWrite(buzzer, HIGH);
      digitalWrite(motorIN1, LOW);
      digitalWrite(motorIN2, LOW);
      delay(1000);

    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Head band setting");
      lcd.setCursor(0, 1);
      lcd.print("Correct");
      delay(1000);
      digitalWrite(relayPin, HIGH);
      digitalWrite(buzzer, LOW);
      digitalWrite(motorIN1, HIGH);
      digitalWrite(motorIN2, LOW);
      delay(1000);
    }

    if (b == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Driver is less");
      lcd.setCursor(0, 1);
      lcd.print("focused fatigue");
      delay(1000);
      digitalWrite(motorIN1, LOW);
      digitalWrite(motorIN2, LOW);
      digitalWrite(relayPin, LOW);
      digitalWrite(buzzer, HIGH);
      delay(1000);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Driver is");
      lcd.setCursor(0, 1);
      lcd.print("focused");
      delay(1000);
      digitalWrite(motorIN1, HIGH);
      digitalWrite(motorIN2, LOW);
      digitalWrite(relayPin, HIGH);
      digitalWrite(buzzer, LOW);
      delay(1000);
    }

    if (c == 0) {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Driver is relax");
      lcd.setCursor(0, 1);
      lcd.print("or sleeping");
      delay(1000);
      digitalWrite(motorIN1, LOW);
      digitalWrite(motorIN2, LOW);
      digitalWrite(relayPin, LOW);
      digitalWrite(buzzer, HIGH);
      delay(1000);


    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Driver is less");
      lcd.setCursor(0, 1);
      lcd.print("fatigue");
      delay(1000);
      digitalWrite(motorIN1, HIGH);
      digitalWrite(motorIN2, LOW);
      digitalWrite(relayPin, HIGH);
      digitalWrite(buzzer, LOW);
      delay(1000);
    }

    if (d == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp high");
      delay(1000);
      digitalWrite(relayPin, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(1000);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp normal");
      delay(1000);
      digitalWrite(relayPin, HIGH);
      digitalWrite(buzzer, LOW);
      delay(1000);
    }
  }
  // // --- Send to Python ---
  // Serial.print(tempC, 1);
  // Serial.print(",");
  // Serial.print(signalQuality);
  // Serial.print(",");
  // Serial.print(attention);
  // Serial.print(",");
  // Serial.println(meditation);
  String str1 = "a" + String(signalQuality) + "b" + String(attention) + "c" + String(meditation) + "d" + String(tempC) + "e";
  Serial.println(str1);
  delay(1000);
}

// ----------------- Parse EEG Data -----------------
void parseEEGData(String str) {
  int idxA = str.indexOf("a");
  int idxB = str.indexOf("b");
  int idxC = str.indexOf("c");
  int idxD = str.indexOf("d");

  if (idxA != -1 && idxB != -1 && idxC != -1 && idxD != -1) {
    signalQuality = str.substring(idxA + 1, idxB).toInt();
    attention = str.substring(idxB + 1, idxC).toInt();
    meditation = str.substring(idxC + 1, idxD).toInt();
  }
}
