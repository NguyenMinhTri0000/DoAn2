#include <ESP8266WiFi.h>
int i = 0;
const int output = 2;
const int buttonPin = 4;


void setup() {
  // put your setup code here, to run once:
  INPUT_PULLUP;
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(output, OUTPUT);
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:


int reading = digitalRead(buttonPin);

  if (reading == 0) //kiem tra lan 1
  {
    delay(200);
    if (reading == 0) //kiem tra lan 2
    {
      //lenh chuong trinh
      i = i + 1;
      if (i >= 2)  i = 0;
      if (reading == 1); //cho nha phim
    }
  }

  if (i == 0) digitalWrite(output, LOW);
  else digitalWrite(output, HIGH);

  //  Serial.println(reading);
  Serial.println(i);
}
