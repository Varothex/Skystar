#include <LiquidCrystal.h>
#include "LedControl.h"

// LCD
const int RS = 9;
const int enable = 8;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
int lastDebounceTimer = 0;
int debounceInterval = 50;
unsigned int lastChanged = 0;
bool startBlink = 0;
bool scoreBlink = 0;
bool settingsBlink = 0;
bool aboutBlink = 0;

// matrix + joystick
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int rows = 8;
const int cols = 8;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER
const int n = 8;
const int pinSW = 6; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output
bool swState = HIGH;
bool swStateLast = HIGH;
bool buttonState = HIGH;
int xValue = 0;
int yValue = 0;
bool joyMovedX = false;
bool joyMovedY = false;
int xIndex = 0;
int yIndex = 0;

void setup() 
{
  lcd.begin(16, 2);
  
  // Greetings message
  lcd.setCursor(3, 0);
  lcd.print("Welcome to");
  lcd.setCursor(4, 1);
  lcd.print("Skystar!");
  delay(1500);
  displayMenu();

  pinMode(pinSW, INPUT_PULLUP);
  
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 2); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  Serial.begin(9600);
}

void loop() 
{
  readJoystick();
  readClick();
}

void displayMenu()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Start");
    lcd.setCursor(10, 0);
    lcd.print("Score");
    lcd.setCursor(1, 1);
    lcd.print("Settings");
    lcd.setCursor(10, 1);
    lcd.print("About");
}

void readJoystick()
{
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);

    if ((xValue < 250 or xValue > 750) && !joyMovedX)
    {
      if (xIndex == 0) 
      {
        xIndex = 1;
        displayMenu();
      }
      else 
      {
        xIndex = 0;
        displayMenu();
      }
      joyMovedX = true;
    }
    
    if ((yValue < 250 or yValue > 750) && !joyMovedY)
    {
      if (yIndex == 0) 
      {
        yIndex = 1;
        displayMenu();
      }
      else 
      {
        yIndex = 0;
        displayMenu();
      }
      joyMovedY = true;
    }

    if (250 <= xValue && xValue <= 750) 
    {
      joyMovedX = false;
    } 
    
    if (250 <= yValue && yValue <= 750) 
    {
      joyMovedY = false;
    } 

    if (xIndex == 0)
    {
      if (yIndex == 0)
      {
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 250)
        {
          lcd.setCursor(1, 0);
          if (startBlink == 0)
          {
            lcd.print("     ");
            startBlink = 1;
          }
          else
          {
            lcd.print("Start");
            startBlink = 0;
          }
          lastChanged = elapsedTime;
        }
      }
      else
      {
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 250)
        {
          lcd.setCursor(10, 0);
          if (scoreBlink == 0)
          {
            lcd.print("     ");
            scoreBlink = 1;
          }
          else
          {
            lcd.print("Score");
            scoreBlink = 0;
          }
          lastChanged = elapsedTime;
        }
      }
     }
     else
     {
      if (yIndex == 0)
      {
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 250)
        {
          lcd.setCursor(1, 1);
          if (settingsBlink == 0)
          {
            lcd.print("        ");
            settingsBlink = 1;
          }
          else
          {
            lcd.print("Settings");
            settingsBlink = 0;
          }
          lastChanged = elapsedTime;
        }
      }
      else
      {
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 250)
        {
          lcd.setCursor(10, 1);
          if (aboutBlink == 0)
          {
            lcd.print("     ");
            aboutBlink = 1;
          }
          else
          {
            lcd.print("About");
            aboutBlink = 0;
          }
          lastChanged = elapsedTime;
        }
      }
    }
}

void readClick()
{
  swState = digitalRead(pinSW);

  if (swState != swStateLast)
  {
    lastDebounceTimer = millis();
  }
  
  if (millis() - lastDebounceTimer > debounceInterval)
  {
    if (swState != buttonState)
    {
      buttonState = swState;
      if (buttonState == LOW)
      {
        if (xIndex == 0)
        {
          if (yIndex == 0)
          {
            startGame();
          }
          else
          {
            score();
          }
        }
        else
        {
          if (yIndex == 0)
          {
            settings();
          }
          else
          {
            about();
          }
        }
      }
    }
  }
  swStateLast = swState;
}

void startGame()
{
  Serial.println("Game on!");
}

void score()
{
  bool scoreBack = false;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Highest score: 0");

  while(scoreBack == false)
  {
    swState = digitalRead(pinSW);
  
    if (swState != swStateLast)
    {
      lastDebounceTimer = millis();
    }
    if (millis() - lastDebounceTimer > debounceInterval)
    {
      if (swState != buttonState)
      {
        buttonState = swState;
        if (buttonState == LOW)
        {
          scoreBack = true;
          displayMenu();
        }
      }
    }
    swStateLast = swState;
  }
}

void settings()
{
  Serial.println("settings");
}

void about()
{
  bool aboutBack = false;
  
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Skystar made by Mihai Vartic.");
  lcd.setCursor(1, 1);
  lcd.print("Github: Varothex/Skystar");
    
  for (int positionCounter = 0; positionCounter < 24; positionCounter++) 
  {
    lcd.scrollDisplayLeft();
    delay(750);
  }
    
  displayMenu();
}
