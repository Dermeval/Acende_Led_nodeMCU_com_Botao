/* Código criado com auxilio da IDE do ArduinoOTA */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "INTELBRAS"
#define STAPSK  "Pbl-Sistemas-Digitais"  /* Colocar o nome da Rede */
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "ESP-10.0.0.107"; /* Endereço da placa */

int led_pin = LED_BUILTIN; /* Instanciando o pino */
#define N_DIMMERS 3
int dimmer_pin[] = {14, 5, 15};

void setup() {
  //Serial.begin(115200);

  /* switch on led */
  pinMode(led_pin, OUTPUT);
  pinMode(D0, INPUT);

  Serial.begin(9600);

/* Uma comunicação assíncrona (que não usam o clock como referência) com 9600 bps envia um bit em 0,0001 s. 
Esta taxa assume diferentes nomes dependendo da comunicação, 
como em comunicações síncronas que é chamada de "clock" ou 
em comunicações assíncronas que é conhecida como "Baud Rate". */


  Serial.println("Booting");
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("Retrying connection...");
  }
  /* switch off led */
  digitalWrite(led_pin, HIGH);

  /* configure dimmers, and OTA server events */
  analogWriteRange(1000);
  analogWrite(led_pin, 990);

  for (int i = 0; i < N_DIMMERS; i++) {
    pinMode(dimmer_pin[i], OUTPUT);
    analogWrite(dimmer_pin[i], 50);
  }

  ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
    for (int i = 0; i < N_DIMMERS; i++) {
      analogWrite(dimmer_pin[i], 0);
    }
    analogWrite(led_pin, 0);
  });

  ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
    for (int i = 0; i < 30; i++) {
      analogWrite(led_pin, (i * 100) % 1001);
      delay(50);
    }
  });

  ArduinoOTA.onError([](ota_error_t error) {
    (void)error;
    ESP.restart();
  });

  /* setup the OTA server */
  ArduinoOTA.begin();
  Serial.println("Ready");

}

void loop() {
  ArduinoOTA.handle();
  if(digitalRead(D0) == HIGH){
      digitalWrite(led_pin, HIGH);
   } else {
    digitalWrite(led_pin, LOW);
   }

}
