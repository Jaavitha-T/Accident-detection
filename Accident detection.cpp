#include <Wire.h> 
#include <MPU6050.h> 
 
#define trigFront 2 
#define echoFront 3 
#define trigBack 4 
#define echoBack 5 
#define vibrationPin 6 
#define relayPin 7 
#define buzzerPin 8 
 
MPU6050 mpu; 
 
void setup() { 
  Serial.begin(9600); 
  Wire.begin(); 
  mpu.initialize(); 
 
  pinMode(trigFront, OUTPUT); 
  pinMode(echoFront, INPUT); 
  pinMode(trigBack, OUTPUT); 
  pinMode(echoBack, INPUT); 
 
  pinMode(vibrationPin, INPUT); 
  pinMode(relayPin, OUTPUT); 
  pinMode(buzzerPin, OUTPUT); 
 
  digitalWrite(relayPin, LOW); 
  digitalWrite(buzzerPin, LOW); 
 
  Serial.println("System Initialized..."); 
} 
 
long readUltrasonicDistance(int trigPin, int echoPin) { 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  long duration = pulseIn(echoPin, HIGH); 
  return duration * 0.034 / 2; // cm 
} 
 
void loop() { 
  // Distance readings 
  long frontDist = readUltrasonicDistance(trigFront, echoFront); 

 
  long backDist = readUltrasonicDistance(trigBack, echoBack); 
 
  // MPU6050 
  int16_t ax, ay, az; 
  mpu.getAcceleration(&ax, &ay, &az); 
  float axG = ax / 16384.0; 
  float ayG = ay / 16384.0; 
  float azG = az / 16384.0; 
 
  // Vibration sensor 
  bool vibrationDetected = digitalRead(vibrationPin) == HIGH; 
 
  // Serial Output 
  Serial.print("Front: "); 
  Serial.print(frontDist); 
  Serial.print(" cm | Back: "); 
  Serial.print(backDist); 
  Serial.print(" cm | MPU (ax, ay, az): "); 
  Serial.print(axG, 2); Serial.print("1, "); 
  Serial.print(ayG, 2); Serial.print("4, "); 
  Serial.print(azG, 2); Serial.print(" 5| Vibration: "); 
  Serial.println(vibrationDetected ? "YES" : "NO"); 
 
  // Danger thresholds 
  bool nearFront = frontDist < 5; 
  bool nearBack = backDist <2; 
  if(nearFront || nearBack||vibrationDetected){ 
    digitalWrite(buzzerPin,HIGH); 
  }else{ 
    digitalWrite(buzzerPin,LOW); 
  } 
  bool highAccel = (abs(axG) > 2.0 || abs(ayG) > 2.0);  // Sudden tilt 
  bool allTriggered = nearFront && nearBack && vibrationDetected; 
  delay(500); 
} 
 

}
