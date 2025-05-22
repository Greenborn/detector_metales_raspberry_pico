#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <LEAmDNS.h>
#include <hardware/adc.h>

#ifndef STASSID
#define STASSID "Fibertel WiFi945 2.4GHz"
#define STAPSK "Chevrolet1973"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
int duty = 65535/10;
int cant_ciclos = 200;
int multiplicador = 1;
int frecuencia = 150;

WebServer server(80);

const int led = LED_BUILTIN;
const int ledPin = 22;

int cnt = 0;
int arr_1[300];
int arr_2[300];

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from pico w!\r\n");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup() {
  analogReadResolution(16);
  Serial.begin(2000000);

  pinMode(ledPin, OUTPUT);
  analogWriteFreq(frecuencia);
  analogWriteRange(65535);
  analogWriteResolution(16);
  analogWrite(ledPin, duty);
/*
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("picow")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/gif", []() {
    static const uint8_t gif[] = {
      0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
      0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
      0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
      0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
    };
    char gif_colored[sizeof(gif)];
    memcpy_P(gif_colored, gif, sizeof(gif));
    // Set the background to a random set of colors
    gif_colored[16] = millis() % 256;
    gif_colored[17] = millis() % 256;
    gif_colored[18] = millis() % 256;
    server.send(200, "image/gif", gif_colored, sizeof(gif_colored));
  });

  server.onNotFound(handleNotFound);

  /////////////////////////////////////////////////////////
  // Hook examples

  server.addHook([](const String & method, const String & url, WiFiClient * client, WebServer::ContentTypeFunction contentType) {
    (void)method;       // GET, PUT, ...
    (void)url;          // example: /root/myfile.html
    (void)client;       // the webserver tcp client connection
    (void)contentType;  // contentType(".html") => "text/html"
    Serial.printf("A useless web hook has passed\n");
    return WebServer::CLIENT_REQUEST_CAN_CONTINUE;
  });

  server.addHook([](const String&, const String & url, WiFiClient*, WebServer::ContentTypeFunction) {
    if (url.startsWith("/fail")) {
      Serial.printf("An always failing web hook has been triggered\n");
      return WebServer::CLIENT_MUST_STOP;
    }
    return WebServer::CLIENT_REQUEST_CAN_CONTINUE;
  });

  server.addHook([](const String&, const String & url, WiFiClient * client, WebServer::ContentTypeFunction) {
    if (url.startsWith("/dump")) {
      Serial.printf("The dumper web hook is on the run\n");

      // Here the request is not interpreted, so we cannot for sure
      // swallow the exact amount matching the full request+content,
      // hence the tcp connection cannot be handled anymore by the
      auto last = millis();
      while ((millis() - last) < 500) {
        char buf[32];
        size_t len = client->read((uint8_t*)buf, sizeof(buf));
        if (len > 0) {
          Serial.printf("(<%d> chars)", (int)len);
          Serial.write(buf, len);
          last = millis();
        }
      }
      // Two choices: return MUST STOP and webserver will close it
      //                       (we already have the example with '/fail' hook)
      // or                  IS GIVEN and webserver will forget it
      // trying with IS GIVEN and storing it on a dumb WiFiClient.
      // check the client connection: it should not immediately be closed
      // (make another '/dump' one to close the first)
      Serial.printf("\nTelling server to forget this connection\n");
      static WiFiClient forgetme = *client;  // stop previous one if present and transfer client refcounter
      return WebServer::CLIENT_IS_GIVEN;
    }
    return WebServer::CLIENT_REQUEST_CAN_CONTINUE;
  });

  // Hook examples
  /////////////////////////////////////////////////////////

  server.begin();
  Serial.println("HTTP server started");
*/
  int j= 0;
  while (1) {
    /*if (Serial.available() > 0) {
      String comando = Serial.readStringUntil(':');
      if (comando == "frec") {
        int frecuencia = Serial.parseInt();
        analogWriteFreq(frecuencia);
        analogWrite(ledPin, duty);
      }
      if (comando == "duty") {
        duty = Serial.parseInt();
        analogWrite(ledPin, duty);
      }
    }*/
  int sum = 0;
  int max_ = 0;
  int min_ = 1200;
    //if (analogRead(A1)< 300){
      for (cnt=0; cnt < cant_ciclos; cnt ++){
        arr_1[cnt] = analogRead(A0);
        arr_2[cnt] = analogRead(A1)*multiplicador;
        if (arr_1[cnt] > 50){
          //arr_2[cnt] = 0;
        }
        sum += arr_2[cnt];
        if (arr_2[cnt] > max_){ max_ = arr_2[cnt]; }
        if (arr_2[cnt] < min_){ min_ = arr_2[cnt]; }
      }  
    //}    

    
    
    for (cnt=0; cnt < cant_ciclos; cnt ++){
      for (j=0; j < 1; j++){
      Serial.print(arr_1[cnt]);
      Serial.print(",");
      Serial.print(arr_2[cnt]);
      Serial.print(",");
      Serial.print(sum/cant_ciclos);
      Serial.print(",");
      Serial.print(min_);
      Serial.print(",");
      Serial.print(max_);
      Serial.print("\n");
      }
    }
    delay(250);
    //server.handleClient();
    //MDNS.update();
  }
}

void loop() {
  /* Nothing here */
}
