#include <SoftwareSerial.h>

#define TX_PIN 2  // TX to LIN module
#define RX_PIN 3  // RX from LIN module
#define SLP_PIN 4 // Sleep pin

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

// Function to calculate parity bits for LIN ID
uint8_t calculateParity(uint8_t id) {
    uint8_t p0 = (id & 0x01) ^ ((id & 0x02) >> 1) ^ ((id & 0x04) >> 2) ^ ((id & 0x10) >> 4);
    uint8_t p1 = ~(((id & 0x02) >> 1) ^ ((id & 0x08) >> 3) ^ ((id & 0x10) >> 4) ^ ((id & 0x20) >> 5));
    p1 = (p1 & 0x01) << 1;
    return id | p0 | p1;
}

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

void sendLINFrame(uint8_t id, uint8_t* data, uint8_t length) {
    id = calculateParity(id);

    // Simulate Break field by sending a series of zeros
    linSerial.write((uint8_t)0x00);
    linSerial.write((uint8_t)0x00);
    linSerial.write((uint8_t)0x00);

    // Send Sync byte
    linSerial.write(0x55); // Sync byte

    // Send the ID with parity
    linSerial.write(id);   

    // Send Data bytes (empty for header request)
    for (int i = 0; i < length; i++) {
        linSerial.write(data[i]);
    }

    // Send Checksum (empty for header request, calculated for slave's response)
    uint8_t checksum = calculateChecksum(id, data, length);
    linSerial.write(checksum);
}

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600); // LIN baud rate
    Serial.begin(9600);    // Debugging

    Serial.println("Master Ready");
}

void loop() {
    uint8_t id = 0x17;  // Requesting data from slave with ID 0x17
    uint8_t message[] = {}; // No data for header request
    uint8_t length = sizeof(message);

    Serial.println("Sending LIN Frame Request...");
    sendLINFrame(id, message, length);  // Send header with no data

    delay(2000); // Wait for 2 seconds before sending again
}
