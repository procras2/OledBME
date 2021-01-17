#include <Adafruit_MLX90614.h>

#include "shortJson.h"
#include "miscString.h"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
struct {
	bool status;
	float ambient_t;
	float object_t;
} mlxData {false, 0.0, 0.0};

// Store for strings which do not change
char mlx90614S1[] = "MLX90614";
char mlx90614S2[] = "T amb";
char mlx90614S3[] = "T obj";

void updateMLX90614string(char * stringBuf) {
	//"MLX90614":{"init":true,"T amb":19.17001,"T obj":18.20999}

	jsonStart(stringBuf, mlx90614S1); // MLX90614

	jsonInit(stringBuf, miscS1); // init
	if (mlxData.status == true) {
		// Output true
		strcat(stringBuf, miscS2); // true

		jsonComma(stringBuf); // ,

		jsonInit(stringBuf, mlx90614S2); // T amb
		dtostrf(mlxData.ambient_t, 4, 2, dummyString);  //4 is mininum width, 2 is precision
		strcat(stringBuf, dummyString);
		jsonComma(stringBuf); // ,

		jsonInit(stringBuf, mlx90614S3); // T obj
		dtostrf(mlxData.object_t, 4, 2, dummyString);  //4 is mininum width, 2 is precision
		strcat(stringBuf, dummyString);

	} else {
		// Output false
		strcat(stringBuf, miscS3); // false
	}

	jsonEnd(stringBuf); // right brace
}

/**
  mlx.begin() always returns true, hence we need to check the I2C adress
  */
bool isMLX90614Present() {
	Wire.beginTransmission(MLX90614_I2CADDR);
	byte error = Wire.endTransmission();

	return (error == 0);
}

void updateMLX() {
	if (mlxData.status || (mlxData.status = isMLX90614Present())) {
		mlx.begin();
		mlxData.ambient_t = mlx.readAmbientTempC();
		mlxData.object_t  = mlx.readObjectTempC();
	}
}

void serializeMLX(JsonDocument & doc) {

	JsonObject data = doc.createNestedObject("MLX90614");
	data["init"] = mlxData.status;

	if (mlxData.status) {
		data["T amb"] = mlxData.ambient_t;
		data["T obj"] = mlxData.object_t;
	}
}

void print_MLX(void)
{
	Serial.println(F("MLX90614"));
	if (mlxData.status == false) {
		Serial.println(F("No data"));
	} else {
		Serial.print(F("Ambient T = "));
		Serial.print(mlxData.ambient_t);
		Serial.println(F(" *C"));

		Serial.print(F("Object T = "));
		Serial.print(mlxData.object_t);
		Serial.println(F(" *C"));

		Serial.println();
	}
}
