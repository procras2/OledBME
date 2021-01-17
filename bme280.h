#include <Adafruit_BME280.h>

#include "shortJson.h"
#include "miscString.h"

Adafruit_BME280 bme;

struct {
	bool status;
	float temperature;
	float pressure;
	float humidity;
} bmeData;

// Store for strings which do not change
char bmeS1[] = "BME280";
char bmeS2[] = "Temp";
char bmeS3[] = "Pres";
char bmeS4[] = "Hum";

void updateBMEstring(char * stringBuf) {
	//"BME280":{"init":true,"Temp":16.30,"Pres":1015.28,"Hum":52.64}

	jsonStart(stringBuf, bmeS1); // BME280

	jsonInit(stringBuf, miscS1); // init
	if (bmeData.status == true) {
		// Output true
		strcat(stringBuf, miscS2); // true

		jsonComma(stringBuf);

		jsonInit(stringBuf, bmeS2); // Temp
		dtostrf(bmeData.temperature, 4, 2, dummyString);  //4 is mininum width, 2 is precision
		strcat(stringBuf, dummyString);
		jsonComma(stringBuf); //,

		jsonInit(stringBuf, bmeS3); // Pres
		dtostrf(bmeData.pressure, 7, 2, dummyString);  //7 is mininum width, 2 is precision
		strcat(stringBuf, dummyString);
		jsonComma(stringBuf); //,

		jsonInit(stringBuf, bmeS4); // Hum
		dtostrf(bmeData.humidity, 4, 2, dummyString);  //4 is mininum width, 2 is precision
		strcat(stringBuf, dummyString);

	} else {
		// Output false
		strcat(stringBuf, miscS3); // false
	}

	jsonEnd(stringBuf); // right brace
}

void updateBME() {
	if (bmeData.status || (bmeData.status = bme.begin()) || (bmeData.status = bme.begin(BME280_ADDRESS_ALTERNATE))) {

		bmeData.temperature = bme.readTemperature();
		bmeData.pressure    = bme.readPressure() / 100.0;
		bmeData.humidity    = bme.readHumidity();

	}
}

void serializeBME(JsonDocument &doc) {

	JsonObject data = doc.createNestedObject("BME280");
	data["init"] = bmeData.status;

	if (bmeData.status) {
		data["Temp"] = bmeData.temperature;
		data["Pres"] = bmeData.pressure;
		data["Hum"] = bmeData.humidity;
	}
}

void print_BME(void)
{
	Serial.println(F("BME280"));
	if (bmeData.status == false) {
		Serial.println(F("No data"));
	} else {
		Serial.print(F("T = "));
		Serial.print(bmeData.temperature);
		Serial.println(F(" *C"));

		Serial.print(F("P = "));
		Serial.print(bmeData.pressure);
		Serial.println(F(" hPa"));

		Serial.print(F("H = "));
		Serial.print(bmeData.humidity);
		Serial.println(F(" %"));
		Serial.println();
	}

}
