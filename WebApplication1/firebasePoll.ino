#include <WiFiNINA.h>
#include <Firebase_Arduino_WiFiNINA.h>  // correct library for Nano 33 IoT

// Firebase credentials
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

// WiFi credentials
const char* ssid = "";
const char* password = "";

// LED pins
const int RED_LED = 2;
const int GREEN_LED = 5;
const int BLUE_LED = 8;

FirebaseData fbdo; // for handling Firebase requests

void setup() {
  Serial.begin(9600);

  // Connect to WiFi (for logs)
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, ssid, password);
  Firebase.reconnectWiFi(true);

  // LED pin setup
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  Serial.println("Setup complete, waiting for commands...");
}


void loop() {
  String ledColor = "";

  // Get the color string from Firebase
  if (Firebase.getString(fbdo, "/ledControl/color")) {
    ledColor = fbdo.stringData();
    Serial.print("Color received: ");
    Serial.println(ledColor);
  } else {
    Serial.print("Firebase get failed: ");
    Serial.println(fbdo.errorReason());
  }

  // Control LEDs based on Firebase value
  if (ledColor == "red") {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    Serial.println("Red LED ON");
  } else if (ledColor == "green") {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
    Serial.println("Green LED ON");
  } else if (ledColor == "blue") {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
    Serial.println("Blue LED ON");
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    Serial.println("No valid color, all LEDs OFF");
  }

  delay(1000);
}
