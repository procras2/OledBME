#include <math.h>

struct {
  volatile unsigned long Rotations;         // cup rotation counter used in interrupt routine
  volatile unsigned long ContactBounceTime; // Timer to avoid contact bounce in interrupt routine
  float WindSpeed;                          // speed miles per hour
} windData;

// This is the function that the interrupt calls to increment the rotation count
void isr_rotation ()   {
  if ((millis() - windData.ContactBounceTime) > 15 ) {  // debounce the switch contact.
    windData.Rotations++;
    windData.ContactBounceTime = millis();
  }
}

void davisWindSetup() {
  pinMode(ANEMOMETER_WINDSPEEDPIN, INPUT);
  //attachInterrupt(digitalPinToInterrupt(DAVIS_WIND_DIGITAL_PIN), isr_rotation, FALLING);
}

void updateWindspeed() {
  windData.Rotations = 0;   //Set NbTops to 0 ready for calculations
  attachInterrupt(digitalPinToInterrupt(ANEMOMETER_WINDSPEEDPIN), isr_rotation, FALLING);
  //sei();         //Enables interrupts
  delay (3000);  //Wait 3 seconds to average
  //cli();         //Disable interrupts
  detachInterrupt(digitalPinToInterrupt(ANEMOMETER_WINDSPEEDPIN));

  // convert to mp/h using the formula V=P(2.25/T)
  // V = P(2.25/3) = P * 0.75

  windData.WindSpeed = windData.Rotations * 0.75;
}

void printWindspeed() {
  Serial.println("DAVIS WIND");
  Serial.print(windData.Rotations);   Serial.print("\t\t");
  Serial.println(windData.WindSpeed);
  Serial.println();
}
