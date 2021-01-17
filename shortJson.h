#ifndef SHORTJSON_H
#define SHORTJSON_H

//"Davis Anemometer":{"init":true,"direction":53,"avg speed":0,"min speed":0,"max speed":0,"rotations":0},
//"BME280":{"init":true,"Temp":19.03,"Pres":1014.456,"Hum":53.21289},
//"MLX90614":{"init":true,"T amb":19.41,"T obj":18.64999},
//"TSL2591":{"init":true,"Lux":2.223584,"Visible":782,"IR":178,"Gain":32,"Timing":1}

const static char jsonS1[] = "\"";
const static char jsonS2[] = ",";
const static char jsonS3[] = ":";
const static char jsonS4[] = "{";
const static char jsonS5[] = "}";

void jsonQuote(char *jsonString) {
	strcat(jsonString, jsonS1); // "\""
}

void jsonComma(char *jsonString) {
	strcat(jsonString, jsonS2); // ","
}

void jsonColon(char *jsonString) {
	strcat(jsonString, jsonS3); // ":"
}

void jsonLcurl(char *jsonString) {
	strcat(jsonString, jsonS4); // "{"
}

void jsonRcurl(char *jsonString) {
	strcat(jsonString, jsonS5); // "}"
}

void jsonInit(char *jsonString, char * name)
{
	jsonQuote(jsonString);
	strcat(jsonString, name);
	jsonQuote(jsonString);
	jsonColon(jsonString);
}

void jsonStart(char *jsonString, char * name)
{
	jsonString[0] = '\0'; // Set to null string
	jsonInit(jsonString, name); // "BME280"
	jsonLcurl(jsonString);
}

void jsonEnd(char *jsonString)
{
	jsonRcurl(jsonString); 
}
#endif // SHORTJSON_H
