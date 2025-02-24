#include <SoftwareSerial.h>

#define TX_PIN 2  // Connect to UART to LIN module TX
#define RX_PIN 3  // Connect to UART to LIN module RX
#define SLP_PIN 4 // Sleep pin to wake up the LIN module

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600); // LIN bus communication
    Serial.begin(9600); // Debugging

    Serial.println("Slave 1 Ready");
}

void loop() {
    if (linSerial.available()) {
        String receivedData = linSerial.readStringUntil('\n'); // Read until newline
        int delimiterIndex = receivedData.indexOf(':');
        
        if (delimiterIndex > 0) {
            String id = receivedData.substring(0, delimiterIndex);
            String message = receivedData.substring(delimiterIndex + 1);

            // Check if the ID matches Slave 1 (ID 3 in the LDF for Slave 1)
            if (id == "3") {
                Serial.println("Received by Slave 1: " + message);
            }
        }
    }
}
