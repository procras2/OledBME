#include "version.h"

#include <Wire.h>
#include <ArduinoJson.h>

// Here we select the parts of program we use
// Comment out the ones you do not want
// Uncomment the ones you do want
//#define USE_I2C_SCAN
#define USE_BME_SENSOR
#define USE_MLX_SENSOR
#define USE_TSL2591_SENSOR
//#define USE_DAVIS_VANE0
//#define USE_DAVIS_WIND
//#define USE_DAVIS_SENSOR


// refresh cache interval (ms)
#define MAX_CACHE_AGE 30000

// Here we define the arduino pins we use
//#define DAVIS_VANE_ANALOG_PIN A0
//#define DAVIS_WIND_DIGITAL_PIN 2
// Davis Anemometer
#define ANEMOMETER_WINDSPEEDPIN 2    // The digital pin for the wind speed sensor
#define ANEMOMETER_WINDDIRECTIONPIN A0 // The analog pin for the wind direction
#define ANEMOMETER_WINDOFFSET 0        // anemometer arm direction (0=N, 90=E, ...)


#ifdef USE_I2C_SCAN
#include "i2cscan.h"
#endif // USE_I2C_SCAN

#ifdef USE_BME_SENSOR
#include "bme280.h"
#endif // USE_BME_SENSOR

#ifdef USE_MLX_SENSOR
#include "mlx90614.h"
#endif //USE_MLX_SENSOR

#ifdef USE_TSL2591_SENSOR
#include "tsl2591.h"
#endif //USE_TSL_SENSOR

#ifdef USE_DAVIS_VANE
#include "davisvane.h"
#endif // USE_DAVIS_VANE

#ifdef USE_DAVIS_WIND
#include "daviswind.h"
#endif // USE_DAVIS_WIND

#ifdef USE_DAVIS_SENSOR
#include "davis_anemometer.h"
#endif //USE_DAVIS_SENSOR
