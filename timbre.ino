#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);
}

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  server.on("/", handleRoot);
  server.begin();
}

void handleRoot() {
  server.send(200, "text/plain", "Hello from ESP8266!");
}

void loop() {
  server.handleClient();
}
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  server.on("/", handleRoot);
  server.begin();
}

void handleRoot() {
  server.send(200, "text/plain", "Hello from ESP8266!");
}

void loop() {
  server.handleClient();
}
const int relayPin = 8;

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // Asegúrate de que el relé está apagado al inicio
}

void loop() {
  DateTime now = rtc.now();

  // Aquí deberías comprobar los horarios almacenados en la EEPROM
  // y activar el relé cuando coincidan con la hora actual

  if (shouldRing(now)) {
    digitalWrite(relayPin, HIGH);  // Enciende el relé (activa el timbre)
  } else {
    digitalWrite(relayPin, LOW);  // Apaga el relé (desactiva el timbre)
  }

  delay(1000);  // Espera 1 segundo antes de volver a comprobar
}

bool shouldRing(DateTime now) {
  // Implementa la lógica para comprobar si el timbre debe sonar
  // comparando la hora actual con los horarios almacenados en la EEPROM
  return false;
}
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Wire.h>
#include <RTClib.h>

#define FIREBASE_HOST "YOUR_PROJECT_ID.firebaseio.com"
#define FIREBASE_AUTH "YOUR_FIREBASE_DATABASE_SECRET"
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"

RTC_DS3231 rtc;
const int relayPin = 8;
FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado a WiFi");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Wire.begin();
  rtc.begin();

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
}

void loop() {
  DateTime now = rtc.now();

  if (Firebase.getJSON(firebaseData, "/horarios")) {
    if (firebaseData.dataType() == "json") {
      FirebaseJson& json = firebaseData.jsonObject();
      String time1, time2, time3, time4, time5;
      json.get(time1, "horario1");
      json.get(time2, "horario2");
      json.get(time3, "horario3");
      json.get(time4, "horario4");
      json.get(time5, "horario5");

      if (shouldRing(now, time1) || shouldRing(now, time2) || shouldRing(now, time3) || shouldRing(now, time4) || shouldRing(now, time5)) {
        digitalWrite(relayPin, HIGH);
      } else {
        digitalWrite(relayPin, LOW);
      }
    }
  }

  delay(1000);
}

bool shouldRing(DateTime now, String horario) {
  int hour = horario.substring(0, 2).toInt();
  int minute = horario.substring(3, 5).toInt();
  return (now.hour() == hour && now.minute() == minute);
}
