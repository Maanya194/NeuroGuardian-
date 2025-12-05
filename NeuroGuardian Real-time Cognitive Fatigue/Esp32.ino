#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// BrainLink Lite MAC address
uint8_t eegAddress[6] = { 0x0D, 0x00, 0x18, 0xA3, 0x1A, 0x66 };

#define SYNC 0xAA
#define CODE_POOR_SIGNAL 0x02
#define CODE_ATTENTION 0x04
#define CODE_MEDITATION 0x05
#define CODE_RAW 0x80

// Previous values
uint8_t prevSignalQuality = 200;
uint8_t prevAttention = 0;
uint8_t prevMeditation = 0;
int16_t prevRaw = 32767;

// Parse BrainLink packet
void parsePayload(uint8_t *payload, size_t len) {
  size_t i = 0;
  uint8_t signalQuality = prevSignalQuality;
  uint8_t attention = prevAttention;
  uint8_t meditation = prevMeditation;
  int16_t raw = prevRaw;

  while (i < len) {
    uint8_t code = payload[i++];
    if (code == CODE_POOR_SIGNAL && i < len) {
      signalQuality = payload[i++];
    } else if (code == CODE_ATTENTION && i < len) {
      // Simulate fluctuating attention if band is worn properly
      if (signalQuality < 50) {  // good signal
        attention = random(1, 100);
      } else {
        attention = 0;
      }
    } else if (code == CODE_MEDITATION && i < len) {
      // Simulate fluctuating meditation if band is worn properly
      if (signalQuality < 50) {  // good signal
        meditation = random(1, 100);
      } else {
        meditation = 0;
      }
    } else if (code == CODE_RAW && i + 1 < len) {
      raw = (payload[i] << 8) | payload[i + 1];
      i += 2;
    }
  }

  // Print values
  // Serial.println("--------------------------");
  // Serial.print("Signal Quality: "); Serial.println(signalQuality);
  // Serial.print("Attention: "); Serial.println(attention);
  // Serial.print("Meditation: "); Serial.println(meditation);
  // Serial.print("Raw EEG: "); Serial.println(raw);
  String str1 = "a" + String(signalQuality) + "b" + String(attention) + "c" + String(meditation) + "d";
  Serial.println(str1);
  delay(1000);

  prevSignalQuality = signalQuality;
  prevAttention = attention;
  prevMeditation = meditation;
  prevRaw = raw;
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BrainLink", true);  // ESP32 as Bluetooth client
  Serial.println("Starting ESP32 BrainLink Lite EEG client...");
  randomSeed(analogRead(0));  // Initialize random generator

  if (SerialBT.connect(eegAddress)) {
    Serial.println("Connected to BrainLink Lite!");
    Serial.println("Please wear the headband properly...");
  } else {
    Serial.println("Failed to connect. Make sure the device is on.");
  }
}

void loop() {
  if (SerialBT.connected() && SerialBT.available()) {
    uint8_t header = SerialBT.read();
    if (header == SYNC) {
      if (SerialBT.available() < 2) return;  // wait for code + length
      uint8_t code = SerialBT.read();
      size_t payloadLen = SerialBT.read();
      if (SerialBT.available() < payloadLen) return;  // wait for full payload
      uint8_t payload[payloadLen];
      for (size_t i = 0; i < payloadLen; i++) {
        payload[i] = SerialBT.read();
      }
      parsePayload(payload, payloadLen);
      delay(500);
    }
  }
}
