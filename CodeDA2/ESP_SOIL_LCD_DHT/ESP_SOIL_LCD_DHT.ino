#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 16 column and 2 rows  
DHTesp dht;

void setup()
{
  Serial.begin(9600);
  
  pinMode (2, INPUT);
  pinMode(A0, INPUT);
  pinMode (12, OUTPUT);   //D6

  // Autodetect is not working reliable, don't use the following line
  // dht.setup(17);
  // use this instead: 
  dht.setup(2, DHTesp::DHT11); // Connect DHT sensor to GPIO 17

  lcd.init();                      // initialize the lcd
  lcd.backlight();
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
 int SAnalog = analogRead(A0);     // Ta sẽ đọc giá trị hiệu điện thế của cảm biến
  // Giá trị được số hóa thành 1 số nguyên có giá trị
  // trong khoảng từ 0 đến 1023
  int SPhanTramAo = map(SAnalog, 0, 1023, 0, 100);//phần trăm độ khô
  int s = 100 - SPhanTramAo;                      //phần trăm độ ẩm
  

  Serial.print("Do am: ");
  Serial.print(humidity);
  Serial.println(" %");  
  Serial.print("Nhiet do: ");
  Serial.print(temperature);
  Serial.println(" C");    
  Serial.print("Do am dat: ");
  Serial.print(s);//Xuất ra serial Monitor
  Serial.println(" %");    
  delay(1000);

  ShowLCD(int(temperature), int(humidity), int(s));
  
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
