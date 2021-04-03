//Portfolyomda da belirttiğim gibi bu kodu yalnız başına yazmadığımı başka bir 
//siteden incelediğim kodları anlayarak tekrar kendime göre derlediğimi ve eklemeler 
//yaparak tekrar yazdığımı belirtmek istiyorum.

#define USE_ARDUINO_INTERRUPTS true    //Analog okumada değerleri düzgün okuması için kesmeleri çalışmasını istiyorum.
#include <PulseSensorPlayground.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>             

#define SCREEN_WIDTH 128                //OLED ekranımın genişlik yükseklik bilgisini tanımladım.
#define SCREEN_HEIGHT 64

//Bu kısımda SCL ve SDA pinlerini ekran bağlantısı için tanımamız gerekiyormuş. 
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int pulseAnalog = A0;               //Nabız değerininverisini A0 analog pininden çek.
const int LED13 = 13;                     //Arduinodaki ledin nabızla beraber yanıp yanıp sön. 
int Threshold = 600;                       //Azami nabız değerimiz
int nabiz;                                //Nabız sayısını tutacak değişken
int sinyal;
int ledK = 6;                             //Kırmızı ledi 6. digital pine bağladım.


PulseSensorPlayground pulseSensor;        //Sensörümüzü kodumuzda kullanabilmek için onu obje olarak oluşturduk.


void setup() {
  Serial.begin(9600);

  pinMode(ledK , OUTPUT);
  
  pulseSensor.analogInput(pulseAnalog);   //pulseAnalog değişkeninin giriş değeri olduğunu analog değerler beklediğimizi belirtiyorum 
  pulseSensor.blinkOnPulse(LED13);        //Arduino üzerindeki ledin nabzımızla yanıp sönmesini istediğimizi söylüyoruz.
  pulseSensor.setThreshold(Threshold);     //Sensore azami nabız sayısını tanıttık.


// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
// BU KISMA dosya -> örnekler -> Adafruit1306 -> 128x64_i2c yolundan ulaşabilirsiniz.
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
  }


  // Draw a single pixel in white
  display.drawPixel(10, 10, WHITE);

  display.display();
  delay(500);

  display.clearDisplay();

//OLED NABIZ ALMAYA BAŞLAMADAN ÖNCEKİ DURUMU
  display.setTextSize(2); 
  display.setTextColor(WHITE);
  display.setCursor(5, 0);
  display.println(F("Yukleniyor"));
  display.display();      
  delay(100);
  

  if (pulseSensor.begin()) {
    Serial.println("Pulse sensörü bağlantısı başarılı."); 
  }
  
}

void loop() {
   if (pulseSensor.sawStartOfBeat()) 
   { //Eğer nabız algılarsak
      sinyal = analogRead(pulseAnalog);
      nabiz = map(sinyal , 0 , 1000 , 0 , 300);
  
      display.clearDisplay();                     //OLEDi temizledik
      display.setTextSize(2);                     //yazı kalınlığını seçtik
      display.setTextColor(WHITE);                //yazı rengini belirledik
      display.setCursor(33, 0);                   //Nabiz: yazısının konumunu belirttik
      display.println(F("Nabiz:"));
      display.setCursor(50, 30);                  //nabiz değerinin görüneceği konumu belirttik
      display.println(nabiz);
      display.display();                          //çalışması için
      delay(100);                                 //100ms beklettik

      Serial.println("Nabız ");
      Serial.print("BPM: ");                        
      Serial.println(nabiz);
    }



  if(sinyal > Threshold)
  {
    digitalWrite(ledK , HIGH);
    delay(1000);
    digitalWrite(ledK , LOW);
  }
  else
  {
    digitalWrite(ledK , LOW);  
  }

  delay(20); //20 milisaniye bekletiyoruyuz.
}
