#include <WiFi.h>

#include <WiFiClientSecure.h> // Include WiFiClientSecure for secure communication

#include <PubSubClient.h>

#include <DHT.h> // Include the DHT sensor library



// Define pins for sensors

#define PH_SENSOR_PIN 34       // pH Sensor connected to pin 34 (Analog pin)

#define SOUND_SENSOR_PIN 35    // Sound Sensor connected to pin 35 (Analog pin)

#define DHT_PIN 4              // DHT sensor connected to digital pin 4

#define DHT_TYPE DHT11         // Define the type of DHT sensor (DHT11)



// Initialize the DHT sensor

DHT dht(DHT_PIN, DHT_TYPE);



// WiFi credentials

#define SECRET_SSID "Narzo"

#define SECRET_PASS "0101010101"



// MQTT broker credentials

const char* mqttServer = "faf8ea67d34245ff9ab5e0e4241c3d58.s1.eu.hivemq.cloud";

const int mqttPort = 8883; // Secure port for MQTT

const char* mqttUser = "farhaan114";

const char* mqttPassword = "sudopw";

const char* mqttTopic = "/stirs";  // Topic for sensor data



WiFiClientSecure wifiClient; // Use WiFiClientSecure for secure communication

PubSubClient client(wifiClient);



void connectToMqtt() {

  while (!client.connected()) {

    Serial.println("Connecting to MQTT...");

    if (client.connect("SensorClient", mqttUser, mqttPassword)) {

      Serial.println("Connected to MQTT Broker");

    } else {

      Serial.print("MQTT connection failed, rc=");

      Serial.print(client.state());

      delay(2000);

    }

  }

}



void setup() {

  Serial.begin(115200); // Start the Serial Monitor

  dht.begin();          // Initialize the DHT sensor



  // Connect to WiFi

  WiFi.begin(SECRET_SSID, SECRET_PASS);

  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);

    Serial.println("Connecting to WiFi...");

  }

  Serial.println("Connected to WiFi");



  // Set the CA certificate

  wifiClient.setCACert(

"-----BEGIN CERTIFICATE-----\n"

"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\n" 

"-----END CERTIFICATE-----\n");



  // Connect to MQTT broker

  client.setServer(mqttServer, mqttPort);



  // Wait until MQTT is connected

  connectToMqtt();

}



void loop() {

  static unsigned long lastReadTime = 0;

  unsigned long currentMillis = millis();



  if (!client.connected()) {

    connectToMqtt();

  }

  client.loop();



  // Read and publish data every 5 seconds

  if (currentMillis - lastReadTime >= 2000) {

    lastReadTime = currentMillis;



    int phRawValue = analogRead(PH_SENSOR_PIN); 

    float voltage = phRawValue * (3.3 / 4095.0);

    float phValue = 7 + ((2.5 - voltage) / 0.18);



    int soundRawValue = analogRead(SOUND_SENSOR_PIN);

    float soundLevel = soundRawValue * (3.3 / 4095.0);



    float temperature = dht.readTemperature();

    if (isnan(temperature)) {

      Serial.println("Failed to read from DHT sensor!");

      temperature = 0; // Default or error handling

    }



    String payload = "pH: " + String(phValue) + ", Sound: " + String(soundLevel) + ", Temperature: " + String(temperature);

    Serial.println("Publishing: " + payload);



    if (!client.publish(mqttTopic, payload.c_str())) {

      Serial.println("Failed to publish data");

    }

  }

}
