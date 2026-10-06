#include <WiFiNINA.h>
#include <DHT.h>

// Your Wi-Fi credentials
char ssid[] = "GIMM-Lab";
char pass[] = "34cP0WwMjdKB71";


// --------------------
// AWS server
// --------------------
char server[] = "54.234.130.4";
int port = 3000;

WiFiClient client;

// --------------------
// DHT11 sensor
// --------------------
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);


void setup() {

  Serial.begin(9600);

  while (!Serial);

  // Start sensor
  dht.begin();

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");

  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    delay(5000);
  }

  Serial.println("Connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}


void loop() {

  // --------------------
  // Read DHT11
  // --------------------

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Make sure sensor reading worked
  if (isnan(humidity) || isnan(temperature)) {

    Serial.println("Failed to read DHT11!");

    delay(5000);
    return;
  }


  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C | Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");


  // --------------------
  // Build JSON
  // --------------------

  String jsonData =
    "{\"temperature\":" + String(temperature, 1) +
    ",\"humidity\":" + String(humidity, 1) + "}";


  Serial.print("Sending JSON: ");
  Serial.println(jsonData);


  // --------------------
  // Connect to AWS
  // --------------------

  Serial.println("Connecting to server...");

  if (client.connect(server, port)) {

    Serial.println("Connected to server.");


    // HTTP POST request
    client.println("POST /api/sensor HTTP/1.1");

    client.print("Host: ");
    client.println(server);

    client.println("Content-Type: application/json");

    client.print("Content-Length: ");
    client.println(jsonData.length());

    client.println("Connection: close");

    client.println();

    // Send JSON body
    client.println(jsonData);


    // --------------------
    // Read server response
    // --------------------

    unsigned long startTime = millis();

    while (client.connected() && millis() - startTime < 5000) {

      while (client.available()) {

        char c = client.read();
        Serial.write(c);

        // Reset timeout whenever data arrives
        startTime = millis();
      }
    }

    client.stop();

    Serial.println();
  }

  else {

    Serial.println("Connection failed.");
  }


  // Wait before collecting another reading
  delay(5000);
}
