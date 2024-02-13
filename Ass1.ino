#include <MIDI.h>

// Create a MIDI instance with default settings
MIDI_CREATE_DEFAULT_INSTANCE();

// Pin assignments
const int trigPin = 10; // Trigger pin for the ultrasonic sensor
const int echoPin = 9;  // Echo pin for the ultrasonic sensor
const int potPin = A0;  // Pin for the potentiometer
const int buttonPin = 7; // First button pin
int buttonPin2 = 3; // Second button pin
int buttonPin3 = 4; // Third button pin
byte velocity; // MIDI velocity value
byte noteON = 0x90; // MIDI note on command

void setup() {
  // Set pin modes
  pinMode(trigPin, OUTPUT); // Set the ultrasonic sensor's trigger pin as output
  pinMode(echoPin, INPUT); // Set the ultrasonic sensor's echo pin as input
  pinMode(potPin, INPUT); // Set the potentiometer pin as input
  pinMode(buttonPin, INPUT_PULLUP); // Configure the first button pin as input with internal pull-up
  pinMode(buttonPin2, INPUT_PULLUP); // Configure the second button pin as input with internal pull-up
  pinMode(buttonPin3, INPUT_PULLUP); // Configure the third button pin as input with internal pull-up
  Serial.begin(115200); // Initialize serial communication
}

void loop() {
  long duration, distanceCm;
  
  // Measure distance only when the button is pressed
  if (digitalRead(buttonPin) == LOW) { // Check if the first button is pressed
    // Trigger ultrasonic sensor to measure distance
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Calculate the distance
    duration = pulseIn(echoPin, HIGH);
    distanceCm = duration * 0.034 / 2; // Convert time to distance

    // Map the distance to MIDI note values
    byte note = map(distanceCm, 10, 100, 0, 127); // Adjust distance range to MIDI note range
    note = constrain(note, 0, 127); // Ensure note value stays within MIDI range

    // Map potentiometer value to MIDI velocity
    int PotVal = analogRead(potPin);
    velocity = map(PotVal, 0, 1023, 0, 127); // Map potentiometer value to dynamic range

    // Send MIDI message
    MIDImessage(noteON, note, velocity);
  }

  // Additional logic for other buttons with specific notes for simplicity in this example
  // Each block sends a MIDI message when a specific button is pressed
  // Demonstrates how to send different MIDI notes based on button press combinations
  if (digitalRead(buttonPin2) == LOW) {
    byte note = 60; // Assume sending a C note
    // Reuse potentiometer value for velocity
    MIDImessage(noteON, note, velocity);
    delay(100); // Debounce delay
  }
  
  if (digitalRead(buttonPin3) == LOW) {
    byte note = 67; // Assume sending a G note
    // Reuse potentiometer value for velocity
    MIDImessage(noteON, note, velocity);
    delay(100); // Debounce delay
  }

  // Examples of logic for button combinations sending specific notes
  if (digitalRead(buttonPin2) == LOW && digitalRead(buttonPin) == LOW) {
    byte note = 15; // Example note value
    MIDImessage(noteON, note, velocity);
    delay(100); // Debounce delay
  }

  if (digitalRead(buttonPin3) == LOW && digitalRead(buttonPin) == LOW) {
    byte note = 27; // Example note value
    MIDImessage(noteON, note, velocity);
    delay(100); // Debounce delay
  }

  if (digitalRead(buttonPin3) == LOW && digitalRead(buttonPin2) == LOW) {
    byte note = 60; // C note for both buttons pressed
    MIDImessage(noteON, note, velocity);
    delay(100); // Debounce delay
  }

  delay(100); // Simple debounce delay for the loop
}

// Function to send MIDI messages
void MIDImessage(byte command, byte data1, byte data2) {
  Serial.write(command); // Send the command byte (e.g., note on)
  Serial.write(data1); // Send the first data byte (e.g., note number)
  Serial.write(data2); // Send the second data byte (e.g., velocity)
}
