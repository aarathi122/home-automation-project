#define BLYNK_TEMPLATE_ID "TMPL3vYgLLW-f"
#define BLYNK_TEMPLATE_NAME "homeautomation"
#define BLYNK_AUTH_TOKEN "2pdYwA2TC2ye0jxK0ly5FlGFasdrUS80"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "2pdYwA2TC2ye0jxK0ly5FlGFasdrUS80";  // Enter your Blynk authentication token

char ssid[] = "realme 10 Pro 5G"; // Enter your WiFi name
char pass[] = "r7h9ue8u"; // Enter your WiFi password
#define trigPin 1 // Trig pin of the ultrasonic sensor
#define echoPin 2 // Echo pin of the ultrasonic sensor
#define pirPin 6 // PIR sensor pin
#define ledPin 5// Relay control pin for the first bulb
#define buzzer 4
#define gasPin A0 // Gas sensor analog pin

const int tankHeight = 100; // Height of the tank in centimeters
int distanceToWater = 0;
bool motionDetected = false;

BlynkTimer timer;

void sendWaterLevel() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1; // Convert distance to centimeters

  distanceToWater = tankHeight - distance;
  
  if (distanceToWater < 0) {
    distanceToWater = 0; // To avoid negative values if sensor goes beyond the tank height
  }

  Blynk.virtualWrite(V0, distanceToWater); // Send water level data to Blynk app
}

void checkPIR() {
  if (digitalRead(pirPin) == HIGH) {
    motionDetected = true;
    digitalWrite(buzzer,HIGH);
    Blynk.virtualWrite(V1, "Motion Detected!"); // Update Blynk app with motion status
  } else {
    motionDetected = false;
    digitalWrite(buzzer,LOW);
    Blynk.virtualWrite(V1, "No Motion"); // Update Blynk app with no motion status
  }
}

void checkGas() {
  int gasValue = analogRead(gasPin);
  Blynk.virtualWrite(V2, gasValue); // Send gas sensor value to Blynk app
}
BLYNK_WRITE(V3) { // Button widget on V3
  int ledState = param.asInt(); // Get the value from the app
  digitalWrite(ledPin, ledState ? HIGH : LOW); // Turn on/off the LED based on the app's button state
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(gasPin, INPUT);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L, sendWaterLevel); // Set interval for sending water level data to Blynk
  timer.setInterval(500L, checkPIR); // Set interval for checking PIR sensor (adjust as needed)
  timer.setInterval(1000L, checkGas); // Set interval for checking gas sensor (adjust as needed)
}

void loop() {
  Blynk.run();
  timer.run();
}
