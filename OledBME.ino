#include "config.h"

/*************************************/
/*  G L O B A L   V A R I A B L E S  */
/*************************************/
unsigned long lastSensorRead;				// When last sensor was read
String input = "";					// Holds string from serial console

/***********************/
/*  F U N C T I O N S  */
/***********************/

/*
 *  updateSensorData()
 *  returns         - none
 *  paramaters      - none
 *
 *  Uses preprocessor directive to update data for sensors defined as being used
 *  in config.h
 */
void
updateSensorData() {
#ifdef USE_BME_SENSOR
	updateBME();
#endif // USE_BME_SENSOR

#ifdef USE_MLX_SENSOR
	updateMLX();
#endif // USE_MLX_SENSOR

#ifdef USE_TSL2591_SENSOR
	updateTSL2591();
#endif // USE_TSL_SENSOR

#ifdef USE_DAVIS_SENSOR
	readAnemometer();
#endif // USE_DAVIS_SENSOR
}

/*
 *  getCurrentConfig
 *  returns         - String object
 *  paramater       - none
 *
 *  Forms a JSON object using ArduinoJson library
 *  This is the configuration of various sensors and the WiFi
 */
String
getCurrentConfig() {
	const int docSize = JSON_OBJECT_SIZE(7) + // max 7 configurations
		JSON_OBJECT_SIZE(2) + // DHT sensors
		JSON_OBJECT_SIZE(3) + // Davis Anemometer
		JSON_OBJECT_SIZE(1) + // Water sensor
		JSON_OBJECT_SIZE(2) + // Rain Sensor
		JSON_OBJECT_SIZE(3) + // WiFi parameters
		JSON_OBJECT_SIZE(1) + // Arduino
		JSON_OBJECT_SIZE(3) + // OTA
		JSON_OBJECT_SIZE(2);  // buffer
	StaticJsonDocument <docSize> doc;

#ifdef USE_WIFI
	// currently, we have memory info only available for ESP8266
	JsonObject arduinodata = doc.createNestedObject("Arduino");
	arduinodata["free memory"] = freeMemory();
#endif

#ifdef USE_DHT_SENSOR
	JsonObject dhtdata = doc.createNestedObject("DHT");
	dhtdata["pin"]  = DHTPIN;
	dhtdata["type"] = DHTTYPE;
#endif

#ifdef USE_DAVIS_SENSOR
	JsonObject davisdata               = doc.createNestedObject("Davis Anemometer");
	davisdata["wind speed pin"]        = ANEMOMETER_WINDSPEEDPIN;
	davisdata["wind direction pin"]    = ANEMOMETER_WINDDIRECTIONPIN;
	davisdata["wind direction offset"] = ANEMOMETER_WINDOFFSET;
#endif

#ifdef USE_WATER_SENSOR
	JsonObject waterdata = doc.createNestedObject("Water");
	waterdata["pin"] = WATER_PIN;
#endif
#ifdef USE_RAIN_SENSOR
	JsonObject rainsensordata          = doc.createNestedObject("Rain Sensor");
	rainsensordata["rain sensor pin"]  = RAINSENSOR_PIN;
	rainsensordata["bucket size"]      = RAINSENSOR_BUCKET_SIZE;
#endif //USE_RAIN_SENSOR

#ifdef USE_WIFI
	JsonObject wifidata = doc.createNestedObject("WiFi");
	wifidata["SSID"] = WiFi.SSID();
	wifidata["connected"] = WiFi.status() == WL_CONNECTED;
	if (WiFi.status() == WL_CONNECTED)
		wifidata["IP"]        = WiFi.localIP().toString();
	else
		wifidata["IP"]        = "";
#endif

#ifdef USE_OTA
	serializeOTA(doc);
#endif // USE_OTA

	String result = "";
	serializeJson(doc, result);

	if (doc.isNull())
		return "{}";
	else {
		return result;
	}
}

/*******************************************************************/
/*  O u t p u t   t h i n g s   t o   s e r i a l   c o n s o l e  */
/*                                                                 */
/*  - d   h e a d l i n e                                          */
/*******************************************************************/

/*
 *  outputSensorData()
 *
 *  returns         - none
 *  input           - none
 *
 *  Triggered in response to the 'w' command at the serial console
 *  Goes through configured sensors and outputs the information required
 *  by the 'w' command
 *  -d c-cmt2
 */
void
outputSensorData() {
	// Record if we have found a sensor so we can process if we need to
	// output a comma when the next sensor is found
	bool hasBeenOne = false;

	Serial.print(jsonS4);				// The start brace

#ifdef USE_BME_SENSOR
	updateBMEstring(bufferString);			// Create string holding Json with sensor readings
	if (hasBeenOne)
		Serial.print(jsonS2);			// , A comma
	Serial.print(bufferString);
	hasBeenOne = true;
#endif //USE_BME_SENSOR

#ifdef USE_MLX_SENSOR
	updateMLX90614string(bufferString);		// Create string holding Json with sensor readings
	if (hasBeenOne)
		Serial.print(jsonS2);			// , A comma
	Serial.print(bufferString);
	hasBeenOne = true;
#endif // USE_MLX_SENSOR

#ifdef USE_TSL2591_SENSOR
	updateTSL2591string(bufferString);		// Create string holding Json with sensor readings
	if (hasBeenOne)
		Serial.print(jsonS2);			// , A comma
	Serial.print(bufferString);
	hasBeenOne = true;
#endif // USE_TSL_SENSOR

#ifdef USE_DAVIS_SENSOR
/* TODO updateDAVISstring(bufferString); */
	if (hasBeenOne)
		Serial.print(jsonS2);			// , A comma
	Serial.print(bufferString);
	hasBeenOne = true;
#endif // USE_DAVIS_SENSOR

	Serial.println(jsonS5);				// The end brace
}

/*
 *  outputVersion()
 *
 *  returns         - none
 *  input           - none
 *
 *  Triggered in response tot he 'v' command at the serial console
 *  Outputs json string {"version":"0.1"}
 *  -d c-cmt2
 */
void
outputVersion() {
	Serial.print(jsonS4);				// Start brace
	Serial.print(F("\"version\":"));
	Serial.print(jsonS1);				// "
	Serial.print(OLEDBME_VERSION);
	Serial.print(jsonS1);				// "
	Serial.println(jsonS5);				// End brace
}

/*******************************************************************************/
/*  M a i n   p a r s e   f u n c t i o n   f o r   s e r i a l   o u t p u t  */
/*******************************************************************************/

/*
 *  parseInput()
 *  returns         - none
 *  paramaters      - none
 *
 *  Parses input from serial input and performs the appropriate action
 */
void
parseInput() {
	if (input.length() == 0)			// ignore empty input
		return;

	switch (input.charAt(0)) {
		case 'v':				// Version command
			outputVersion();
			break;
		case 'w':				// Write command
			outputSensorData();
			break;
		case 'c':				// Show current configuration command
			Serial.println(getCurrentConfig());
			break;
		case 'p':				// Pretty print sensor data command
			break;
		case 't':				// Print out timings command
			//Serial.println(getReadDurations());
			break;
#ifdef USE_WIFI
		case 's':
			if (input.length() > 2 && input.charAt(1) == '?')
				parseCredentials(input.substring(2));
			initWiFi();
			break;
		case 'd':
			stopWiFi();
			break;
		case 'r':
			reset();
			break;
#endif // USE_WIFI
	}

}

/***************/
/*  S e t u p  */
/***************/
void setup() {
	Wire.begin();

	Serial.begin(9600);

	while (!Serial)
		continue;				// time to get serial running

	lastSensorRead = 0;				// sensors never read

	sprintf(bufferString, "\n Weather Radio V %s", OLEDBME_VERSION);
	Serial.println(bufferString);

#ifdef USE_DAVIS_SENSOR
	initAnemometer();
#endif							// USE_DAVIS_SENSOR

	updateSensorData();
}


/***************************************/
/*  M a i n   p r o g r a m   l o o p  */
/***************************************/
void loop() {

	byte ch;

#ifdef USE_DAVIS_SENSOR
	updateAnemometer();
#endif							//USE_DAVIS_SENSOR

	if (Serial.available() > 0) {
		ch = Serial.read();

		if (ch == '\r' || ch == '\n') {		// Command received and ready.
			parseInput();
			input = "";
		}
		else
			input += (char)ch;
	}

	// regularly update sensor data
	unsigned long now = millis();
	if (abs(now - lastSensorRead) > MAX_CACHE_AGE) {
		updateSensorData();
		lastSensorRead = now;
	}

	delay(50);
}
