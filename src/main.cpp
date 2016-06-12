// https://github.com/adafruit/DHT-sensor-library
#include <DHT.h>

// https://github.com/wayoda/LedControl
#include <LedControl.h>

// http://playground.arduino.cc/Code/HashMap
#include <HashMap.h>

// http://playground.arduino.cc/Code/StackArray
#include <StackArray.h>

// DHT Sensor
#define DHT_TYPE DHT11   // DHT 11
// #define DHT_TYPE DHT22   // DHT 22  (AM2302), AM2321 (untested)
// #define DHT_TYPE DHT21   // DHT 21 (AM2301) (untested)
#define DHT_PIN 2

// LED Matrix pins
#define LED_DATA_IN 12
#define LED_CLK 11
#define LED_LOAD 10
#define LED_LIGHT_INTENSITY 7 // 0 - 15

// Etc
#define DELAY_TIME 1000 // In milliseconds
#define INVALID_VALUE 4000

// Initialize DHT
DHT dht(DHT_PIN, DHT_TYPE);

// Initialize LedControl
LedControl lc = LedControl(LED_DATA_IN, LED_CLK, LED_LOAD, 1);

// Initialize HashMap
// HashMap is used to map between temperature and which LED lights to light
const byte HASH_SIZE = 9;
HashType<int, int> hashRawArray[HASH_SIZE];
HashMap<int, int> hashMap = HashMap<int, int>(hashRawArray, HASH_SIZE);

void setupValueMapping()
{
        hashMap[0](0, 0); // Zero LEDs
        hashMap[1](1, 1); // One LED
        hashMap[2](2, 3); // Two LEDs
        hashMap[3](3, 7); // Three LEDs
        hashMap[4](4, 15); // Four LEDs
        hashMap[5](5, 31); // Five LEDs
        hashMap[6](6, 63); // Six LEDs
        hashMap[7](7, 127); // Seven LEDs
        hashMap[8](8, 255); // Eight LEDs
}

void setup()
{
        Serial.begin(9600);
        while (!Serial)
        {
                ; // Wait for serial port to connect. Needed for native USB port only
        }

        setupValueMapping();

        lc.shutdown(0, false);
        lc.setIntensity(0, LED_LIGHT_INTENSITY);
        lc.clearDisplay(0);

        dht.begin();
}

int getTemperature()
{
        float t = dht.readTemperature();

        // Check if read failed and exit early
        if (isnan(t))
        {
                Serial.println("Failed to read temperature from DHT sensor");
                return INVALID_VALUE;
        }

        t = floor(t);
        return t;
}

int getHumidity()
{
        float h = dht.readHumidity();

        // Check if any reads failed and exit early
        if (isnan(h))
        {
                Serial.println("Failed to read humidity from DHT sensor");
                return INVALID_VALUE;
        }

        h = floor(h);
        return h;
}

void setLedValues(int temperature, int *ledValues)
{
        StackArray <int> stack;
        do
        {
                if (temperature > 8)
                {
                        stack.push(8);
                        temperature -= 8;
                }
                else
                {
                        if (temperature < 0)
                        {
                                temperature = 0;
                        }
                        stack.push(temperature);
                        temperature = 0;
                }
        }
        while (stack.count() < 4);

        ledValues[3] = stack.pop();
        ledValues[2] = stack.pop();
        ledValues[1] = stack.pop();
        ledValues[0] = stack.pop();
}

void lightLeds(int *values, boolean isTemperature = true)
{
        int startIndex = (isTemperature ? 0 : 4);
        for (int i = 0; i < 4; i++)
        {
                lc.setColumn(0, startIndex, hashMap.getValueOf(values[i]));
                startIndex++;
        }
}

void lightTemperatureLeds(float t)
{
        int values[4] = {};
        setLedValues(t, values);
        lightLeds(values);
}

void lightHumidityLeds(float t)
{
        int values[4] = {};
        setLedValues(t, values);
        lightLeds(values, false);
}

void printDebug(float temperature, float humidity)
{
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%\t");
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println("C ");
}

void loop()
{
        int temperature = getTemperature();
        int humidity = getHumidity();

        if (temperature == INVALID_VALUE || humidity == INVALID_VALUE)
        {
                return;
        }

        lightTemperatureLeds(temperature);
        lightHumidityLeds(humidity);

        // printDebug(temperature, humidity);

        delay(DELAY_TIME);
}
