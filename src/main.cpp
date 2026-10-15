#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h> 
#include "secrets.h"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWiFi connected");

  // --- Firebase Firestore Configuration ---
  config.api_key = API_KEY;
  auth.user.email = "ngkeanchun120005@gmail.com";     // temporary email for testing
  auth.user.password = "Abcd6075!";      // temporary password for testing

  // Initialize Firebase with the config and auth objects.
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase initialized for Firestore");
}

// Test function to simulate weight data and send it to Firestore
void loop() {
  if (Firebase.ready()) {
    float testWeight = random(100, 900) / 100.0;
    
    FirebaseJson content;
    content.set("fields/current_weight/doubleValue", testWeight);
    content.set("fields/status/stringValue", "active");

    // Will update the document at "bins/paper_bin_01" in Firestore with the new weight and status for this test.
    String documentPath = "bins/paper_bin_01";
    String projectId = PROJECT_ID;

    if (Firebase.Firestore.patchDocument(&fbdo, projectId.c_str(), "", 
                                          documentPath.c_str(), 
                                          content.raw(), 
                                          "current_weight,status")) {
      Serial.printf("Data written: %.2f kg\n", testWeight);
    } else {
      Serial.printf("Error: %s\n", fbdo.errorReason().c_str());
    }
  }
  delay(10000);
}
