#include <SoftwareSerial.h>
#include <DHT11.h>

#define TX_PIN 2  // TX to LIN module
#define RX_PIN 3  // RX from LIN module
#define SLP_PIN 4 // Sleep pin for LIN transceiver
#define DHT_PIN 7 // DHT11 sensor connected to pin 7

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX
DHT11 dht11(DHT_PIN); // Initialize DHT11 sensor

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600);  // LIN baud rate
    Serial.begin(9600);      // Debugging

    Serial.println("Slave Ready");
}

void loop() {
    if (linSerial.available() >= 5) {  // Wait for a complete header (break + sync + ID)
        // Read and ignore break field
        linSerial.read(); linSerial.read(); linSerial.read();

        uint8_t sync = linSerial.read();
        uint8_t id = linSerial.read();

        if (sync == 0x55 && id == 0x20) { // Check for valid sync and correct ID
            int temperature = 0;
            int humidity = 0;

            int result = dht11.readTemperatureHumidity(temperature, humidity);

            if (result == 0) { // Check if the temperature reading is valid
                delay(50); // Small delay for synchronization
                linSerial.write((uint8_t)temperature); // Send temperature as a single byte
                Serial.print("Sent Temperature: ");
                Serial.print(temperature);
                Serial.println("°C");
            } else {
                Serial.print("DHT11 Error: ");
                Serial.println(DHT11::getErrorString(result));
            }
        }
    }
}
