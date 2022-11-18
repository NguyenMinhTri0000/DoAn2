#include "DHT.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 16 column and 2 rows
const int DHTPIN = 2;    //D4
const int DHTTYPE = DHT11;

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode (2, INPUT);
  pinMode(A0, INPUT);
  pinMode (12, OUTPUT);   //D6

  lcd.init();                      // initialize the lcd
  lcd.init();  // Print a message to the LCD.
  lcd.backlight();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int SAnalog = analogRead(A0);     // Ta sẽ đọc giá trị hiệu điện thế của cảm biến
  // Giá trị được số hóa thành 1 số nguyên có giá trị
  // trong khoảng từ 0 đến 1023
  int SPhanTramAo = map(SAnalog, 0, 1023, 0, 100);//phần trăm độ khô
  int s = 100 - SPhanTramAo;                      //phần trăm độ ẩm
  if (isnan(t) || isnan(h)) {
    Serial.println(" Failed to read from DHT");
  } else {
    Serial.print("Nhiet do: ");
    Serial.println(t);
    Serial.print("Do am: ");
    Serial.println(h);
  }
  Serial.print("Do am dat: ");
  Serial.println(s);//Xuất ra serial Monitor
  Serial.print("Do am dat: ");
  Serial.println(SAnalog);//Xuất ra serial Monitor

  Serial.println();
  delay(1000);


  ShowLCD(int(t), int(h), int(s));
  // Đọc giá trị D0 rồi điều khiển Led 13...Các bạn cũng có thể điều khiển bơm nước thông qua rơle....
  if (s < 1000)
  {
    digitalWrite (12, HIGH);
  }
  else {
    digitalWrite (12, LOW);
  }
}




void  ShowLCD(int(t), int(h), int(s))
{
  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print("Nhiet Do: ");        // print message at (0, 0)
  lcd.print(t);        // print message at (0, 0)

  lcd.setCursor(0, 1);
  lcd.print("Do Am: ");
  lcd.print(h);


  lcd.setCursor(0, 2);
  lcd.print("Do Am Dat:");
  lcd.print(s);

  lcd.setCursor(0, 3);
  lcd.print("NGUYEN MINH TRI");

}
