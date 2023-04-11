#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SHT31.h>
#include <ThingsBoard.h>

#define SHT31_ADDRESS 0x44
#define LED 2

#define token "YOUR_TOKEN"
#define thingsboard_server "demo.thingsboard.io"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

char ssid[] = "SSID";
char password[] = "PASSWORD";

WiFiClient client;
ThingsBoard tb(client);

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  pinMode(LED,OUTPUT);

  if(!sht31.begin(SHT31_ADDRESS))
  {
    Serial.println("Error initializing SHT31 sensor!");
    while(1);
  }

  WiFi.begin(ssid, password);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  float temp = sht31.readTemperature();
  float humid = sht31.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" *C");
  Serial.print("Humidity: ");
  Serial.print(humid);
  Serial.println(" %");

  if (!tb.connected())
  {
    Serial.print("Connecting to: ");
    Serial.print(thingsboard_server);
    Serial.print(" with token ");
    Serial.println(token);
    if (!tb.connect(thingsboard_server, token))
    {
      Serial.println("Failed to connect");
      return;
    }
  }

  Serial.println("Sending data...");
  blink();
  tb.sendTelemetryInt("Temperature:", temp);
  tb.sendTelemetryFloat("Humidity: ", humid);

  tb.loop();
  delay(1000);
}

void blink()
{
  delay(500);
  digitalWrite(LED,HIGH);
  delay(500);
  digitalWrite(LED,LOW);
}
