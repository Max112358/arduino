const int debounceDelay = 500; // Debounce time in milliseconds
unsigned long firstDebounceTime = 0; // Last time the output was toggled
unsigned long recentDebounceTime = 0; // Last time the output was toggled
bool debounceCheck = false;

const int stepPin = 2;
const int dirPin = 5;
const int enablePin = 8; // Common ENABLE pin
const int buttonPin = 11; // Button connected to analog pin A0
const int ledPin = LED_BUILTIN; // Onboard LED pin
const int takePhotoPin = 12; // Button connected to analog pin A0

const int stepsToTake = 200; // Number of steps to rotate
const int onDelay = 50; // Delay between steps (in milliseconds)
const int offDelay = 1250; // Delay between steps (in milliseconds)

bool runMode = false; // Flag to track if the motor is in run mode
int steps = 0; // Counter for the number of steps taken

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT); // Set the ENABLE pin as output
  pinMode(ledPin, OUTPUT); // Set the LED pin as output
  pinMode(takePhotoPin, OUTPUT); // Set the LED pin as output
  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as input with internal pull-up

  digitalWrite(enablePin, LOW); // Enable the motor driver
  digitalWrite(dirPin, HIGH);   // Set the direction
}

void loop() {
  // Read the state of the button
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && !debounceCheck) {
    firstDebounceTime = millis();
    debounceCheck = true;
  }

  if (buttonState == LOW && debounceCheck) {
    recentDebounceTime = millis();
    if(recentDebounceTime - firstDebounceTime >  debounceDelay){
      runMode = true; // Set the run mode flag
      steps = 0; // Reset the step counter
      debounceCheck = false;
    }
  }

  if (buttonState == HIGH && debounceCheck) {
    debounceCheck = false;
  }



  // If the motor is in run mode
  if (runMode) {
    // Rotate the motor for one step
    digitalWrite(ledPin, HIGH);
    digitalWrite(stepPin, HIGH);
    digitalWrite(takePhotoPin, HIGH);
    delay(onDelay);
    digitalWrite(stepPin, LOW);
    digitalWrite(takePhotoPin, LOW);
    delay(offDelay);

    steps++; // Increment the step counter

    // If the specified number of steps is reached, exit run mode
    if (steps >= stepsToTake) {
      runMode = false;
    }
  }
  else{
    digitalWrite(ledPin, LOW);
  }

}