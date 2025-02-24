#include <SoftwareSerial.h>

SoftwareSerial linSerial(3, 2);  // RX, TX for communication

void setup() {
  linSerial.begin(9600);  // Initialize the serial communication
  Serial.begin(9600);      // Initialize the serial monitor
}

void loop() {
  if (linSerial.available()) {
    byte received = linSerial.read();
    if (received == 0x01) {
      Serial.println("Hello from Raspberry Pi - Arduino 1");
    }
    if (received == 0x02) {
      Serial.println("Hello from Raspberry Pi - Arduino 2");
    }
  }
}
