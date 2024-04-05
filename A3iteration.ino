
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h> 
#include <LiquidCrystal_I2C.h>
Servo servoUpper; 
Servo servoMid;
Servo servoDown;
Servo servoLeft;
Servo servoRight;
#define ONE_WIRE_BUS 4

#define FULLSTEP 4
#define LED_PIN     2
#define SIGNAL_PIN  A1
#define THRESHOLD   300
#define LDR A0 // Photoresistor at Arduino analog pin A0

int value = 0; // variable to store the sensor value
int button = 7;
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
float getCurrentTem;
float  curTem;
               // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 10;    
const int buttonPin = 12;  
boolean change;   
float duration_us, distance_cm;          
LiquidCrystal_I2C lcd(0x27, 16, 2);


int trigPin = 13;    // TRIG pin
int echoPin = 12;    // ECHO pin

void updateLCD() {
  if (change==false){
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waiting for user!!");
    lcd.setCursor(0, 1);
    lcd.print(curTem);    // print the temperature in Celsius
    lcd.print((char)223);      // print ° character
    lcd.print("C");
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("User is using!!");
    lcd.setCursor(0, 1);
    lcd.print(curTem);    // print the temperature in Celsius
    lcd.print((char)223);      // print ° character
    lcd.print("C");
  
  }


}
void setup(void)
{

  // Start serial communication for debugging purposes
  Serial.begin(9600);
    lcd.init(); // initialize the lcd
  lcd.backlight();
  // Start up the library

  pinMode(LDR, INPUT);// Set LDR - A0 pin as an input (optional)
  pinMode(button, INPUT);
  servoUpper.attach(9);
  servoMid.attach(10);
  servoDown.attach(11);
 
  updateLCD();
  pinMode(inputPin, INPUT);     // declare sensor as input
     // configure the trigger pin to output mode
  pinMode(trigPin, OUTPUT);
  // configure the echo pin to input mode
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
    pinMode(buttonPin, INPUT);

  
}


void loop(void){ 


  // // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  curTem=sensors.getTempCByIndex(0);

  // Serial.print("Celsius temperature: ");
  // // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  // Serial.print(sensors.getTempCByIndex(0)); 
  delay(10);

// generate 10-microsecond pulse to TRIG pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");


  if(distance_cm <= 15){
    change = true; // Update change to true when distance is less than 15 cm
  } else {
    change = false; // Otherwise, set change to false
  }
  
  updateLCD(); // Update the LCD based on the current state of change

  
  
  
  //-30 degree
  // Serial.print(curTem);
  if(curTem<=0){ 
     servoUpper.write(180);
     servoMid.write(0);
    servoDown.write(135);
  }
  //-30  to -15
  else if (curTem>0 && curTem<=5 ){
    servoDown.write(180);
      servoUpper.write(135);
      servoMid.write(45);
      // servoDown.write(90);
  }
// -15 to 0
  else if (curTem>5  && curTem<=10 ){
    servoDown.write(180);
     servoUpper.write(95);
     servoMid.write(45);
     //0 to 15
  }else if(curTem>10&&curTem<=15){
    servoDown.write(180);
       servoUpper.write(45);
        servoMid.write(90);
        servoDown.write(180);
        // above 15
  }else if(curTem>15){
    servoDown.write(180);
     servoUpper.write(0);
        servoMid.write(135);
        
  }
   
  
  //Create the water leverl sensor
  delay(10);                      // wait 10 milliseconds
  value = analogRead(SIGNAL_PIN); // read the analog value from sensor
 
  
  Serial.print("\n");
  Serial.print("water Sensor value: ");
  Serial.println(value);

  // create the sun shine detector
  int LDRvalue = analogRead(LDR);
  //You can change value "25"  
  Serial.println("lighe Sensor value: ");               
  Serial.println(LDRvalue);
  bool changeFalge = false;
  if(value >= 100 && LDRvalue <=300){
      servoDown.write(45);
      
  }
  else if(LDRvalue>=300&&value <= 100){
            // servoMid.write(135);                         // waits 10ms for the servo to reach the position
    servoDown.write(0);
  }else if(value >= 100 &&LDRvalue>=300 ){
        servoDown.write(90);
  }else {
    //  
  }









  delay(1000); //Small delay


}
