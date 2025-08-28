/* 
 * HD44780 LCD
 * DS1307 Real Time Clock
 * AT24C32 EEPROM
 * 
 * Arduino Mini 328P (Old Bootloader)
 * Steve Buer
 */

#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <at24c32.h>

/* LCD dimensions */

#define COLUMNS 16
#define ROWS 2

/* LCD pins */

#define RS 2
#define EN 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

AT24C32 eprom(AT24C_ADDRESS_0);

/* helper functions -- todo: move to separate file */

void set_rtc();
bool getTime(const char *);
bool getDate(const char *);

/* custom chars */

byte smiley[8] = {
  B00000,
  B01010,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000 
};

byte yautja_A[8] = {
  
B10001,
B10010,
B10100,
B00000,
B00000,
B00100,
B00100,
B00100};

void setup() 
{
  /* serial setup */
  Serial.begin(9600);

  /* LED pin setup*/
  pinMode(LED_BUILTIN, OUTPUT);

  /* RTC init */
  // set_rtc(); /* this sets RTC with time of compilation */

  /*LCD display setup */
  lcd.begin(COLUMNS, ROWS);
  // lcd.print("Pr3dat0r Time");
  lcd.createChar(0, yautja_A);
  lcd.setCursor(0,0); /* switch from cgram to ddram */
  lcd.write(byte(0));
  //lcd.setCursor(0,1);
  //lcd.print("01:59:22");
  read_time();

  /* EEPROM */
  uint8_t byte = eprom.read(0);

  Serial.print("eeprom: ");
  Serial.println(byte);
}

/* blink or dim LED */
void led_control()
{
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(500);  
  digitalWrite(LED_BUILTIN, LOW);
  delay(5000);
}

/* process commands from serial input */
void read_cmd()
{
  if (Serial.available() <= 0)
    return;
    
  char command = 0;
  uint8_t eByte = 0;
  uint8_t buf[8] = { 0xD, 0xE, 0xA, 0xD, 0xB, 0xE, 0xE, 0xF };
  
  command = Serial.read();

  // Serial.print("command ");
  // Serial.println(command);

  // Serial.println("enter switch");

  switch (command) {

     // Serial.print("switch: ");
     // Serial.println(command);

    case 'e':
       Serial.print("eeprom read: ");
       eByte = eprom.read(0);
       Serial.println(eByte);
       break;

    case 't':
      read_time();
      break;

    case 'w':
      
      Serial.println("eeprom store: 0xdeadbeef");
      eprom.writeBuffer(0, buf, 8);
      Serial.println("store complete");
      break;

    case 'h':
    default:
      Serial.println("e = read eeprom, s = set time, t = print time, w = write eeprom");
  }

  // Serial.println("exit switch");

  Serial.print("ok ");
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

char time_string[64];

void read_time()
{
    tmElements_t tm;

  if (RTC.read(tm)) {

    sprintf(time_string, "%02d:%02d:%02d", tm.Hour, tm.Minute, tm.Second);

    lcd.setCursor(0,1);
    lcd.print(time_string);
    
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
}

void loop()
{
  // led_control();
  read_cmd();
}

tmElements_t tm;

void set_rtc()
{
  if (getDate(__DATE__) && getTime(__TIME__)) {
    if (RTC.write(tm)) {
      Serial.println("RTC set");
    }
  }
}

bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}
