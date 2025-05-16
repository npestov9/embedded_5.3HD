#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// WiFi credentials (left blank for submission)
const char* ssid = "";
const char* password = "";

// Firebase host (without https:// and no trailing slash)
const char* FIREBASE_HOST = "";

// HTTP client setup
WiFiClient wifi;
HttpClient client = HttpClient(wifi, FIREBASE_HOST, 443); // Firebase uses HTTPS

// LED pins
const int RED_LED = 2;
const int GREEN_LED = 5;
const int BLUE_LED = 8;

void setup() {
  Serial.begin(9600);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println("Setup complete.");
}

void loop() {
  // Firebase REST path to value (no auth param because rules are public)
  String path = "/ledControl/color.json";

  // 🔹 This is the actual HTTP GET request to Firebase
  client.get(path);

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  // Clean and use the response
  response.trim();
  response.replace("\"", "");  // remove quotes from JSON string

  if (response == "red") {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    Serial.println("Red LED ON");
  } else if (response == "green") {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
    Serial.println("Green LED ON");
  } else if (response == "blue") {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
    Serial.println("Blue LED ON");
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    Serial.println("Invalid color. LEDs OFF");
  }

  delay(1000);
}
