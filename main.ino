#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>

// LCD
const int RS = 9;
const int enable = 8;
const int d4 = 7;
const int potentiometer = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
int contrast = 90;
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
bool doneOne = false;
bool doneThree = false;
bool doneFive = false;

// EEPROM
int highscore = 0;
int presentScore = 0;
const byte byteMask = 0xFF;
const int byteLength = 8;

void setup() 
{
  pinMode(pinSW, INPUT_PULLUP);

  analogWrite(potentiometer, contrast);
  lcd.begin(16, 2);

//  writeEEPROM(0);
  
  // Greetings message
  lcd.setCursor(3, 0);
  lcd.print("Welcome to");
  lcd.setCursor(4, 1);
  lcd.print("Skystar!");
  delay(1500);
  displayMenu();
  
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 2); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  Serial.begin(9600);
}

void loop() 
{
  readJoystick();
  readClick(100);
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

int readClick(int mode)
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
        if (mode == 100)
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
        else
        {
          for (int col = 0; col < 6; col++)
          {
            lc.setLed(0, mode, col, false);
            if (mode == 1 and !doneOne)
            {
              doneOne = true;
              presentScore++;
            }
            if (mode == 3 and !doneThree)
            {
              doneThree = true;
              presentScore++;
            }
            if (mode == 5 and !doneFive)
            {
              doneFive = true;
              presentScore++;
            }
          }
        }
      }
    }
  }
  swStateLast = swState;
}

void startGame()
{
  bool shipState = false;
  bool playing = true;
  int shipLin = 4;
  int shipCol = 7;
  doneOne = false;
  doneThree = false;
  doneFive = false;
  presentScore = 0;

  lc.setLed(0, 1, 2, true);
  lc.setLed(0, 3, 3, true);
  lc.setLed(0, 5, 1, true);

  while (playing)
  {
    unsigned int elapsedTime = millis();
    if (elapsedTime - lastChanged > 500)
    {
      lc.setLed(0, shipLin, shipCol - 1, !shipState);
      lc.setLed(0, shipLin, shipCol, !shipState);
      lc.setLed(0, shipLin + 1, shipCol, !shipState);
      lc.setLed(0, shipLin - 1, shipCol, !shipState);
      shipState = !shipState;
      lastChanged = elapsedTime;
    }

    readClick(shipLin);

    yValue = analogRead(pinY);
    if (yValue < 250 && !joyMovedY)
    {
      lc.setLed(0, shipLin, shipCol - 1, 0);
      lc.setLed(0, shipLin, shipCol, 0);
      lc.setLed(0, shipLin + 1, shipCol, 0);
      lc.setLed(0, shipLin - 1, shipCol, 0);
      if (shipLin < 7)
      {
        shipLin++;
      }
      joyMovedY = true;
    }
    else if (yValue > 750 && !joyMovedY)
    {
      lc.setLed(0, shipLin, shipCol - 1, 0);
      lc.setLed(0, shipLin, shipCol, 0);
      lc.setLed(0, shipLin + 1, shipCol, 0);
      lc.setLed(0, shipLin - 1, shipCol, 0);
      if (shipLin > 0)
      {
        shipLin--;
      }
      joyMovedY = true;
    }
    if (250 <= yValue && yValue <= 750) 
    {
      joyMovedY = false;
    }

    if (doneOne == true and doneThree == true and doneFive == true)
    {
      if (presentScore > highscore)
      {
        highscore = presentScore;
        writeEEPROM(highscore);
      }
      lc.clearDisplay(0);
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Good job!");
      lcd.setCursor(1, 1);
      lcd.print("Level 1 Cleared");
      delay(1500);
      displayMenu();
      playing = false;
    }
  } 
}

void writeEEPROM(int score) 
{
  byte firstByte = (score >> byteLength) & byteMask;
  byte secondByte = score & byteMask;
  EEPROM.update(0, firstByte);
  EEPROM.update(1, secondByte);
}

int readEEPROM() 
{
  byte firstByte = EEPROM.read(0);
  byte secondByte = EEPROM.read(1);
  return (firstByte << byteLength) + secondByte;
}

void score()
{
  bool scoreBack = false;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Highest score:");
  lcd.print(readEEPROM());

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
