/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-web-server-physical-button/

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/
//PIN D6 connect DHT11
//PIN D4 connect LED
//PIN D3 connect BUTTON
//ESP cấp nguồn 3V không đủ
/******************/
// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 column and 4 rows  
#include <DHT.h>
//#include "html.h"
// Replace with your network credentials
const char* ssid = "Mallorca cafe";
const char* password = "116mallorca";

#define DHTPIN D6     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* PARAM_INPUT_1 = "state";

const int output = 2; //D4
const int buttonPin = D3;    //D2

// Variables will change:
int ledState = LOW;          // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;
float s = 0.0;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

unsigned long previousMillis = 0;    // will store last time DHT was updated
// Updates DHT readings every 10 seconds
const long interval = 10000; 

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
   <div class="LED">
        <h2>ESP Web Server</h2>
        %BUTTONPLACEHOLDER%
        <script>function toggleCheckbox(element) {
        var xhr = new XMLHttpRequest();
        if(element.checked){ xhr.open("GET", "/update?state=1", true); }
        else { xhr.open("GET", "/update?state=0", true); }
        xhr.send();
        }

        setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
            var inputChecked;
            var outputStateM;
            if( this.responseText == 1){ 
                inputChecked = true;
                outputStateM = "On";
            }
            else { 
                inputChecked = false;
                outputStateM = "Off";
            }
            document.getElementById("output").checked = inputChecked;
            document.getElementById("outputState").innerHTML = outputStateM;
            }
        };
        xhttp.open("GET", "/state", true);
        xhttp.send();
        }, 1000 ) ;
        </script>
     <div>
        <h2>ESP8266 SOIL </h2>
         <p>
            <i class="fas fa-tint" style="color:#660000;"></i> 
            <span class="dht-labels">Soil_Moisture</span>
            <span id="soil_moisture">%SOIL_MOISTURE%</span>
            <sup class="units">%</sup>
         </p>
     </div>   
    </div>
     <div class="DHT">
        <h2>ESP8266 DHT Server</h2>
        <p>
            <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
            <span class="dht-labels">Temperature</span> 
            <span id="temperature">%TEMPERATURE%</span>
            <sup class="units">&deg;C</sup>
        </p>
        <p>
            <i class="fas fa-tint" style="color:#00add6;"></i> 
            <span class="dht-labels">Humidity</span>
            <span id="humidity">%HUMIDITY%</span>
            <sup class="units">%</sup>
         </p>
     </div>    
 
     
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("soil_moisture").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/soil_moisture", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";  



String outputState() {
  if (digitalRead(output)) {
    return "checked";
  }
  else {
    return "";
  }
  return "";
}
//Replaces placeholder with button section in your web page
String processor(const String& var) {
  //Serial.println(var);
  if (var == "BUTTONPLACEHOLDER") {
    String buttons = "";
    String outputStateValue = outputState();
    buttons += "<h4>Output - GPIO 2 - State <span id=\"outputState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label>";
    return buttons;
  }

    if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  else if(var == "SOIL_MOISTURE"){
  return String(s);
}
  return String();
}


void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  dht.begin();
  
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      digitalWrite(output, inputMessage.toInt());
      ledState = !ledState;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/state
  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String(digitalRead(output)).c_str());
  });

    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });
  server.on("/soil_moisture", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(s).c_str());
  });  
  // Start server
  server.begin();
}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      // only toggle the LED if the new button state is HIGH
      if (buttonState == 0) {
        delay(200);
        if (buttonState == 0) //kiem tra lan 2
        {
          //lenh chuong trinh
          ledState = ledState + 1;
          if (ledState >= 2)  ledState = 0;
          Serial.print("Trang thai Led: "); 
          Serial.println(ledState);
          if (buttonState == 1); //cho nha phim
        }
      if (ledState == 0) ledState = LOW;
      if (ledState == 1) ledState = HIGH;  
          
    }
  }

}

// set the LED:
digitalWrite(output, ledState);

// save the reading. Next time through the loop, it'll be the lastButtonState:
lastButtonState = reading;

//DHT11
unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    // Read temperature as Celsius (the default)
    float newT = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float newT = dht.readTemperature(true);
    // if temperature read failed, don't change t value
    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      t = newT;
      Serial.print("Nhiet Do: ");       
      Serial.println(t);
    }
    // Read Humidity
    float newH = dht.readHumidity();
    // if humidity read failed, don't change h value 
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      h = newH;
      Serial.print("Do Am: ");       
      Serial.println(h);
    }

    //Soil Sensor
      float SAnalog = analogRead(A0);     // Ta sẽ đọc giá trị hiệu điện thế của cảm biến
    // Giá trị được số hóa thành 1 số nguyên có giá trị
    // trong khoảng từ 0 đến 1023
    float SPhanTramAo = map(SAnalog, 0, 1023, 0, 100);//phần trăm độ khô
    float newS = 100 - SPhanTramAo;
  

    // if SOIL MOISTURE read failed, don't change s value 
    if (isnan(newS)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      s = newS;
      Serial.print("Do Am Dat: ");      
      Serial.println(s);
    }
//LCD
//ShowLCD(int(t), int(h), int(s));    
  }

}


void  ShowLCD(int(t), int(h), int(s))
{
  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print("Nhiet Do: ");        // print message at (0, 0)
  lcd.print(t);        // print message at (0, 0)
    lcd.print(" ");
    lcd.write(1);
    
  lcd.setCursor(0, 1);
  lcd.print("Do Am: ");
  lcd.print(h);
  lcd.print("%");

  lcd.setCursor(0, 2);
  lcd.print("Do Am Dat:");
  lcd.print(s);
  lcd.print("%");
  
  lcd.setCursor(0, 3);
  lcd.print("NGUYEN MINH TRI");

}
