#include <LiquidCrystal_I2C.h>

int redOut = 13;
int greenOut = 12;
int blueOut = 11;
int button = 8;
bool flag = false;
char timeline[16];

unsigned long previousMillis = 0;
const long interval = 1000; 
signed short minutes = 0, seconds = 5;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init(); // initialize the lcd
  lcd.backlight();
  pinMode(redOut, OUTPUT);
  pinMode(greenOut, OUTPUT);
  pinMode(blueOut, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  setRGB(0, 255, 0); 
  flag = true; 

  updateLCD(); //initial LCD light
}

void setRGB(int red, int green, int blue) {
  analogWrite(redOut, red);
  analogWrite(greenOut, green);
  analogWrite(blueOut, blue);
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("4 times a day");
  lcd.setCursor(0, 1);
  sprintf(timeline, "%02d mins %02d secs", minutes, seconds);
  lcd.print(timeline);
}

void loop() {
  unsigned long currentMillis = millis();

  // Timer
  if (flag && currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (seconds > 0) {
      seconds--;
    } else if (minutes > 0) {
      minutes--;
      seconds = 59;
    }

    updateLCD();// update current time 

    if (minutes == 0 && seconds == 0) {
      flag = false;
      setRGB(255, 0, 0);// LED changes red
    }
  }

  // 
  if (digitalRead(button) == HIGH) {
    minutes = 0;
    seconds = 15;
    flag = true;
    setRGB(0, 255, 0); // Reset color to green
    updateLCD(); // Update fixed tiem
    delay(200); 
  }
}
