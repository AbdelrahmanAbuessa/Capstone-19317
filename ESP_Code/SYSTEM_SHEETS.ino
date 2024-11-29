// -------------------- Time Delay Define Start ----------------
const int timeDelay = 5;
// -------------------- Time Delay Define End ----------------

// -------------------- Wi-Fi Define Start ----------------
#include "WiFi.h"
#include <HTTPClient.h>
#define On_Board_LED_PIN  2

// const char* ssid = "Nodownload";  
// const char* password = "EI2007FAt2002";

const char* ssid = "Bandit enters the room";  
const char* password = "handitbeeler";

String Web_App_URL = "https://script.google.com/macros/s/AKfycbyRwU0yobBtxFwtg1iwCkVl7aJEzueR3ni_nVmH9s4PgmRQmdxrdKk2jVSa0c0PVxQq/exec";
float T;
float H;
float S;
// -------------------- Wi-Fi Define End ----------------

// --------------------------- MQ-2 Define Start -----------------------------
#include <MQUnifiedsensor.h>
#define         Board                   ("ESP-32")
#define         Pin                     (35) 
#define         Type                    ("MQ-2")
#define         Voltage_Resolution      (5)
#define         ADC_Bit_Resolution      (12)
#define         RatioMQ2CleanAir        (9.83)

#define MQSensorTest 13

float CO;
MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);
// --------------------------- MQ-2 Define End -----------------------------

// --------------------------- DHT22 Define Start -----------------------------
#include <DHT.h>
#include <DHT_U.h>
#define DHT22_GPIO 23
DHT dht22(DHT22_GPIO, DHT22);
float humid;
// --------------------------- DHT22 Define End -----------------------------

// --------------------------- DS18B20 Define Start -----------------------------
#include <OneWire.h>
#include <DallasTemperature.h>
#define dataTemp_bus 15
OneWire oneWire(dataTemp_bus);
DallasTemperature sensors(&oneWire);
float temp_1;
float temp_2;
float avg_temp;
DeviceAddress sensor1 = { 0x28, 0x5E, 0x28, 0x13, 0x0, 0x0, 0x0, 0x55 };
DeviceAddress sensor2 = { 0x28, 0xAD, 0xE8, 0x12, 0x0, 0x0, 0x0, 0xD3 };
// --------------------------- DS18B20 Define End -----------------------------

// --------------------------- Alarm Define Start -----------------------------
int temp_V1 = 12;
int temp_V2 = 14;
int temp_V3 = 27;

int cot_V1 = 5;
int cot_V2 = 18;
int cot_V3 = 19;

int humid_V1 = 25;
int humid_V2 = 33;
int humid_V3 = 32;

int buzz_lightyear = 21;

int soundState = LOW;  // soundState used to set the LED
unsigned long pre_time = 0;  // will store last time LED was updated
const long time_period = 300;  // time_period at which to blink (milliseconds)

int temp_level_1 = 30;
int temp_level_2 = 45;
int temp_level_3 = 60;

int humid_level_1 = 75;
int humid_level_2 = 80;
int humid_level_3 = 95;

int cot_level_1 = 65;
int cot_level_2 = 95;
int cot_level_3 = 125;

int temp_level;
int cot_level;
int humid_level;
bool buzz_level;

int alarm_pins[10] = {temp_V1, temp_V2, temp_V3, cot_V1, cot_V2, cot_V3, humid_V1, humid_V2, humid_V3, buzz_lightyear};
// --------------------------- Alarm Define End -----------------------------

void setup() {
  pinMode(MQSensorTest, INPUT); 
  // Serial Communication Started
  Serial.begin(9600);

  // ----- Alarm Setup Start ---------
  for (int i = 0; i < 10; i++) {
    pinMode(alarm_pins[i], OUTPUT);
  }
  // ----- Alarm Setup End ---------

  // ----- MQ-2 Setup Start ---------
  MQ2.setRegressionMethod(1);
  MQ2.setA(36974); MQ2.setB(-3.109);
  /*
    Exponential regression:
    Gas    | a      | b
    H2     | 987.99 | -2.162
    LPG    | 574.25 | -2.222
    CO     | 36974  | -3.109
    Alcohol| 3616.1 | -2.675
    Propane| 658.71 | -2.168
  */
  MQ2.init(); 
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ2.update();
    calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
  }
  MQ2.setR0(calcR0/10);
  MQ2.serialDebug(true);
  // ----- MQ-2 Setup End ---------

  // ----- DHT22 Setup Start ---------
  dht22.begin();
  // ----- DHT22 Setup End ---------

  // ----- DS18B20 Setup Start ---------
  sensors.begin();
  // ----- DS18B20 Setup End ---------

  // ----- Wi-Fi Setup Start ---------
  pinMode(On_Board_LED_PIN, OUTPUT);
  // Set Wifi to STA mode
  Serial.println();
  Serial.println("-------------");
  Serial.println("WIFI mode : STA");
  WiFi.mode(WIFI_STA);
  Serial.println("-------------");
  // Connect to Wi-Fi (STA).
  Serial.println();
  Serial.println("------------");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(On_Board_LED_PIN, HIGH);
    delay(250);
    digitalWrite(On_Board_LED_PIN, LOW);
    delay(250);
    if (connecting_process_timed_out > 0) connecting_process_timed_out--;
    if (connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }
  digitalWrite(On_Board_LED_PIN, LOW);
  Serial.println("WiFi connected");
  // ----- Wi-Fi Setup End ---------
}

void loop() {
 
  // ----- MQ-2 Loop Start ---------
  MQ2.update();
  CO = MQ2.readSensor();
  // ----- MQ-2 Loop End ---------

  // ----- DHT22 Loop Start ---------
  humid = dht22.readHumidity();
  // ----- DHT22 Loop End ---------

  // ----- DS18B20 Loop Start ---------
  sensors.requestTemperatures(); // Send the command to get temperatures
  temp_1 = sensors.getTempC(sensor1);
  temp_2 = sensors.getTempC(sensor2);
  avg_temp = temp_2; // EDIT BEFORE UPLOADING IMPORTANT AF LMAO
  // ----- DS18B20 Loop End ---------

  // ----- Alarm TEST Loop Start ---------
  if (CO < cot_level_1) {
    cot_level = 0;
    buzz_level = false;
  } else if (CO > cot_level_1 && CO < cot_level_2) {
    cot_level = 1;
    buzz_level = true;
  } else if (CO > cot_level_2 && CO < cot_level_3) {
    cot_level = 2;
    buzz_level = true;
  } else if (CO > cot_level_3) {
    cot_level = 3;
    buzz_level = true;
  }

  if (avg_temp < temp_level_1) {
    temp_level = 0;
    buzz_level = false;
  } else if (avg_temp > temp_level_1 && avg_temp < temp_level_2) {
    temp_level = 1;
    buzz_level = true;
  } else if (avg_temp > temp_level_2 && avg_temp < temp_level_3) {
    temp_level = 2;
    buzz_level = true;
  } else if (avg_temp > temp_level_3) {
    temp_level = 3;
    buzz_level = true;
  }

  if (humid < humid_level_1) {
    humid_level = 0;
    buzz_level = false;
  } else if (humid > humid_level_1 && humid < humid_level_2) {
    humid_level = 1;
    buzz_level = true;
  } else if (humid > humid_level_2 && humid < humid_level_3) {
    humid_level = 2;
    buzz_level = true;
  } else if (humid > humid_level_3) {
    humid_level = 3;
    buzz_level = true;
  }

  alarm_temp(temp_level);
  alarm_humid(humid_level);
  alarm_cot(cot_level);
  alarm_buzz(buzz_level);
  // ----- Alarm TEST Loop End ---------

  // ----- Wi-Fi Loop Start ---------
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(On_Board_LED_PIN, HIGH);

    // Create a URL for sending or writing data to Google Sheets.
    T = avg_temp;
    H = humid;
    S = CO;
    String Send_Data_URL = Web_App_URL + "?sts=write" + "&temp=" + T + "&humid=" + H + "&cot=" + S;

    // Initialize HTTPClient as "http".
    HTTPClient http;
    // HTTP GET Request.
    http.begin(Send_Data_URL.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    // Gets the HTTP status code.
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code : ");
    Serial.println(httpCode);
    // Getting response from google sheets.
    String payload;
    if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Payload : " + payload);    
    }
      
    http.end();
    
    digitalWrite(On_Board_LED_PIN, LOW);
  }
  // ----- Wi-Fi Loop End ---------

  // Delay 0.1s
  auto time = millis();
  while (time % (timeDelay * 1000) >= 50) {
    time = millis();
    delay(2);
  }
}

int alarm_temp(int level) {
  switch (level) {
    case 0:
      digitalWrite(temp_V1, LOW);
      digitalWrite(temp_V2, LOW);
      digitalWrite(temp_V3, LOW);
      break;

    case 1:
      digitalWrite(temp_V1, HIGH);
      digitalWrite(temp_V2, LOW);
      digitalWrite(temp_V3, LOW);
      break;
    
    case 2:
      digitalWrite(temp_V1, HIGH);
      digitalWrite(temp_V2, HIGH);
      digitalWrite(temp_V3, LOW);
      break;

    case 3:
      digitalWrite(temp_V1, HIGH);
      digitalWrite(temp_V2, HIGH);
      digitalWrite(temp_V3, HIGH);
      break;
  }
  return 0;
}

int alarm_humid(int level) {
  switch (level) {
    case 0:
      digitalWrite(humid_V1, LOW);
      digitalWrite(humid_V2, LOW);
      digitalWrite(humid_V3, LOW);
      break;

    case 1:
      digitalWrite(humid_V1, HIGH);
      digitalWrite(humid_V2, LOW);
      digitalWrite(humid_V3, LOW);
      break;
    
    case 2:
      digitalWrite(humid_V1, HIGH);
      digitalWrite(humid_V2, HIGH);
      digitalWrite(humid_V3, LOW);
      break;

    case 3:
      digitalWrite(humid_V1, HIGH);
      digitalWrite(humid_V2, HIGH);
      digitalWrite(humid_V3, HIGH);
      break;
  }
  return 0;
}

int alarm_cot(int level) {
  switch (level) {
    case 0:
      digitalWrite(cot_V1, LOW);
      digitalWrite(cot_V2, LOW);
      digitalWrite(cot_V3, LOW);
      break;

    case 1:
      digitalWrite(cot_V1, HIGH);
      digitalWrite(cot_V2, LOW);
      digitalWrite(cot_V3, LOW);
      break;
    
    case 2:
      digitalWrite(cot_V1, HIGH);
      digitalWrite(cot_V2, HIGH);
      digitalWrite(cot_V3, LOW);
      break;

    case 3:
      digitalWrite(cot_V1, HIGH);
      digitalWrite(cot_V2, HIGH);
      digitalWrite(cot_V3, HIGH);
      break;
  }
  return 0;
}

int alarm_buzz(int state) {
  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the time_period at which you want to blink the LED.
  unsigned long current_time = millis();
  if (current_time - pre_time >= time_period) {
    // save the last time you blinked the LED
    pre_time = current_time;
    // if the LED is off turn it on and vice-versa:
    if (state) {
      if (soundState == LOW) {
        soundState = HIGH;
      } else {
        soundState = LOW;
      }
    }
    // set the LED with the soundState of the variable:
    digitalWrite(buzz_lightyear, soundState);
  }

  return 0;
}