/* 
 * HD44780 LCD
 * BMP180 Temp / Pressure
 * 
 * Arduino Mini 328P (Old Bootloader)
 * Steve Buer
 */

#include <LiquidCrystal.h>
#include <BMP180.h>
#include <BMP180DEFS.h>
#include <MetricSystem.h>

/* LCD parameters */

#define COLUMNS 16
#define ROWS 2
#define RS 2
#define EN 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
BMP180 bmp180;
//ImperialSystem imperial;

void setup() 
{
  Serial.begin(9600);
  lcd.begin(COLUMNS, ROWS);
  //bmp180.setMetricSystem(imperial);
  
  if (!bmp180.begin())
    lcd.print("BMP180 not found!");
  else
    lcd.print("BMP180");
}

void read_altitude()
{
  Serial.print(bmp180.readAltitude());
  Serial.print(" m");
}

void read_cmd()
{
  if (Serial.available() <= 0)
    return;
  
  char command = Serial.read();

  switch (command) {

    case 'a':
       read_altitude();
       break;

    case 'h':
    default:
      Serial.println("g = start graphing, a = read altitude, p = read pressure, t = read temp");
  }

  Serial.print("ok ");
}

void loop()
{
    lcd.setCursor(10,0);
    lcd.print(bmp180.readAltitude());
    lcd.setCursor(0,1);
    lcd.print(bmp180.readTemperature());
    lcd.print("  ");
    lcd.print(bmp180.readPressure());
    lcd.print(" ");

    read_cmd();
    
    delay(5000);
}


