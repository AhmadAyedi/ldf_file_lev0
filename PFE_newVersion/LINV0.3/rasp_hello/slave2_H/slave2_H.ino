#include <SoftwareSerial.h>

#define TX_PIN 2  // TX to LIN module
#define RX_PIN 3  // RX from LIN module
#define SLP_PIN 4 // Sleep pin

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

// Function to calculate checksum
uint8_t calculateChecksum(uint8_t id, uint8_t* data, uint8_t length) {
    uint16_t sum = id; // Start with the ID

    // Add each data byte
    for (int i = 0; i < length; i++) {
        sum += data[i];
    }

    // Take the lower 8 bits and add the carry using bitwise operations
    sum = (sum & 0xFF) + (sum >> 8); // This simulates the "carry" by adding the upper byte to the lower byte.

    // Perform the bitwise NOT (invert all bits)
    return ~sum;
}

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600);
    Serial.begin(9600);

    Serial.println("Slave Ready");
}

void loop() {
    if (linSerial.available() > 8) { // LIN frame size for a header request
        // Read Break field (ignore)
        linSerial.read(); linSerial.read(); linSerial.read();

        // Read Sync byte
        uint8_t sync = linSerial.read();

        // Read ID
        uint8_t id = linSerial.read();
        
        // Validate the sync byte
        if (sync != 0x55) {
            Serial.println("Invalid Sync byte");
            return;
        }

        // Check if the request is for this slave (id matching)
        if (id == 0x17) {
            uint8_t data[5] = {'h', 'e', 'l', 'l', 'o'}; // Response data for ID 0x17 (example response "hello")

            // Send response frame
            linSerial.write(0x55); // Sync byte
            linSerial.write(id);   // ID with parity
            for (int i = 0; i < 5; i++) {
                linSerial.write(data[i]);
            }

            // Send checksum
            uint8_t checksum = calculateChecksum(id, data, 5);
            linSerial.write(checksum);

            Serial.println("Response Sent");
        } else {
            Serial.println("Unknown ID requested");
        }
    }
}
