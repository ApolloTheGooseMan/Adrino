#include <WiFiNINA.h>

// Your Wi-Fi credentials
char ssid[] = "GIMM-Lab";
char pass[] = "34cP0WwMjdKB71";


// Server we want to contact
char server[] = "54.234.130.4";


WiFiClient client;


void setup() {
  Serial.begin(9600);


  while (!Serial);


  Serial.println("Connecting to Wi-Fi...");


  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    delay(5000);
  }


  Serial.println("Connected!");


  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  // Data we want to send
  String jsonData = "{\"temperature\":72.5,\"humidity\":41}";


  Serial.println("Connecting to server...");
  int port = 3000;
 
  if (client.connect(server, port)) {


    Serial.println("Connected to server.");


    // HTTP request
    client.println("POST /api/sensor HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Content-Type: application/json");


    client.print("Content-Length: ");
    client.println(jsonData.length());


    client.println("Connection: close");
    client.println();


    // HTTP request body
    client.println(jsonData);


  } else {
    Serial.println("Connection failed.");
  }
}


void loop() {


  // Display the server response
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }


  if (!client.connected()) {
    client.stop();
  }
}

