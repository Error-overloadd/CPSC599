#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

// Pin definitions
const int trigPin = 10; // Ultrasonic sensor trigger pin
const int echoPin = 9;  // Ultrasonic sensor echo pin
const int potPin = A0;  // Potentiometer pin
const int buttonPin = 7; // Button pin 1
const int buttonPin2 = 3; // Button pin 2
const int buttonPin3 = 4; // Button pin 3
byte velocity; // MIDI velocity value
byte noteON = 0x90; // MIDI note on command

void setup() {
  // Initialize pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(potPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  Serial.begin(115200); // Start serial communication
}

void loop() {
  // Update potentiometer reading for velocity control
  int PotVal = analogRead(potPin);
  velocity = map(PotVal, 0, 1023, 0, 127);

  // Check and handle button combinations
  checkButtonCombinations();

  // Check if button 1 is pressed before handling ultrasonic sensor
  if (digitalRead(buttonPin) == LOW) {
    // Trigger the ultrasonic sensor to measure distance
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    long distanceCm = duration * 0.034 / 2;

    // Example: Use distance to control MIDI note pitch, within a valid distance range
    if (distanceCm > 0 && distanceCm <= 100) {
      byte note = map(distanceCm, 10, 100, 48, 72); // Map distance to a MIDI note range
      note = constrain(note, 48, 72); // Ensure the note is within a reasonable range
      MIDImessage(noteON, note, velocity); // Send MIDI message based on distance
    }
  }

  delay(100); // Simple loop delay to reduce sensitivity
}

// Function to send MIDI messages
void MIDImessage(byte command, byte data1, byte data2) {
  Serial.write(command);
  Serial.write(data1);
  Serial.write(data2);
}

// Function to check button combinations
void checkButtonCombinations() {
  bool button1 = digitalRead(buttonPin) == LOW;
  bool button2 = digitalRead(buttonPin2) == LOW;
  bool button3 = digitalRead(buttonPin3) == LOW;

  // Flag to indicate if a combination has been processed
  bool combinationProcessed = false;

  // Check for specific button combinations
  if (button2 && button3) {
    MIDImessage(noteON, 65, velocity); // Send F4(65)
    delay(200); // Debounce delay
    combinationProcessed = true;
  } else if (button1 && button2) {
    MIDImessage(noteON, 67, velocity); // send So
    delay(200); 
    combinationProcessed = true;
  } else if (button1 && button3) {
    MIDImessage(noteON, 69, velocity); //A4
    delay(200); 
    combinationProcessed = true;
  }

  // Process individual button actions only if no combination has been processed
  if (!combinationProcessed) {
    if(button2){
       MIDImessage(noteON, 62, velocity); // send D4
      delay(200);
    }
    if(button3){
        MIDImessage(noteON, 64, velocity); // send Mi E4
      delay(200);
    }
    if(button1){
       MIDImessage(noteON, 60, velocity); //C4
    }
  
  }
}


