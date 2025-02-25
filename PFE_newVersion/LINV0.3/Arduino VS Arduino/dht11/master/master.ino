#include <SoftwareSerial.h>

#define TX_PIN 2  // TX to LIN module
#define RX_PIN 3  // RX from LIN module
#define SLP_PIN 4 // Sleep pin for LIN transceiver

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600);  // LIN baud rate
    Serial.begin(9600);      // Debugging

    Serial.println("Master Ready");
}

void loop() {
    uint8_t id = 0x20;  // LIN ID for temperature request

    // Send LIN header (break, sync, ID)
    linSerial.write((uint8_t)0x00); // Break field
    linSerial.write((uint8_t)0x00);
    linSerial.write((uint8_t)0x00);
    linSerial.write(0x55); // Sync byte
    linSerial.write(id);    // ID for temperature request

    delay(100); // Small delay for synchronization

    // Clear the serial buffer to avoid reading stale data
    while (linSerial.available()) {
        linSerial.read();
    }

    // Wait for a response from the slave
    unsigned long timeout = millis() + 1000; // 1-second timeout
    while (millis() < timeout) {
        if (linSerial.available()) {
            uint8_t temperature = linSerial.read(); // Read temperature data
            Serial.print("Received Temperature: ");
            Serial.print(temperature);
            Serial.println("°C");
            break;
        }
    }

    delay(2000); // Delay between requests
}
