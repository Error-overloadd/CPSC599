#include <Wire.h>

// 定义加速度计和陀螺仪的全局变量
long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;
long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

// 波浪线手势检测变量
int waveDirectionChangesX = 0;
const int waveThresholdX = 6;
float lastWaveGForceX = 0;
bool lastDirectionPositiveX = false;

// 圆形手势检测变量
int circleDirectionChanges = 0;
const int circleThreshold = 4;
float lastCircleGForceX = 0, lastCircleGForceY = 0;

// 拍手检测变量
const float clapThreshold = 2.0;
float lastGForceZ = 0;
unsigned long lastClapTime = 0;

// 按钮引脚
const int buttonPin = 7;
const int buttonPin2= 8;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  setupMPU();

  // 初始化按钮输入引脚
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  // 初始化加速度值
  lastWaveGForceX = 0;
  lastDirectionPositiveX = false;
  lastCircleGForceX = 0;
  lastCircleGForceY = 0;
  lastGForceZ = 0;
}

void loop() {
  recordAccelRegisters();
  recordGyroRegisters();
   recordAccelRegisters();
  processAccelData();

  // 仅当按钮被按下时检测波浪线手势
  if (digitalRead(buttonPin) == LOW) {
    detectWaveGesture();
  
  }
  if (digitalRead(buttonPin2)==LOW){
      


  // 检测拍手
  detectCircleGesture();
    
    }
      
  detectClapGesture();

  delay(100);
}

void setupMPU(){
  Wire.beginTransmission(0b1101000); // This is the I2C address of the MPU
  Wire.write(0x6B); // Accessing the register 6B - Power Management
  Wire.write(0b00000000); // Setting SLEEP register to 0.
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1B); // Accessing the register 1B - Gyroscope Configuration
  Wire.write(0x00000000); // Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1C); // Accessing the register 1C - Accelerometer Configuration
  Wire.write(0b00000000); // Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000);
  Wire.write(0x3B); // Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read();
  accelY = Wire.read()<<8|Wire.read();
  accelZ = Wire.read()<<8|Wire.read();
  processAccelData();
}

void processAccelData(){
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0;
  gForceZ = accelZ / 16384.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000);
  Wire.write(0x43); // Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read();
  gyroY = Wire.read()<<8|Wire.read();
  gyroZ = Wire.read()<<8|Wire.read();
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0;
  rotZ = gyroZ / 131.0;
}

void printData() {
//  Serial.print("Gyro (deg) X=");
//  Serial.print(rotX);
//  Serial.print(" Y=");
//  Serial.print(rotY);
//  Serial.print(" Z=");
//  Serial.print(rotZ);
//  Serial.print(" Accel (g) X=");
//  Serial.print(gForceX);
//  Serial.print(" Y=");
//  Serial.print(gForceY);
//  Serial.print(" Z=");
//  Serial.println(gForceZ);
}
void detectCircleGesture() {
    // 简化的圆形手势检测逻辑
    float deltaX = gForceX - lastCircleGForceX;
    float deltaY = gForceY - lastCircleGForceY;
    if (abs(deltaX) > 0.1 && abs(deltaY) > 0.1) {
        // 同时在X和Y轴上检测到显著变化
        circleDirectionChanges++;
    }
    if (circleDirectionChanges >= circleThreshold) {
        Serial.println("Circle Detected!");
        circleDirectionChanges = 0; // 重置计数器
    }
    lastCircleGForceX = gForceX;
    lastCircleGForceY = gForceY;
}

void detectClapGesture() {
  // 检测拍手的代码
  float currentGForceZ = gForceZ;
  unsigned long currentTime = millis();
  
  if (abs(currentGForceZ - lastGForceZ) > clapThreshold && (currentTime - lastClapTime) > 200) { // 200ms去抖动
    Serial.println("Clap Detected!");
    lastClapTime = currentTime;
  }
  
  lastGForceZ = currentGForceZ;
}
void detectWaveGesture() {
    bool currentDirectionPositiveX = (gForceX - lastWaveGForceX) > 0;
    if (currentDirectionPositiveX != lastDirectionPositiveX) {
        waveDirectionChangesX++;
        lastDirectionPositiveX = currentDirectionPositiveX;
    }
    if (waveDirectionChangesX >= waveThresholdX) {
        Serial.println("Wave Detected!");
        waveDirectionChangesX = 0; // 重置计数器
    }
    lastWaveGForceX = gForceX; // 更新加速度值
}
