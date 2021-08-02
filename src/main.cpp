#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11 // DHT 11
#define DHTPIN 2

Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7);
int count = 0;
unsigned long delayStart = 0;
bool delayRunning = false;
long delayPeriod = 5000;
int ledButtonPin = 8;
int displayLedPin = 9;
int keshaPin = 10;
bool showKeshaMessage = false;

int keshaButtonState = 0;     // текущее состояние кнопки
int keshaLastButtonState = 0; // предыдущее состояние кнопки
// используем тип без знака, чтобы использовать большие числа
unsigned long lastDebounceTime = 0; // последнее время
unsigned long debounceDelay = 50;   // задержка, увеличьте в случае необходимости

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup()
{
  //pins
  pinMode(ledButtonPin, INPUT);
  pinMode(displayLedPin, OUTPUT);
  pinMode(keshaPin, INPUT);

  digitalWrite(displayLedPin, HIGH);
  // инициализация и очистка дисплея
  display.begin();
  display.clearDisplay();
  display.display();

  display.setContrast(60); // установка контраста
  //delay(1000);
  display.setTextSize(1);      // установка размера шрифта
  display.setTextColor(BLACK); // установка цвета текста
  display.setCursor(0, 0);     // установка позиции курсора

  //display.println("Hello, world!");
  display.println("Kesha, Pidor");
  // display.println(1);
  display.display();

  //meteo init

  Serial.begin(9600);
  // Initialize device.
  dht.begin();
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  delayStart = millis(); // start delay
  delayRunning = true;   // not finished yet
}

void ShowTempAndHum()
{
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    display.println("Error reading temperature!");
  }
  else
  {
    display.print("Temp: ");
    display.print(event.temperature);
    display.print("C\n");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    display.println("Error reading humidity!");
  }
  else
  {
    // display.println("Humidity: " + event.relative_humidity + "%");
    display.print("Hum: ");
    display.print(event.relative_humidity);
    display.print("%\n");
  }
}

void loop()
{
  int buttonState = digitalRead(ledButtonPin);
  if (buttonState == HIGH)
  {
    digitalWrite(displayLedPin, LOW);
    delayRunning = true;
  }

  keshaButtonState = digitalRead(keshaPin);
  if (keshaButtonState != keshaLastButtonState)
  {
    // сбрасываем таймер
    //lastDebounceTime = millis();

    if (keshaButtonState == HIGH)
    {
      showKeshaMessage = !showKeshaMessage;
    }
    delay(50);
  }

  keshaLastButtonState = keshaButtonState;

  display.clearDisplay();
  if (delayRunning && ((millis() - delayStart) >= delayPeriod))
  {
    delayRunning = false; //  prevent this code being run more then once
    digitalWrite(displayLedPin, HIGH);
    delayStart = millis();
  }

  // //display.display();

  // display.setContrast(60); // установка контраста
  // //delay(1000);0
  // display.setTextSize(1);      // установка размера шрифта
  // display.setTextColor(BLACK); // установка цвета текста
  // display.setCursor(0, 0);     // установка позиции курсора

  ShowTempAndHum();
  //additional info
  if (showKeshaMessage == true)
  {
    display.println("Tu Pidor");
  }

  // display.println(count);
  // delay(1000);
  //count = count + 1;

  display.display();
}