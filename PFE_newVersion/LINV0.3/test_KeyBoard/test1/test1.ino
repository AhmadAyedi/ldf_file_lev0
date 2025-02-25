// Define the row and column pins of the keypad
#define ROWS 4
#define COLS 4
int rowPins[ROWS] = {8, 9, 10, 11};  // Rows
int colPins[COLS] = {4, 5, 6, 7};   // Columns

// Define the button map (for 16 buttons: S1 to S16)
char* keys[ROWS][COLS] = {  // Array of strings for button labels
  {"S1", "S2", "S3", "S4"},
  {"S5", "S6", "S7", "S8"},
  {"S9", "S10", "S11", "S12"},
  {"S13", "S14", "S15", "S16"}
};

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

  // Set row pins as output
  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);  // Disable all rows initially
  }

  // Set column pins as input with pull-up resistors
  for (int i = 0; i < COLS; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
}

void loop() {
  // Scan the keypad for button presses
  for (int r = 0; r < ROWS; r++) {
    digitalWrite(rowPins[r], LOW);  // Enable the row (set to LOW)
    
    for (int c = 0; c < COLS; c++) {
      if (digitalRead(colPins[c]) == LOW) {  // Check if button is pressed
        // Debounce the button press
        delay(50);
        
        // Display the full button value (like "S1", "S2", "S10", etc.)
        Serial.print("Button Pressed: ");
        Serial.println(keys[r][c]);  // Display the full value of the key pressed

        // Wait for the button to be released before checking again
        while (digitalRead(colPins[c]) == LOW);  // Wait for release
      }
    }
    digitalWrite(rowPins[r], HIGH);  // Disable the row (set to HIGH)
  }
}
