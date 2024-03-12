
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h> 
#include <LiquidCrystal_I2C.h>
Servo servo; 
Servo servoFan;
#define ONE_WIRE_BUS 4

#define FULLSTEP 4
#define LED_PIN     2
#define POWER_PIN   7
#define SIGNAL_PIN  A5
#define THRESHOLD   300
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define LDR A0 // Photoresistor at Arduino analog pin A0
#define LDRSingal 9      // Led pin at Arduino pin 9
int value = 0; // variable to store the sensor value
int motorPin = 3;
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup(void)
{

  // Start serial communication for debugging purposes
  Serial.begin(9600);
  // Start up the library
  pinMode(POWER_PIN, OUTPUT);   // configure D7 pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW); // turn the sensor OFF
  pinMode(LDR, INPUT);// Set LDR - A0 pin as an input (optional)
  pinMode(motorPin, OUTPUT);
  servo.attach(9);
  servoFan.attach(10);
  Serial.begin(9600);
 
}


void loop(void){ 
   
   
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float  curTem=sensors.getTempCByIndex(0);

  Serial.print("Celsius temperature: ");
  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.print(" - Fahrenheit temperature: ");
  Serial.println(sensors.getTempFByIndex(0));
 
  if(curTem<=5){
     servo.write(0);
  }
  else if (curTem>5 && curTem<=10 ){
    servo.write(60);
  }
  else if (curTem>=10  && curTem<=15 ){
    servo.write(120);
  }else{
     servo.write(180);
  }
   

  //Create the water leverl sensor
  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF

  Serial.print("Sensor value: ");
  Serial.println(value);

  // create the sun shine detector
  int LDRvalue = analogRead(LDR);
  //You can change value "25"  
  Serial.println("Sensor value12: ");
  Serial.println(LDRvalue);
  if(value >= 10){
        servoFan.write(0);

  }
  else if(LDRvalue>=700){
          servoFan.write(180);                           // waits 10ms for the servo to reach the position
  
  }else{
      servoFan.write(90); 
  }









  delay(1000); //Small delay


}
