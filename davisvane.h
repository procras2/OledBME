#define Offset 0;

struct {
  int VaneValue;
  int Direction;
  int CalDirection;
  int LastValue;
} vaneData;

void getHeading(int direction) {
  if (direction < 22)
    Serial.println(F("N"));
  else if (direction < 67)
    Serial.println(F("NE"));
  else if (direction < 112)
    Serial.println(F("E"));
  else if (direction < 157)
    Serial.println(F("SE"));
  else if (direction < 212)
    Serial.println(F("S"));
  else if (direction < 247)
    Serial.println(F("SW"));
  else if (direction < 292)
    Serial.println(F("W"));
  else if (direction < 337)
    Serial.println(F("NW"));
  else
    Serial.println(F("N"));
}

void updateVane() {
  vaneData.VaneValue = analogRead(DAVIS_VANE_ANALOG_PIN);
  vaneData.Direction = map(vaneData.VaneValue, 0, 1023, 0, 360);
  vaneData.CalDirection = vaneData.Direction + Offset;

  if (vaneData.CalDirection > 360)
    vaneData.CalDirection = vaneData.CalDirection - 360;

  if (vaneData.CalDirection < 0)
    vaneData.CalDirection = vaneData.CalDirection + 360;

  vaneData.LastValue = vaneData.CalDirection;
}

void printVane() {
  Serial.println(F("DAVIS VANE"));
  Serial.print(vaneData.VaneValue); Serial.print("\t\t");
  Serial.print(vaneData.CalDirection); Serial.print("\t\t");
  getHeading(vaneData.CalDirection);
  Serial.println();
}
