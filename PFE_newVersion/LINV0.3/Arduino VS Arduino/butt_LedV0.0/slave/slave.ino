#include <SoftwareSerial.h>

#define TX_PIN 2        // TX to LIN module
#define RX_PIN 3        // RX from LIN module
#define SLP_PIN 4       // Sleep pin
#define LED_PIN 6       // LED connected to pin 6

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT); // Set LED pin as output
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600);  // LIN baud rate
    Serial.begin(9600);     // Debugging

    Serial.println("Slave Ready");
}

void loop() {
    if (linSerial.available() > 1) {  // Check if data is available
        uint8_t sync = linSerial.read();
        uint8_t ledCommand = linSerial.read();

        if (sync == 0x55) {
            if (ledCommand == 0x01) {
                digitalWrite(LED_PIN, HIGH);  // Turn ON LED
                Serial.println("LED ON");
            } else if (ledCommand == 0x00) {
                digitalWrite(LED_PIN, LOW);  // Turn OFF LED
                Serial.println("LED OFF");
            }
        } else {
            Serial.println("Invalid LIN Sync Byte");
        }
    }
}
