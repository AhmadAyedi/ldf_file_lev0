#include <SoftwareSerial.h>

#define TX_PIN 2        // TX to LIN module
#define RX_PIN 3        // RX from LIN module
#define SLP_PIN 4       // Sleep pin
#define BUTTON_PIN 5    // Button connected to pin 5

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Button pin set to input pull-up
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600);  // LIN baud rate
    Serial.begin(9600);     // Debugging

    Serial.println("Master Ready");
}

void loop() {
    static bool ledState = false; // Store the LED state
    uint8_t message[1];  // Data to send (ON or OFF)

    // Check if button is pressed
    if (digitalRead(BUTTON_PIN) == LOW) {
        ledState = !ledState;  // Toggle the LED state
        message[0] = ledState ? 0x01 : 0x00;  // ON (0x01) or OFF (0x00)
        Serial.println(ledState ? "Sending ON Command" : "Sending OFF Command");

        linSerial.write(0x55);  // Sync byte
        linSerial.write(message[0]); // Send LED state directly

        delay(200);  // Debounce delay
    }

    delay(100); // Prevent flooding the LIN bus
}
