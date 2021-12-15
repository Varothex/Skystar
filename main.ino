#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>

const int button = 13;
bool buttonPressed = 0;

// name
int alphabetCounter = 0;
char alphabet[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
char name[4] = {'a', 'b' , 'c'};
int cursor = 0;
int cursorMode = 2001;

bool done = false;
bool nameBack = false;
bool settingsBack = false;
bool contrastBack = false;
bool scoreBack = false;
bool aboutBack = false;

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

// blink
int lastDebounceTimer = 0;
int debounceInterval = 50;
unsigned int lastChanged = 0;
bool startBlink = 0;
bool scoreBlink = 0;
bool settingsBlink = 0;
bool aboutBlink = 0;
bool nameBlink = 0;
bool brightnessBlink = 0;
bool contrastBlink = 0;
bool backBlink = 0;
bool doneBlink = 0;

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

int positionCounter;

// game
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
  pinMode(button, INPUT_PULLUP);

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
  readJoystickMenu();
  readClick(1000);
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

void readJoystickMenu()
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
        // main menu
        if (mode == 1000)
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
              settingsBack = false;
              displaySettings();
              settings();
            }
            else
            {
              about();
            }
          }
        }

        // score
        if (mode == 1999)
        {
          scoreBack = true;
          displayMenu();
        }

        // settings menu
        if (mode == 2000)
        {
          if (xIndex == 0)
          {
            if (yIndex == 0)
            {
              nameSettings();
            }
            else
            {
//              brightnessSettings();
            }
          }
          else
          {
            if (yIndex == 0)
            {
              contrastSettings();
            }
            else
            {
              backSettings();
            }
          }
        }
        
        // setting name
        if (mode == 2001)
        {
          name[0] = alphabet[alphabetCounter];
          cursor = 1;
          cursorMode = 2002;
        }
        if (mode == 2002)
        {
          name[1] = alphabet[alphabetCounter];
          cursor = 2;
          cursorMode = 2003;
        }
        if (mode == 2003)
        {
          name[2] = alphabet[alphabetCounter];
          cursorMode = 2004;
        }
        if (mode == 2005)
        {
          done = true;
          nameBack = true;
          displaySettings();
        }

        if (mode == 2006)
        {
          analogWrite(potentiometer, 90);
        }

        if (mode == 2007)
        {
          contrastBack = true;
          displaySettings();
        }

        if (100 <= mode and mode <= 999)
        {
          analogWrite(potentiometer, mode);
        } 

        // about
        if (mode == 3000)
        {
          positionCounter = 25;
          aboutBack = true;
          displayMenu();
        }

        // shoot
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
  scoreBack = false;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Highest score:");
  lcd.print(readEEPROM());

  while(scoreBack == false)
  {
    readClick(1999);
  }
}

void displaySettings()
{ 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Name");
  lcd.setCursor(6, 0);
  lcd.print("Brightness");
  lcd.setCursor(0, 1);
  lcd.print("Contrast");
  lcd.setCursor(10, 1);
  lcd.print("Back");
}

void readJoystickSettings()
{
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);

    if ((xValue < 250 or xValue > 750) && !joyMovedX)
    {
      if (xIndex == 0) 
      {
        xIndex = 1;
        displaySettings();
      }
      else 
      {
        xIndex = 0;
        displaySettings();
      }
      joyMovedX = true;
    }
    
    if ((yValue < 250 or yValue > 750) && !joyMovedY)
    {
      if (yIndex == 0) 
      {
        yIndex = 1;
        displaySettings();
      }
      else 
      {
        yIndex = 0;
        displaySettings();
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
          lcd.setCursor(0, 0);
          if (nameBlink == 0)
          {
            lcd.print("    ");
            nameBlink = 1;
          }
          else
          {
            lcd.print("Name");
            nameBlink = 0;
          }
          lastChanged = elapsedTime;
        }
      }
      else
      {
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 250)
        {
          lcd.setCursor(6, 0);
          if (brightnessBlink == 0)
          {
            lcd.print("          ");
            brightnessBlink = 1;
          }
          else
          {
            lcd.print("Brightness");
            brightnessBlink = 0;
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
          lcd.setCursor(0, 1);
          if (contrastBlink == 0)
          {
            lcd.print("        ");
            contrastBlink = 1;
          }
          else
          {
            lcd.print("Contrast");
            contrastBlink = 0;
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
          if (backBlink == 0)
          {
            lcd.print("    ");
            backBlink = 1;
          }
          else
          {
            lcd.print("Back");
            backBlink = 0;
          }
          lastChanged = elapsedTime;
        }
      }
    }
}

void settings()
{
  while (!settingsBack)
  {
    readJoystickSettings();
    readClick(2000);
  }
}

void contrastSettings()
{
  int contrastLevel = 800;
  int nameIndex = 0;
  contrastBack = false;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Change contrast:");
  lcd.setCursor(4, 1);
  lcd.print("Default");
  lcd.setCursor(12, 1);
  lcd.print("Back");

  while (!contrastBack)
  {
    yValue = analogRead(pinY);

    if (yValue < 250 && !joyMovedY)
    {
      nameIndex++;
      lcd.setCursor(4, 1);
      lcd.print("Default");
      lcd.setCursor(12, 1);
      lcd.print("Back");
      if (nameIndex > 2)
      {
        nameIndex = 0;
      }
      joyMovedY = true;
    }

    if (yValue > 750 && !joyMovedY)
    {
      nameIndex--;
      lcd.setCursor(4, 1);
      lcd.print("Default");
      lcd.setCursor(12, 1);
      lcd.print("Back");
      if (nameIndex < 0)
      {
        nameIndex = 2;
      }      
      joyMovedY = true;
    }

    if (250 <= yValue && yValue <= 750) 
    {
      joyMovedY = false;
    }

    if (nameIndex == 0)
    {
      xValue = analogRead(pinX);
      
      if (xValue < 250) 
      {
        contrastLevel++;
        if (contrastLevel > 999)
        {
          contrastLevel = 100;
        }
      }

      if (xValue > 750 and nameIndex == 0)
      {
        contrastLevel--;
        if (contrastLevel < 100)
        {
          contrastLevel = 999;
        }
      }
    
      lcd.setCursor(0, 1);
      lcd.print(contrastLevel);
      readClick(contrastLevel);
    }

    if (nameIndex == 1)
    {
      unsigned int elapsedTime = millis();
      if (elapsedTime - lastChanged > 250)
      {
        lcd.setCursor(4, 1);
        if (doneBlink == 0)
        {
          lcd.print("       ");
          doneBlink = 1;
        }
        else
        {
          lcd.print("Default");
          doneBlink = 0;
        } 
        lastChanged = elapsedTime;
      }
      readClick(2006);
    }

    if (nameIndex == 2)
    {
      unsigned int elapsedTime = millis();
      if (elapsedTime - lastChanged > 250)
      {
        lcd.setCursor(12, 1);
        if (doneBlink == 0)
        {
          lcd.print("    ");
          doneBlink = 1;
        }
        else
        {
          lcd.print("Back");
          doneBlink = 0;
        } 
        lastChanged = elapsedTime;
      }
      readClick(2007);
    }
  }
}

void nameSettings()
{
  nameBack = false;

  cursor = 0;
  cursorMode = 2001;
  done = false;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select a name:");
  lcd.setCursor(0, 1);
  lcd.print("___");
  lcd.setCursor(0, 1);
  
  while (!nameBack)
  {
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    swState = digitalRead(pinSW);
//    int buttonvalue = analogRead(button);
  
    if (xValue < 250)
    {
      alphabetCounter--;
      if (alphabetCounter < 0)
      {
        alphabetCounter = 25;
      }
    }

    if (xValue > 750)
    {
      alphabetCounter++;
      if (alphabetCounter > 25)
      {
        alphabetCounter = 0;
      }
    }

    lcd.print(alphabet[alphabetCounter]);
    
    readClick(cursorMode);
    lcd.setCursor(cursor, 1);

    if (cursorMode == 2004)
    {
      while (!done)
      {
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 250)
        {
          lcd.setCursor(7, 1);
          if (doneBlink == 0)
          {
            lcd.print("     ");
            doneBlink = 1;
          }
          else
          {
            lcd.print("done?");
            doneBlink = 0;
          } 
          lastChanged = elapsedTime;
        }
        readClick(2005);
      }
    }
  }
}

void backSettings()
{
  displayMenu();
  settingsBack = true;
}

void about()
{
  aboutBack = false;
  
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Skystar made by Mihai Vartic.");
  lcd.setCursor(1, 1);
  lcd.print("Github: Varothex/Skystar");

  while(aboutBack == false)
  {
    for (positionCounter = 0; positionCounter < 24; positionCounter++) 
    {
      lcd.scrollDisplayLeft();
      readClick(3000);
      delay(750);
    }
  }
    
  displayMenu();
}
