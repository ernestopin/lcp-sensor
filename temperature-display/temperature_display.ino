#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>

/* Temperature sensors */
#define TEMP_BUS 2
#define TEMP_LT 0
#define TEMP_MT 1

/* Temperature sensor */
OneWire oneWire(TEMP_BUS);
DallasTemperature sensors(&oneWire);

/* LED displays */
#define DISP_LT 0x70
#define DISP_MT 0x71

Adafruit_7segment led[2] = {Adafruit_7segment(), Adafruit_7segment()};

/* Globals */
float temps[2] = {0, 0};
float ref_low = 17;
float ref_high = 33;
float raw_high[2] = {33.63, 33.25};
float raw_low[2] = {18.19, 17.94};

void setup()
{
	sensors.begin();
	sensors.requestTemperatures();

	led[0].begin(0x70);
	led[1].begin(0x71);

	Serial.begin(9600);
}

void loop()
{
	displayTemp(sensors.getTempCByIndex(0), 0);
	displayTemp(sensors.getTempCByIndex(1), 1);

	sensors.requestTemperatures();
}

void displayTemp(float read, int temp) {
	if (temps[temp] != read) {
		temps[temp] = (read - raw_low[temp]) * (ref_high - ref_low) / (raw_high[temp] - raw_low[temp]) + ref_low;
		led[temp].print(temps[temp]);
		led[temp].writeDisplay();
	}
}

