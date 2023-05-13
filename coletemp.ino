#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHT_SENSOR_PIN  21 
#define DHT_SENSOR_TYPE DHT22

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

const char WIFI_SSID[] = "Rede";
const char WIFI_PASSWORD[] = "Senha";

String HOST_NAME = "http://servidor"; 
String PATH_NAME   = "/caminho";

void setup() {
  Serial.begin(115200); 
  
  dht_sensor.begin(); 

  //Conectar Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void enviarColeta(){
  //Coletar temeratura e umidade
  float umid_ar = dht_sensor.readHumidity();
  float temp_ar = dht_sensor.readTemperature();

  HTTPClient http;

  http.begin(HOST_NAME + PATH_NAME + "?umid_ar=" + umid_ar + "&temp_ar=" + temp_ar + "&local=comAr"); //HTTP
  int httpCode = http.GET();

  // httpCode will be negative on error
  if(httpCode > 0) {
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void loop() {
  enviarColeta();
  delay(300000);
}
