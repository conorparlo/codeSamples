#include <Arduino.h>
#include <Keypad.h>
#include <TinyGPS++.h>
//#include <Wire.h>

#define DISPLAY_ADDRESS1 0x72 //This is the default address of the OpenLCD

bool gameState = false;
int gameLevel;
int level;
int position = 0;
int altitude;
int altitudeThreshold = 5200;
int speed;
int speedThreshold = 7;
//tone config
int freq = 2000;
int channel = 0;
int resolution = 8;
//End tone config
char* secretCode = "420";
char* mathAnswer = "9";
char* resetCode = "6824";
int displayText;
int lcdPosition = 0;
int clearCount = 0;
//bool clearLCD = false;

//Keypad Matrix
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {5, 15, 2, 0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 17, 16}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int RELAY_PIN = 18;

//Initialize GPS Serial

HardwareSerial gpsSerial(2);  // UART1/Serial1 pins 16,17
HardwareSerial OpenLCD(1);
TinyGPSPlus gps;


void setLCD(int displayText)
{
  if (displayText == 1) {
    OpenLCD.print("Adult Summer Camp");
    OpenLCD.write(254); //Send command character
    OpenLCD.write(128 + 64 + lcdPosition); //Change the position (128) of the cursor to 2nd row (64), position 8 (8)
    OpenLCD.print("Mystery Box v1");
  } else if (displayText == 2) {
    OpenLCD.print("Game on!");
  } else if (displayText == 3) {
    OpenLCD.print("Game over - You Won!");
  } else if (displayText == 4) {
    OpenLCD.print("Enter reset code:");
  } else {
    OpenLCD.print("setDisplay Error");
  }
  delay(100);
}

void clearLCD()
{
  OpenLCD.write('|'); //Put LCD into setting mode
  OpenLCD.write('-'); //Send clear display command
}

void setup()
{
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 27, 25);
  OpenLCD.begin(9600, SERIAL_8N1, 27, 25);
  clearLCD();
  setLCD(1);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(12, channel);
  startupTone();
  gameState = true;
  gameLevel = 1;
  delay(3000);
  clearLCD();
}

void loop() {
  if (gameState)
  {
    if (gameLevel == 1)
    {
      keypadLevel();
    }
    if (gameLevel == 2)
    {
      gpsLevel();
    }
    if (gameLevel == 3)
    {
      mathLevel();
    }
  }
  else
  {
    resetKey();
  }
}

void keypadLevel()
{
  if (clearCount == 0) {
    clearLCD();
    OpenLCD.print("I'm a date and a");
    OpenLCD.write(254); //Send command character
    OpenLCD.write(128 + 64 + 0); //Change the position (128) of the cursor to 2nd row (64), position 8 (8)
    OpenLCD.print("time that might make");
    OpenLCD.write(254); //Send command character
    OpenLCD.write(128 + 20 + 0); //Change the position (128) of the cursor to 2nd row (64), position 8 (8)
    OpenLCD.print("you hungry and tired");
    OpenLCD.write(254); //Send command character
    OpenLCD.write(128 + 84 + 0); //Change the position (128) of the cursor to 2nd row (64), position 8 (8)
    OpenLCD.print("___");
    clearCount++;
  }
  char key = keypad.getKey();
  if (key != 0)
  {
    buttonTone();
    if (key == secretCode[position])  // Valid key in Password sequence
    {
      OpenLCD.write(254); //Send command character
      OpenLCD.write(128 + 84 + lcdPosition); //Change the position (128) of the cursor to 2nd row (64), position 8 (8)
      OpenLCD.print(key);
      position ++;
      lcdPosition++;
    }
    else  // Invalid key - start all over again
    {
      position = 0;
      lcdPosition = 0;
      failLevel();
    }
  }
  if (position == 3)  // Password successfully entered - advance state
  {
    position = 0;
    lcdPosition = 0;
    advLevel();
  }
  delay(100);
}

void gpsLevel()
{
  if (clearCount == 0) {
    clearLCD();
    OpenLCD.print("I'm no normal box.  I like to go fast.");
    clearCount++;
  }
  while ((gpsSerial.available() > 0) && (gameState))
  {
    if (gps.encode(gpsSerial.read()))
    {
      if ((gps.altitude.isValid()) && (gps.altitude.isUpdated()))
      {
        //displayInfo();
        displaySpeed();
        //checkAltitude();
        speed = gps.speed.mph();
        if (speed > speedThreshold)
        {
          gpsSerial.end();
          advLevel();
        }
      }
    }
    // If 5000 milliseconds pass and there are no characters coming in
    // over the software serial port, show a "No GPS detected" error
    //    if (millis() > 5000 && gps.charsProcessed() < 10)
    //    {
    //      Serial.println(F("No GPS detected"));
    //      while(true);
    //    }
  }
}

void mathLevel()
{
  if (clearCount == 0) {
    clearLCD();
    OpenLCD.print("6/2(1+2)");
    clearCount++;
  }
  char key = keypad.getKey();
  if (key != 0)
  {
    buttonTone();
    if (key == mathAnswer[position])  // Valid key in Password sequence
    {
      OpenLCD.write(254); //Send command character
      OpenLCD.write(128 + 84 + lcdPosition); //Change the position (128) of the cursor to 2nd row (64), position 8 (8)
      OpenLCD.print(key);
      position ++;
      lcdPosition++;
    }
    else  // Invalid key - start all over again
    {
      position = 0;
      lcdPosition = 0;
      failLevel();
    }
  }
  if (position == 1)  // Password successfully entered - advance state
  {
    position = 0;
    lcdPosition = 0;
    advLevel();
  }
  delay(100);
}

void advLevel()
{
  gameLevel++;
  successTone();
  OpenLCD.write('|'); //Put LCD into setting mode
  OpenLCD.write('-'); //Send clear display command
  OpenLCD.println("Correct!");
  delay(3000);
  OpenLCD.write('|'); //Put LCD into setting mode
  OpenLCD.write('-'); //Send clear display command
  if (gameLevel > 3) {
    playingGame(false);
    gameLevel = 0;
    delay(3000);
  } else {
    OpenLCD.println("Going to next level");
    delay(2000);
  }
  clearCount = 0;
}

void failLevel()
{
  clearLCD();
  OpenLCD.print("Incorrect!");
  failTone();
  delay(600);
  clearCount = 0;
}

void resetKey()
{
  if (clearCount == 0) {
    clearLCD();
    setLCD(4);
    clearCount++;
  }
  char key = keypad.getKey();
  if (key != 0)
  {
    buttonTone();
    if (key == resetCode[position])  // Valid key in Password sequence
    {
      OpenLCD.write(254); //Send command character
      OpenLCD.write(128 + 84 + lcdPosition); //Change the position (128) of the cursor to 2nd row (64), position 8 (8)
      OpenLCD.print("*");
      position ++;
      lcdPosition++;
    }
    else  // Invalid key - start all over again
    {
      Serial.println("Invalid Code!");
      lcdPosition = 0;
      failLevel();
    }
  }
  if (position == 4)  // Password successfully entered - advance state
  {
    successTone();
    playingGame(true);
    position = 0;
    gameLevel = 1;
    clearCount = 0;
  }
  delay(100);
}

void startupTone()
{
  ledcWriteTone(channel, 200);
  delay(150);
  ledcWriteTone(channel, 0);
  delay(100);
  ledcWriteTone(channel, 200);
  delay(150);
  ledcWriteTone(channel, 0);
  delay(20);
  ledcWriteTone(channel, 1000);
  delay(500);
  ledcWriteTone(channel, 0);
}

void buttonTone()
{
  ledcWriteTone(channel, 1800);
  delay(100);
  ledcWriteTone(channel, 0);
}

void successTone()
{
  ledcWriteTone(channel, 294);
  delay(50);
  ledcWriteTone(channel, 0);
  delay(50);
  ledcWriteTone(channel, 594);
  delay(50);
  ledcWriteTone(channel, 0);
  delay(50);
  ledcWriteTone(channel, 394);
  delay(50);
  ledcWriteTone(channel, 0);
  delay(50);
  ledcWriteTone(channel, 794);
  delay(50);
  ledcWriteTone(channel, 0);
  delay(50);
  ledcWriteTone(channel, 694);
  delay(50);
  ledcWriteTone(channel, 294);
  delay(50);
  ledcWriteTone(channel, 0);
  delay(50);
  ledcWriteTone(channel, 594);
  delay(50);
  ledcWriteTone(channel, 0);
  delay(50);
  ledcWriteTone(channel, 394);
  delay(50);
  ledcWriteTone(channel, 0);
  delay(50);
  ledcWriteTone(channel, 794);
  delay(50);
  ledcWriteTone(channel, 0);
  delay(50);
  ledcWriteTone(channel, 694);
  delay(150);
  ledcWriteTone(channel, 0);
}

void failTone()
{
  ledcWriteTone(channel, 100);
  delay(100);
  ledcWriteTone(channel, 0);
  delay(100);
  ledcWriteTone(channel, 100);
  delay(100);
  ledcWriteTone(channel, 0);
}

void playingGame(bool state)
{
  if (state) {
    gameState = true;
    clearLCD();
    setLCD(1);
  }
  else {
    clearLCD();
    setLCD(3);
    delay(500);
    digitalWrite(RELAY_PIN, HIGH);
    delay(200);
    digitalWrite(RELAY_PIN, LOW);
    gameState = false;
  }
}

void checkAltitude()
{
  altitude = gps.altitude.feet();
  if (altitude > altitudeThreshold)
  {
    playingGame(false);
  }
}

void displayInfo()
{

  //LOCATION
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  //ALTITUDE
  Serial.print(F("  Altitude: "));
  if (gps.altitude.isValid())
  {
    Serial.print(gps.altitude.feet());
    Serial.print(F(" Ft "));
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  //DATE
  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  //TIMESTAMP
  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  //SATELLITES
  //  Serial.print(F("  Satellites Connected: "));
  //  if (gps.altitude.isValid())
  //  {
  //    Serial.print(gps.satellites.value());
  //  }
  //  else
  //  {
  //    Serial.print(F("INVALID"));
  //  }

  //SPEED

  Serial.print(F("  Speed: "));
  if (gps.speed.isValid())
  {
    Serial.print(gps.speed.mph());
    Serial.print(F(" MPH"));
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}


void displaySpeed()
{

  if (gps.speed.isValid())
  {
    OpenLCD.write(254); //Send command character
    OpenLCD.write(128 + 84 + 0); //Change the position (128) of the cursor to 2nd row (64), position 8 (8)
    OpenLCD.print("Speed: ");
    OpenLCD.write(254); //Send command character
    OpenLCD.write(128 + 84 + 10); //Change the position (128) of the cursor to 2nd row (64), position 8 (8)
    OpenLCD.print(gps.speed.mph());
    OpenLCD.write(254); //Send command character
    OpenLCD.write(128 + 84 + 16); //Change the position (128) of the cursor to 2nd row (64), position 8 (8)
    OpenLCD.print(" MPH");
  }
  else
  {
    Serial.print(F("INVALID"));
  }
}
