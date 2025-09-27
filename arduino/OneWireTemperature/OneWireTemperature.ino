/*
 * Dallas 1-Wire
 * DS18B20 Temperature Sensor
 * Steve N7MKO
 */

#include <DS18B20.h>

#define DS_PIN 2

DS18B20 ds(DS_PIN);

void setup() 
{
  Serial.begin(9600);
  Serial.print("Found Devices: ");
  Serial.println(ds.getNumberOfDevices());
}

void loop() 
{
  int i;
  uint8_t address[8];
  ds.getAddress(address);

  Serial.print("Address:");
  
  for (i = 0; i < 8; i++) {
      Serial.print(" ");
      Serial.print(address[i], HEX);
  }
  
  Serial.println();

  Serial.print("Resolution: ");
  Serial.println(ds.getResolution());

  Serial.print("Power Mode: ");
  
  if (ds.getPowerMode())
      Serial.println("External");
  else
      Serial.println("Parasite");

  Serial.print("Temperature: ");
  Serial.print(ds.getTempC());
  Serial.print(" C / ");
  Serial.print(ds.getTempF());
  Serial.println(" F");
  Serial.println();

  delay(10000);

}
