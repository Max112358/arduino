#include <Mouse.h>

//make sure you install the sparkfun libraries
//Install Pro Micro Support:
//Open Arduino IDE → File → Preferences
//In Additional Boards Manager URLs, add:
//https://raw.githubusercontent.com/sparkfun/Arduino_Boards/main/IDE_Board_Manager/package_sparkfun_index.json
//Go to Tools → Board → Boards Manager, search for "SparkFun AVR Boards", and install it.
//Select the Board:
//Tools → Board → SparkFun Pro Micro
//Processor: ATmega32U4 (5V, 16MHz)

const int buttonPin = 3;    // Using D3 for the button (this is also INT1 on Arduino Leonardo/Micro)
volatile boolean mouseActive = false; // whether the sequence is active
unsigned long lastDebounceTime = 0;  // for debouncing

// Positions for the two click locations (relative to corner)
//first button, increase sales count
const int pos1X = -180;  // X offset from corner for first position
const int pos1Y = -265;  // Y offset from corner for first position

//second button, hit sell button
const int pos2X = -100;  // X offset from corner for second position
const int pos2Y = -50;   // Y offset from corner for second position

void setup() {
  // Initialize button pin as input with internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Attach interrupt to the button pin
  // FALLING for when the button is pressed (goes from HIGH to LOW with pull-up)
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING);

  // Initialize mouse library
  Mouse.begin();

  // Small delay at startup
  delay(1000);
}

// Interrupt Service Routine for button press
void buttonInterrupt() {
  // Debounce
  unsigned long currentTime = millis();
  if (currentTime - lastDebounceTime > 50) {  // 50ms debounce time
    mouseActive = !mouseActive;  // Toggle mouse active state
    lastDebounceTime = currentTime;
  }
}

// Function to move mouse by a specific distance in steps
void moveMouseBy(int deltaX, int deltaY) {
  // Calculate how many steps are needed
  const int stepSize = 25;  // Maximum pixels per step
  int stepsX = abs(deltaX) / stepSize + 1;
  int stepsY = abs(deltaY) / stepSize + 1;
  int steps = max(stepsX, stepsY);

  // Calculate movement per step
  float stepX = (float)deltaX / steps;
  float stepY = (float)deltaY / steps;

  // Move in small increments
  for (int i = 0; i < steps; i++) {
    if (!mouseActive) return;  // Check flag and exit if deactivated

    Mouse.move(round(stepX), round(stepY), 0);
    delay(5);  // Small delay between moves
  }
  delay(100);  // Allow time for the cursor to settle
}

// Function to move cursor to bottom-right corner
void moveToCorner() {
  // Make multiple large moves to ensure we hit the corner
  for (int i = 0; i < 10; i++) {
    if (!mouseActive) return;  // Check flag and exit if deactivated

    Mouse.move(127, 127, 0);
    delay(10);
  }
  delay(100);
}

void loop() {
  // Only perform mouse actions if active
  if (mouseActive) {
    // First, move to bottom right corner
    moveToCorner();
    if (!mouseActive) return;  // Early exit if deactivated

    // Move to position 1 (relative to corner)
    moveMouseBy(pos1X, pos1Y);
    if (!mouseActive) return;  // Early exit if deactivated

    // Click position 1 twenty-five times
    for (int i = 0; i < 25; i++) {
      if (!mouseActive) return;  // Early exit if deactivated

      Mouse.click(MOUSE_LEFT);
      delay(50);  // Short delay between clicks
    }

    if (!mouseActive) return;  // Early exit if deactivated

    // Move to position 2 (relative to position 1)
    moveMouseBy(pos2X, pos2Y);
    if (!mouseActive) return;  // Early exit if deactivated

    // Click position 2 once
    Mouse.click(MOUSE_LEFT);

    // Wait for 20 seconds, checking mouseActive periodically
    unsigned long startWait = millis();
    while (millis() - startWait < 20000) {
      if (!mouseActive) return;  // Early exit if deactivated
      delay(100);  // Check every 100ms during wait
    }
  }

  // Small delay in main loop
  delay(10);
}