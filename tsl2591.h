#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

#include "shortJson.h"
#include "miscString.h"

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

struct {
  bool status;
  uint32_t full;
  uint16_t ir;
  uint16_t visible;
  int      gain;
  int      timing;
  float    lux;
} tsl2591Data {false, 0, 0, 0, 0, 0, 0.0};

// Store for strings which do not change
char tsl2591S1[] = "TSL2591";
char tsl2591S2[] = "Lux";
char tsl2591S3[] = "Visible";
char tsl2591S4[] = "IR";
char tsl2591S5[] = "Gain";
char tsl2591S6[] = "Timing";

void updateTSL2591string(char * stringBuf) {
	//"TSL2591":{"init":true,"Lux":2.223584,"Visible":782,"IR":178,"Gain":32,"Timing":1}

	jsonStart(stringBuf, tsl2591S1);		// "TSL2591"

	jsonInit(stringBuf, miscS1);			// "init"
	if (tsl2591Data.status == true) {
		// Output true
		strcat(stringBuf, miscS2);		// "true"

		jsonComma(stringBuf);

		jsonInit(stringBuf, tsl2591S2);		// "Lux"
		dtostrf(tsl2591Data.lux, 4, 6, dummyString);  //4 is mininum width, 2 is precision
		strcat(stringBuf, dummyString);
		jsonComma(stringBuf);

		jsonInit(stringBuf, tsl2591S3);		// "Visible"
		sprintf(dummyString, "%d", tsl2591Data.visible); // Yay we can use sprintf as it is an int
		strcat(stringBuf, dummyString);
		jsonComma(stringBuf);

		jsonInit(stringBuf, tsl2591S4);		// "IR"
		sprintf(dummyString, "%d", tsl2591Data.ir); // Yay we can use sprintf as it is an int
		strcat(stringBuf, dummyString);
		jsonComma(stringBuf);

		jsonInit(stringBuf, tsl2591S5);		// "Gain"
		sprintf(dummyString, "%d", tsl2591Data.gain); // Yay we can use sprintf as it is an int
		strcat(stringBuf, dummyString);
		jsonComma(stringBuf);

		jsonInit(stringBuf, tsl2591S6);		// "Timing"
		sprintf(dummyString, "%d", tsl2591Data.timing); // Yay we can use sprintf as it is an int
		strcat(stringBuf, dummyString);
	} else {
		// Output false
		strcat(stringBuf, miscS3);		// "false"
	}
	jsonEnd(stringBuf);
}


bool isTSL2591Present() {
  Wire.beginTransmission(TSL2591_ADDR);
  byte error = Wire.endTransmission();

  return (error == 0);
}


void configureSensorTSL2591(tsl2591Gain_t gainSetting, tsl2591IntegrationTime_t timeSetting)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  tsl.setGain(gainSetting);

  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  tsl.setTiming(timeSetting);
}


// calibrate TSL2591 gain and integration time
bool calibrateTSL2591() {
  if (tsl2591Data.visible < 100) { //Increase GAIN (and INTEGRATIONTIME) if light level too low
    switch (tsl2591Data.gain)
    {
      case TSL2591_GAIN_LOW :
        configureSensorTSL2591(TSL2591_GAIN_MED, TSL2591_INTEGRATIONTIME_200MS);
        break;
      case TSL2591_GAIN_MED :
        configureSensorTSL2591(TSL2591_GAIN_HIGH, TSL2591_INTEGRATIONTIME_200MS);
        break;
      case TSL2591_GAIN_HIGH :
        configureSensorTSL2591(TSL2591_GAIN_MAX, TSL2591_INTEGRATIONTIME_200MS);
        break;
      case TSL2591_GAIN_MAX :
        switch (tsl2591Data.timing)
        {
          case TSL2591_INTEGRATIONTIME_200MS :
            configureSensorTSL2591(TSL2591_GAIN_MAX, TSL2591_INTEGRATIONTIME_300MS);
            break;
          case TSL2591_INTEGRATIONTIME_300MS :
            configureSensorTSL2591(TSL2591_GAIN_MAX, TSL2591_INTEGRATIONTIME_400MS);
            break;
          case TSL2591_INTEGRATIONTIME_400MS :
            configureSensorTSL2591(TSL2591_GAIN_MAX, TSL2591_INTEGRATIONTIME_500MS);
            break;
          case TSL2591_INTEGRATIONTIME_500MS :
            configureSensorTSL2591(TSL2591_GAIN_MAX, TSL2591_INTEGRATIONTIME_600MS);
            break;
          case TSL2591_INTEGRATIONTIME_600MS :
            // no higher sensitivity level available
            return false;
            break;
          default:
            configureSensorTSL2591(TSL2591_GAIN_MAX, TSL2591_INTEGRATIONTIME_600MS);
            break;
        }
        break;
      default:
        configureSensorTSL2591(TSL2591_GAIN_MED, TSL2591_INTEGRATIONTIME_200MS);
        break;
    }

    // calibration changed
    return true;
  }

  if (tsl2591Data.visible > 30000) { //Decrease GAIN (and INTEGRATIONTIME) if light level too high
    switch (tsl2591Data.gain)
    {
      case TSL2591_GAIN_LOW :
        switch (tsl2591Data.timing)
        {
          case TSL2591_INTEGRATIONTIME_500MS :
            configureSensorTSL2591(TSL2591_GAIN_LOW, TSL2591_INTEGRATIONTIME_400MS);
            break;
          case TSL2591_INTEGRATIONTIME_400MS :
            configureSensorTSL2591(TSL2591_GAIN_LOW, TSL2591_INTEGRATIONTIME_300MS);
            break;
          case TSL2591_INTEGRATIONTIME_300MS :
            configureSensorTSL2591(TSL2591_GAIN_LOW, TSL2591_INTEGRATIONTIME_200MS);
            break;
          case TSL2591_INTEGRATIONTIME_200MS :
            // no higher sensitivity level available
            return false;
            break;

          default:
            configureSensorTSL2591(TSL2591_GAIN_LOW, TSL2591_INTEGRATIONTIME_200MS);
            break;
        }
        break;
      case TSL2591_GAIN_MED :
        configureSensorTSL2591(TSL2591_GAIN_LOW, TSL2591_INTEGRATIONTIME_200MS);
        break;
      case TSL2591_GAIN_HIGH :
        configureSensorTSL2591(TSL2591_GAIN_MED, TSL2591_INTEGRATIONTIME_200MS);
        break;
      case TSL2591_GAIN_MAX :
        configureSensorTSL2591(TSL2591_GAIN_HIGH, TSL2591_INTEGRATIONTIME_200MS);
        break;
      default:
        configureSensorTSL2591(TSL2591_GAIN_MED, TSL2591_INTEGRATIONTIME_200MS);
        break;
    }
    // calibraton changed
    return true;
  }

  // no calibration change necessary
  return false;

}

void updateTSL2591(void)
{
  if (tsl2591Data.status || (tsl2591Data.status = isTSL2591Present())) {
    tsl.begin();
    tsl2591Data.full = tsl.getFullLuminosity();
    tsl2591Data.ir = tsl2591Data.full >> 16;
    tsl2591Data.visible = tsl2591Data.full & 0xFFFF;
    tsl2591Data.lux = tsl.calculateLux(tsl2591Data.visible, tsl2591Data.ir);
    tsl2591Data.gain    = tsl.getGain();
    tsl2591Data.timing  = tsl.getTiming();

    bool changed = calibrateTSL2591();
    if (changed) updateTSL2591();
  }
}

void serializeTSL2591(JsonDocument &doc) {

  JsonObject data = doc.createNestedObject("TSL2591");
  data["init"] = tsl2591Data.status;

  if (tsl2591Data.status) {
    data["Lux"]     = tsl2591Data.lux;
    data["Visible"] = tsl2591Data.visible;
    data["IR"]      = tsl2591Data.ir;
    data["Gain"]    = tsl2591Data.gain;
    data["Timing"]  = tsl2591Data.timing;
  }
}

void print_TSL(void)
{
  Serial.println(F("TSL2591"));
  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print(F("IR: ")); Serial.print(tsl2591Data.ir);  Serial.print(F("  "));
  Serial.print(F("Full: ")); Serial.print(tsl2591Data.full); Serial.print(F("  "));
  Serial.print(F("Visible: ")); Serial.print(tsl2591Data.visible); Serial.print(F("  "));
  Serial.print(F("Lux: ")); Serial.println(tsl2591Data.lux, 6);
  Serial.println();
}
