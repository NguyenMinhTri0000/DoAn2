/* 
ESP8266 With DHT11 & i2C 16x2 LCD Display

LCD config
  SDA pin - D2
  SCL pin - D1
  VCC Pin - 5v

Sensor Config
  Sensor Data pin - RX
  Sensor VCC - 3.3 volt pin

  If you use different pins change the config below.
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h> //This library you can add under Manage Library
#include <ESP8266mDNS.h>
#include <Wire.h>  // This library is already built-in
#include <LiquidCrystal_I2C.h> //Add this library in the Manage Library
LiquidCrystal_I2C lcd(0x3F, 16, 2);

#define DHTPIN 3         //define as DHTPIN the Pin 3 used to connect the Sensor
#define DHTTYPE DHT11    //define the sensor (DHT11)
DHT dht(DHTPIN, DHTTYPE);//create an instance of DHT
const char* host = "IP OF ESP/api"; //IP Address Of The ESP. Ex 192.168.0.xxx/api
const char* ssid     = "Lll";
const char* password = "12356789";

ESP8266WebServer server(80);
float humidity, temp_f;  // Values read from sensor
String webString="";   
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor
 
void handle_root() {
  server.send(200, "text/plain", "Welcome, open /temp, /humidity or /api");
  delay(100);
}
 
void setup(void)
{
  lcd.init();   // initializing the LCD
  lcd.backlight();
  Serial.begin(115200);
  dht.begin(); // initialize temperature sensor
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("DHT Weather Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  MDNS.begin(host);
  MDNS.addService("http", "tcp", 80);
  Serial.printf("Open http://%s.local in your browser\n", host);
  server.on("/", handle_root);
  
  server.on("/temp", [](){  
    gettemperature();       // read sensor
    webString="Temperature: "+String((int)temp_f)+" Celsius";
    server.send(200, "text/plain", webString);
  });

  server.on("/humidity", [](){ 
    gettemperature();           // read sensor
    webString="Humidity: "+String((int)humidity)+"%";
    server.send(200, "text/plain", webString); 
  });

 //Rest API for sensor data
  server.on("/api", [](){  
    gettemperature();           // read sensor
    String json="{{{\"Temperature\"=:="+String((int)temp_f)+",\"Humidity\"=:="+String((int)humidity)+"}}}";
    Serial.println(json);
    server.send(200, "application/json", json);
  });
  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void)
{
  server.handleClient();
  gettemperature();
  lcd.setCursor(0, 0);
  String hs="HUMIDITY: "+String((int)humidity)+" % ";
  String ts="TEMP: "+String((int)temp_f)+" C ";
  lcd.setCursor(0, 0);
  lcd.print(ts);
  lcd.setCursor(0, 1);
  lcd.print(hs);
  
} 

void gettemperature() {
  unsigned long currentMillis = millis();
 if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;   
    humidity = dht.readHumidity();          // Read humidity (percent)
    temp_f = dht.readTemperature(false);     // Read temperature as Celsius
    if (isnan(humidity) || isnan(temp_f)) {
      humidity=0;
      temp_f=0;
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }
}
