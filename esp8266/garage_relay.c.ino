#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 160);
IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

const char* ssid = "<your wifi ssid>";
const char* password = "<your wifi password>";

const char* secret_token = "<some symmetric key to offer basic security>";

ESP8266WebServer server(80);

uint8_t relayPin = D6;
bool triggerStatus = false;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
  WiFi.begin(ssid, password);
  delay(100);

  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  server.on("/trigger", handle_trigger);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  if (triggerStatus)
  {
    triggerStatus = false;
    digitalWrite(relayPin, LOW);
    delay(1000);
    digitalWrite(relayPin, HIGH);
  }
}

void handle_trigger() {
  Serial.println("Triggered!");
  if (server.arg("token") != secret_token)
  {
    Serial.println("Unauthenticated!");
    server.send(404, "text/plain", "Not found"); //Don't reveal anything if not authenticated
    return;
  }

  triggerStatus = true;
  server.send(200, "application/json", "{}");
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
