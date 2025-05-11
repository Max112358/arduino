#include <Mouse.h>

const int buttonPin = 3;    // Using D3 for the button
boolean mouseActive = true; // whether the sequence is active
boolean lastButtonState = HIGH; // previous state of the button
boolean currentButtonState = HIGH; // current state of the button

// Positions for the two click locations (relative to corner)
const int pos1X = -180;  // X offset from corner for first position
const int pos1Y = -265;  // Y offset from corner for first position
const int pos2X = -100;  // X offset from corner for second position
const int pos2Y = -50;  // Y offset from corner for second position

void setup() {
  // Initialize the button pin as input with internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Initialize the mouse library
  Mouse.begin();
  
  // Small delay at startup
  delay(1000);
}

// Generic function to move mouse by a specific distance in steps
void moveMouseBy(int deltaX, int deltaY) {
  // Calculate how many steps are needed
  const int stepSize = 25;  // Maximum pixels per step (smaller for more precision)
  int stepsX = abs(deltaX) / stepSize + 1;
  int stepsY = abs(deltaY) / stepSize + 1;
  int steps = max(stepsX, stepsY);
  
  // Calculate movement per step
  float stepX = (float)deltaX / steps;
  float stepY = (float)deltaY / steps;
  
  // Move in small increments
  for (int i = 0; i < steps; i++) {
    Mouse.move(round(stepX), round(stepY), 0);
    delay(5);  // Small delay between moves
  }
  delay(100);  // Allow time for the cursor to settle
}

// Function to move cursor to bottom-right corner
void moveToCorner() {
  // Make multiple large moves to ensure we hit the corner
  for (int i = 0; i < 10; i++) {
    moveMouseBy(127, 127);
  }
}

void loop() {
  // Read the state of the button (LOW when pressed because of pull-up)
  currentButtonState = digitalRead(buttonPin);
  
  // Check if button was just pressed (transition from HIGH to LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    // Toggle mouse active state
    mouseActive = !mouseActive;
    
    // Add a small delay to debounce
    delay(50);
  }
  
  // Update last button state
  lastButtonState = currentButtonState;
  
  // If sequence is active, perform the clicks
  if (mouseActive) {
    // First, move to bottom right corner
    moveToCorner();
    
    // Move to position 1 (relative to corner)
    moveMouseBy(pos1X, pos1Y);
    
    // Click position 1 twenty-five times
    for (int i = 0; i < 25; i++) {
      Mouse.click(MOUSE_LEFT);
      delay(50);  // Short delay between clicks
    }
    
    // Move to position 2 (relative to position 1)
    moveMouseBy(pos2X, pos2Y);
    
    // Click position 2 once
    Mouse.click(MOUSE_LEFT);
    
    // Wait for 30 seconds
    delay(30000);
  }
  
  // Small delay in the main loop
  delay(10);
}