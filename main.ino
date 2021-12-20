#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>
#include <WTV020SD16P.h>

//int resetPin = 13;
//int soundClockPin = 0;
//int dataPin = 1;
//int buzyPin = A2; // 12
//WTV020SD16P wtv020sd16p(resetPin,soundClockPin,dataPin,buzyPin);

//const int button = 13;
//bool buttonPressed = 0;

// name
int alphabetCounter = 0;
char alphabet[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
char name[4] = {'a', 'b' , 'c'};
int cursor = 0;
int cursorMode = 2001;

bool settingsBack = false;
bool nameBack = false;
bool done = false;
bool brightnessBack = false;
bool contrastBack = false;
bool scoreBack = false;
bool aboutBack = false;
bool matrixBrightnessBack = false;

// LCD
const int RS = 9;
const int enable = 8;
const int d4 = 7;
const int potentiometer = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
const int lcdBrightnessPin = 1;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
int contrast = 90;
bool lcdBrightness = HIGH; // ???


byte heart[8] = 
{
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

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
bool matrixBrightnessBlink = 0;
bool lcdBrightnessBlink = 0;
bool hpBlink = 0;

// matrix
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int rows = 8;
const int cols = 8;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER
int brightness = 2;

// joystick
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
int level = 3; // 1
int hp = 3;
bool nameSet = false;

bool lvl1ship1 = true;
bool lvl1ship2 = true;
bool lvl1ship3 = true;

bool lvl2ship1 = true;
bool lvl2ship2 = true;
int lvl2ship1poz;
int lvl2ship1col;
int lvl2ship2poz;
int lvl2ship2col;
unsigned int lastChangedship1 = 0;
unsigned int lastChangedship2 = 0;

bool lvl3ship1 = true;
bool lvl3ship2 = true;
bool lvl3ship3 = true;
bool lvl3ship4 = true;
int lvl3ship1poz;
int lvl3ship1col;
int lvl3ship2poz;
int lvl3ship2col;
int lvl3ship3poz;
int lvl3ship3col;
int lvl3ship4poz;
int lvl3ship4col;
unsigned int lastChangedship3 = 0;
unsigned int lastChangedship4 = 0;

// EEPROM
int highscore;
int presentScore = 0;
const byte byteMask = 0xFF;
const int byteLength = 8;

//ship (){}

void setup() 
{
  pinMode(pinSW, INPUT_PULLUP);
//  pinMode(button, INPUT_PULLUP);
  pinMode(lcdBrightnessPin, OUTPUT);
  
  digitalWrite(lcdBrightnessPin, lcdBrightness);
  analogWrite(potentiometer, contrast);
  lcd.begin(16, 2);
  
//  wtv020sd16p.reset();

  //  writeEEPROM(0);
  highscore = readEEPROM(); 
  
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, brightness);
  lc.clearDisplay(0);

  randomSeed(analogRead(0));
  
  // Greetings message
  lcd.setCursor(3, 0);
  lcd.print("Welcome to");
  lcd.setCursor(4, 1);
  lcd.print("Skystar!");
  delay(1500);
  displayMenu();
  

  Serial.begin(9600);
}

void loop() 
{
//  wtv020sd16p.playVoice(0);
  readJoystickMenu();
  readClick(1000);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void displayMenu()
{
    lc.clearDisplay(0);
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

void readClick(int mode)
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
              nameSet = false;
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
//              soundSettings();
            }
            else
            {
              brightnessSettings();
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
        
        // settings name
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
        }

        // settigs contrast
        if (100 <= mode and mode <= 250)
        {
          contrast = mode-100;
          lcd.setCursor(0, 1);
          if (10 < contrast and contrast < 100)
          {
            lcd.print("0");
          }
          else if (contrast < 10)
          {
            lcd.print("00");
          }
          lcd.print(contrast);
          analogWrite(potentiometer, contrast);
        } 

        if (mode == 2006)
        {
          contrast = 90;
          lcd.setCursor(0, 1);
          lcd.print("090");
          analogWrite(potentiometer, contrast);
        }

        // settings brightness
        if (mode == 2009)
        {
          if (xIndex == 0)
          {
            if (yIndex == 0)
            {
              matrixBrightnessSettings();
            }
            else
            {
              lcdBrightness = !lcdBrightness;
              digitalWrite(lcdBrightnessPin, lcdBrightness); 
            }
          }
          else
          {
            backBrightness();
            matrixSettings();
          }
        }
        
        if (10 <= mode and mode <= 25)
        {
          brightness = mode-10;
          lcd.setCursor(0, 1);
          if (brightness < 10)
          {
            lcd.print("0");
          }
          lcd.print(brightness);
          lc.setIntensity(0, brightness);
        } 

        if (mode == 2008)
        {
          brightness = 2;
          lcd.setCursor(0, 1);
          lcd.print("02");
          lc.setIntensity(0, brightness);
        }

        // brightness back
        if (mode == 5000)
        {
          matrixBrightnessBack = true;
          lc.clearDisplay(0);
          displayBrightness();
          matrixSettings();
        }

        // back
        if (mode == 2007)
        {
          contrastBack = true;
          lc.clearDisplay(0);
          displaySettings();
          matrixSettings();
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
          for (int col = 0; col <= 5; col++)
          {
            lc.setLed(0, mode, 5 - col, true);
            delay(10);
            lc.setLed(0, mode, 5 - col, false);
            
            if (level == 1)
            {
              if (mode == 1 and lvl1ship1)
              {
                lvl1ship1 = false;
                presentScore++;
                displayGameScreen();
              }
              if (mode == 3 and lvl1ship2)
              {
                lvl1ship2 = false;
                presentScore++;
                displayGameScreen();
              }
              if (mode == 5 and lvl1ship3)
              {
                lvl1ship3 = false;
                presentScore++;
                displayGameScreen();
              }
            }
            if (level == 2)
            {
              if (mode == lvl2ship1poz and lvl2ship1)
              {
                lvl2ship1 = false;
                presentScore += 2;
                displayGameScreen();
              }
              if (mode == lvl2ship2poz and lvl2ship2)
              {
                lvl2ship2 = false;
                presentScore += 2;
                displayGameScreen();
              }
            }
            if (level == 3)
            {
              if (mode == lvl3ship1poz and lvl3ship1)
              {
                lvl3ship1 = false;
                presentScore += 3;
                displayGameScreen();
              }
              if (mode == lvl3ship2poz and lvl3ship2)
              {
                lvl3ship2 = false;
                presentScore += 3;
                displayGameScreen();
              }
              if (mode == lvl3ship3poz and lvl3ship3)
              {
                lvl3ship3 = false;
                presentScore += 3;
                displayGameScreen();
              }
              if (mode == lvl3ship4poz and lvl3ship4)
              {
                lvl3ship4 = false;
                presentScore += 3;
                displayGameScreen();
              }
            }
          }
        }
      }
    }
  }
  swStateLast = swState;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void displayGameScreen()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score ");
  lcd.setCursor(7, 0);
  lcd.print(presentScore);
  
  lcd.setCursor(9, 0);
  lcd.print("Level ");
  lcd.setCursor(15, 0);
  lcd.print(level);

  lcd.setCursor(0, 1);
  lcd.print("Name ");
  lcd.setCursor(5, 1);
  lcd.print(name);
  
  lcd.setCursor(9, 1);
  lcd.print("HP ");  
  lcd.createChar(0, heart);
  if (hp == 3)
  {
    lcd.setCursor(12, 1);
    lcd.write((byte)0);
    lcd.setCursor(13, 1);
    lcd.write((byte)0);
    lcd.setCursor(14, 1);
    lcd.write((byte)0);
  }
  else if (hp == 2)
  {
    lcd.setCursor(12, 1);
    lcd.print("X");
    lcd.setCursor(13, 1);
    lcd.write((byte)0);
    lcd.setCursor(14, 1);
    lcd.write((byte)0);    
  }
  else if (hp == 1)
  {
    lcd.setCursor(12, 1);
    lcd.print("X");
    lcd.setCursor(13, 1);
    lcd.print("X");
    lcd.setCursor(14, 1);
    lcd.write((byte)0); 
     
//    unsigned int elapsedTime = millis();
//    if (elapsedTime - lastChanged > 250)
//    {
//      lcd.setCursor(14, 1);
//      if (hpBlink == 0)
//      {
//        lcd.print(" ");
//        hpBlink = 1;
//      }
//      else
//      {
//        lcd.write((byte)0);
//        hpBlink = 0;
//      }
//      lastChanged = elapsedTime;
//    } 
  }
}

void displayVictoryScreen()
{
  if (presentScore > highscore)
  {
    lc.clearDisplay(0);
    lcd.clear();  
    lcd.setCursor(1, 0);
    lcd.print("New highscore!");
    lcd.setCursor(7, 1);
    lcd.print(presentScore);
    highscore = presentScore;
    writeEEPROM(highscore);
    delay(1500);
  }
  lc.clearDisplay(0);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Good job!");
  lcd.setCursor(1, 1);
  lcd.print("Level ");
  lcd.print(level);
  lcd.print(" Cleared");
  delay(1500);
  displayMenu();
}

void displayDefeatScreen()
{
  lc.clearDisplay(0);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Oh, no!");
  lcd.setCursor(1, 1);
  lcd.print("Level ");
  lcd.print(level);
  lcd.print(" Failed!");
  delay(1500);
  displayMenu();  
}

void startGame()
{
  bool shipState = false;
  bool playing = true;
  int shipLin = 4;
  int shipCol = 7;
  int lin, col;
  hp = 3;

  presentScore = 0;

  if (!nameSet)
  {
    nameSettings();
    nameSet = true;
  }

  displayGameScreen();

  if (level == 1)
  {
    lvl1ship1 = true;
    lvl1ship2 = true;
    lvl1ship3 = true;

    lc.setLed(0, 1, 1, true);
    lc.setLed(0, 1, 2, true);

    lc.setLed(0, 3, 2, true);
    lc.setLed(0, 3, 3, true);

    lc.setLed(0, 5, 0, true);
    lc.setLed(0, 5, 1, true);
  }

  if (level == 2)
  {
    lvl2ship1 = true;
    lvl2ship2 = true;
    
    col = random(0, 2);
    lin = random(0, 5);

    lvl2ship1poz = lin;
    lvl2ship1col = col;

    lc.setLed(0, lvl2ship1poz, lvl2ship1col, true);
    lc.setLed(0, lvl2ship1poz, lvl2ship1col+1, true);

    col = random(2, 4);
    lin = random(5, 8);

    lvl2ship2poz = lin;
    lvl2ship2col = col;

    lc.setLed(0, lvl2ship2poz, lvl2ship2col, true);
    lc.setLed(0, lvl2ship2poz, lvl2ship2col+1, true);
  }

  if (level == 3)
  {
    lvl3ship1 = true;
    lvl3ship2 = true;
    lvl3ship3 = true;
    lvl3ship4 = true;
    
    lin = random(0, 8);

    lvl3ship1poz = lin;
    lvl3ship1col = 0;

    lc.setLed(0, lvl3ship1poz, lvl3ship1col, true);
    lc.setLed(0, lvl3ship1poz, lvl3ship1col+1, true);

    lin = random(0, 8);

    lvl3ship2poz = lin;
    lvl3ship2col = 1;

    lc.setLed(0, lvl3ship2poz, lvl3ship2col, true);
    lc.setLed(0, lvl3ship2poz, lvl3ship2col+1, true);

    lin = random(0, 8);

    lvl3ship3poz = lin;
    lvl3ship3col = 2;

    lc.setLed(0, lvl3ship3poz, lvl3ship3col, true);
    lc.setLed(0, lvl3ship3poz, lvl3ship3col+1, true);

    lin = random(0, 8);

    lvl3ship4poz = lin;
    lvl3ship4col = 3;

    lc.setLed(0, lvl3ship4poz, lvl3ship4col, true);
    lc.setLed(0, lvl3ship4poz, lvl3ship4col+1, true);
  }

  while (playing)
  {
    unsigned int elapsedTime = millis();
    if (elapsedTime - lastChanged > 250)
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
      if (shipLin > 0)
      {
        shipLin--;
      }
      joyMovedY = true;
    }
    else if (yValue > 750 && !joyMovedY)
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
    if (250 <= yValue && yValue <= 750) 
    {
      joyMovedY = false;
    }

    if (level == 1 and lvl1ship1 == false and lvl1ship2 == false and lvl1ship3 == false)
    {
      displayVictoryScreen();
      level = 2;
      playing = false;
      startGame();
    }
    
    if (level == 2) 
    {
      unsigned int elapsedTimeShip1 = millis();
      if (elapsedTimeShip1 - lastChangedship1 > 1000)
      {
        if (lvl2ship1)
        {
          lin = random(0, 2);
          if (lin == 0 and lvl2ship1poz > 0)
          {
            lc.setLed(0, lvl2ship1poz, lvl2ship1col, false);
            lc.setLed(0, lvl2ship1poz, lvl2ship1col+1, false);              
            lvl2ship1poz--;
            lc.setLed(0, lvl2ship1poz, lvl2ship1col, true);
            lc.setLed(0, lvl2ship1poz, lvl2ship1col+1, true); 
          }
          if (lin == 1 and lvl2ship1poz < 7)
          {
            lc.setLed(0, lvl2ship1poz, lvl2ship1col, false);
            lc.setLed(0, lvl2ship1poz, lvl2ship1col+1, false);
            lvl2ship1poz++;
            lc.setLed(0, lvl2ship1poz, lvl2ship1col, true);
            lc.setLed(0, lvl2ship1poz, lvl2ship1col+1, true);  
          }
          if (lvl2ship2)
          {
            lc.setLed(0, lvl2ship2poz, lvl2ship2col, true);
            lc.setLed(0, lvl2ship2poz, lvl2ship2col+1, true);            
          }
        }
        lastChangedship1 = elapsedTimeShip1;
      }

      unsigned int elapsedTimeShip2 = millis();
      if (elapsedTimeShip2 - lastChangedship2 > 500)
      {
        if (lvl2ship2)
        {
          lin = random(0, 2);
          if (lin == 0 and lvl2ship2poz > 0)
          {
            lc.setLed(0, lvl2ship2poz, lvl2ship2col, false);
            lc.setLed(0, lvl2ship2poz, lvl2ship2col+1, false);              
            lvl2ship2poz--;
            lc.setLed(0, lvl2ship2poz, lvl2ship2col, true);
            lc.setLed(0, lvl2ship2poz, lvl2ship2col+1, true); 
          }
          if (lin == 1 and lvl2ship2poz < 7)
          {
            lc.setLed(0, lvl2ship2poz, lvl2ship2col, false);
            lc.setLed(0, lvl2ship2poz, lvl2ship2col+1, false);
            lvl2ship2poz++;
            lc.setLed(0, lvl2ship2poz, lvl2ship2col, true);
            lc.setLed(0, lvl2ship2poz, lvl2ship2col+1, true);  
          }
          if (lvl2ship1)
          {
            lc.setLed(0, lvl2ship1poz, lvl2ship1col, true);
            lc.setLed(0, lvl2ship1poz, lvl2ship1col+1, true);            
          }
        }
        lastChangedship2 = elapsedTimeShip2;
      }
      
      if (lvl2ship1 == false and lvl2ship2 == false)
      {
        displayVictoryScreen();
        level = 3;
        playing = false;
        startGame();
      }
    }

    if (level == 3)
    {
      unsigned int elapsedTimeShip1 = millis();
      if (elapsedTimeShip1 - lastChangedship1 > 3500)
      {
        if (lvl3ship1)
        {
          bool damage = false;
          for (int col = lvl3ship1col+2; col <= 7; col++)
          {
            lc.setLed(0, lvl3ship1poz, col, true);
            delay(10);
            lc.setLed(0, lvl3ship1poz, col, false);
            if (shipLin == lvl3ship1poz and !damage)
            {
              if (hp == 0)
              {
                displayDefeatScreen();
                playing = false;
              }
              else
              {
                displayGameScreen();
                hp--;
                damage = true;
              }
            }
          }
          
          lin = random(0, 2);
          if (lin == 0 and lvl3ship1poz > 0)
          {
            lc.setLed(0, lvl3ship1poz, lvl3ship1col, false);
            lc.setLed(0, lvl3ship1poz, lvl3ship1col+1, false);              
            lvl3ship1poz--;
            lc.setLed(0, lvl3ship1poz, lvl3ship1col, true);
            lc.setLed(0, lvl3ship1poz, lvl3ship1col+1, true); 
          }
          if (lin == 1 and lvl3ship1poz < 7)
          {
            lc.setLed(0, lvl3ship1poz, lvl3ship1col, false);
            lc.setLed(0, lvl3ship1poz, lvl3ship1col+1, false);
            lvl3ship1poz++;
            lc.setLed(0, lvl3ship1poz, lvl3ship1col, true);
            lc.setLed(0, lvl3ship1poz, lvl3ship1col+1, true);  
          }
          if (lvl3ship2)
          {
            lc.setLed(0, lvl3ship2poz, lvl3ship2col, true);
            lc.setLed(0, lvl3ship2poz, lvl3ship2col+1, true);            
          }
          if (lvl3ship3)
          {
            lc.setLed(0, lvl3ship3poz, lvl3ship3col, true);
            lc.setLed(0, lvl3ship3poz, lvl3ship3col+1, true);            
          }
          if (lvl3ship4)
          {
            lc.setLed(0, lvl3ship4poz, lvl3ship4col, true);
            lc.setLed(0, lvl3ship4poz, lvl3ship4col+1, true);            
          }
        }
        lastChangedship1 = elapsedTimeShip1;
      }

      unsigned int elapsedTimeShip2 = millis();
      if (elapsedTimeShip2 - lastChangedship2 > 2500)
      {
        if (lvl3ship2)
        {
          bool damage = false;
          for (int col = lvl3ship2col+2; col <= 7; col++)
          {
            lc.setLed(0, lvl3ship2poz, col, true);
            delay(10);
            lc.setLed(0, lvl3ship2poz, col, false);
            if (shipLin == lvl3ship2poz and !damage)
            {
              if (hp == 0)
              {
                displayDefeatScreen();
                playing = false;
              }
              else
              {
                displayGameScreen();
                hp--;
                damage = true;
              }
            }
          }
          
          lin = random(0, 2);
          if (lin == 0 and lvl3ship2poz > 0)
          {
            lc.setLed(0, lvl3ship2poz, lvl3ship2col, false);
            lc.setLed(0, lvl3ship2poz, lvl3ship2col+1, false);              
            lvl3ship2poz--;
            lc.setLed(0, lvl3ship2poz, lvl3ship2col, true);
            lc.setLed(0, lvl3ship2poz, lvl3ship2col+1, true); 
          }
          if (lin == 1 and lvl3ship2poz < 7)
          {
            lc.setLed(0, lvl3ship2poz, lvl3ship2col, false);
            lc.setLed(0, lvl3ship2poz, lvl3ship2col+1, false);
            lvl3ship2poz++;
            lc.setLed(0, lvl3ship2poz, lvl3ship2col, true);
            lc.setLed(0, lvl3ship2poz, lvl3ship2col+1, true);  
          }
          if (lvl3ship1)
          {
            lc.setLed(0, lvl3ship1poz, lvl3ship1col, true);
            lc.setLed(0, lvl3ship1poz, lvl3ship1col+1, true);            
          }
          if (lvl3ship3)
          {
            lc.setLed(0, lvl3ship3poz, lvl3ship3col, true);
            lc.setLed(0, lvl3ship3poz, lvl3ship3col+1, true);            
          }
          if (lvl3ship4)
          {
            lc.setLed(0, lvl3ship4poz, lvl3ship4col, true);
            lc.setLed(0, lvl3ship4poz, lvl3ship4col+1, true);            
          }
        }
        lastChangedship2 = elapsedTimeShip2;
      }

      unsigned int elapsedTimeShip3 = millis();
      if (elapsedTimeShip3 - lastChangedship3 > 2000)
      {
        if (lvl3ship3)
        {
//          bool damage = false;
//          for (int col = lvl3ship1col+2; col <= 7; col++)
//          {
//            lc.setLed(0, lvl3ship1poz, col, true);
//            delay(10);
//            lc.setLed(0, lvl3ship1poz, col, false);
//            if (shipLin == lvl3ship1poz and !damage)
//            {
//              if (hp == 0)
//              {
//                displayDefeatScreen();
//                playing = false;
//              }
//              else
//              {
//                displayGameScreen();
//                hp--;
//                damage = true;
//              }
//            }
//          }
          
          lin = random(0, 2);
          if (lin == 0 and lvl3ship3poz > 0)
          {
            lc.setLed(0, lvl3ship3poz, lvl3ship3col, false);
            lc.setLed(0, lvl3ship3poz, lvl3ship3col+1, false);              
            lvl3ship3poz--;
            lc.setLed(0, lvl3ship3poz, lvl3ship3col, true);
            lc.setLed(0, lvl3ship3poz, lvl3ship3col+1, true); 
          }
          if (lin == 1 and lvl3ship3poz < 7)
          {
            lc.setLed(0, lvl3ship3poz, lvl3ship3col, false);
            lc.setLed(0, lvl3ship3poz, lvl3ship3col+1, false);
            lvl3ship3poz++;
            lc.setLed(0, lvl3ship3poz, lvl3ship3col, true);
            lc.setLed(0, lvl3ship3poz, lvl3ship3col+1, true);  
          }
          if (lvl3ship1)
          {
            lc.setLed(0, lvl3ship1poz, lvl3ship1col, true);
            lc.setLed(0, lvl3ship1poz, lvl3ship1col+1, true);            
          }
          if (lvl3ship2)
          {
            lc.setLed(0, lvl3ship2poz, lvl3ship2col, true);
            lc.setLed(0, lvl3ship2poz, lvl3ship2col+1, true);            
          }
          if (lvl3ship4)
          {
            lc.setLed(0, lvl3ship4poz, lvl3ship4col, true);
            lc.setLed(0, lvl3ship4poz, lvl3ship4col+1, true);            
          }
        }
        lastChangedship3 = elapsedTimeShip3;
      }

      unsigned int elapsedTimeShip4 = millis();
      if (elapsedTimeShip4 - lastChangedship4 > 1000)
      {
        if (lvl3ship4)
        {
//          bool damage = false;
//          for (int col = lvl3ship1col+2; col <= 7; col++)
//          {
//            lc.setLed(0, lvl3ship1poz, col, true);
//            delay(10);
//            lc.setLed(0, lvl3ship1poz, col, false);
//            if (shipLin == lvl3ship1poz and !damage)
//            {
//              if (hp == 0)
//              {
//                displayDefeatScreen();
//                playing = false;
//              }
//              else
//              {
//                displayGameScreen();
//                hp--;
//                damage = true;
//              }
//            }
//          }
          
          lin = random(0, 2);
          if (lin == 0 and lvl3ship4poz > 0)
          {
            lc.setLed(0, lvl3ship4poz, lvl3ship4col, false);
            lc.setLed(0, lvl3ship4poz, lvl3ship4col+1, false);              
            lvl3ship4poz--;
            lc.setLed(0, lvl3ship4poz, lvl3ship4col, true);
            lc.setLed(0, lvl3ship4poz, lvl3ship4col+1, true); 
          }
          if (lin == 1 and lvl3ship4poz < 7)
          {
            lc.setLed(0, lvl3ship4poz, lvl3ship4col, false);
            lc.setLed(0, lvl3ship4poz, lvl3ship4col+1, false);
            lvl3ship4poz++;
            lc.setLed(0, lvl3ship4poz, lvl3ship4col, true);
            lc.setLed(0, lvl3ship4poz, lvl3ship4col+1, true);  
          }
          if (lvl3ship1)
          {
            lc.setLed(0, lvl3ship1poz, lvl3ship1col, true);
            lc.setLed(0, lvl3ship1poz, lvl3ship1col+1, true);            
          }
          if (lvl3ship2)
          {
            lc.setLed(0, lvl3ship2poz, lvl3ship2col, true);
            lc.setLed(0, lvl3ship2poz, lvl3ship2col+1, true);            
          }
          if (lvl3ship3)
          {
            lc.setLed(0, lvl3ship3poz, lvl3ship3col, true);
            lc.setLed(0, lvl3ship3poz, lvl3ship3col+1, true);            
          }
        }
        lastChangedship4 = elapsedTimeShip4;
      }

      if (lvl3ship1 == false and lvl3ship2 == false and lvl3ship3 == false and lvl3ship4 == false)
      {
        displayVictoryScreen();
//        level = 4;
        playing = false;
//        startGame();
      }
    }
  } 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void matrixScore()
{
  int i;

  lc.clearDisplay(0);
  
  for (i = 1; i <= 6; i++)
  {
    lc.setLed(0, i, 1, true);
    lc.setLed(0, i, 4, true);
  }

  for (int i = 2; i <= 5; i++)
  {
    lc.setLed(0, i, 2, true);
    lc.setLed(0, i, 3, true);
    lc.setLed(0, i, 5, true);
    lc.setLed(0, i, 7, true);
  }
  
  lc.setLed(0, 0, 2, true);
  lc.setLed(0, 0, 3, true);
  lc.setLed(0, 7, 2, true);
  lc.setLed(0, 7, 3, true);
  lc.setLed(0, 3, 6, true);
  lc.setLed(0, 4, 6, true);
}

void score()
{
  matrixScore();
  
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void matrixSettings()
{
  lc.clearDisplay(0);
  
  lc.setLed(0, 0, 2, true);
  lc.setLed(0, 0, 3, true);
  lc.setLed(0, 1, 3, true);
  lc.setLed(0, 1, 4, true);
  lc.setLed(0, 2, 0, true);
  lc.setLed(0, 2, 4, true);
  lc.setLed(0, 3, 0, true);
  lc.setLed(0, 3, 1, true);
  lc.setLed(0, 3, 4, true);
  lc.setLed(0, 4, 1, true);
  lc.setLed(0, 4, 2, true);
  lc.setLed(0, 4, 3, true);
  lc.setLed(0, 4, 4, true);
  lc.setLed(0, 4, 5, true);
  lc.setLed(0, 5, 4, true);
  lc.setLed(0, 5, 5, true);
  lc.setLed(0, 5, 6, true);
  lc.setLed(0, 6, 5, true);
  lc.setLed(0, 6, 6, true);
  lc.setLed(0, 6, 7, true);
  lc.setLed(0, 7, 6, true);
  lc.setLed(0, 7, 7, true);
}

void settings()
{
  matrixSettings();
  while (!settingsBack)
  {
    readJoystickSettings();
    readClick(2000);
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
      unsigned int elapsedTime = millis();
      if (elapsedTime - lastChanged > 300)
      {
        alphabetCounter--;
        lastChanged = elapsedTime;
      }
      if (alphabetCounter < 0)
      {
        alphabetCounter = 25;
      }
    }

    if (xValue > 750)
    {
      unsigned int elapsedTime = millis();
      if (elapsedTime - lastChanged > 300)
      {
        alphabetCounter++;
        lastChanged = elapsedTime;
      }
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
            lcd.print("      ");
            doneBlink = 1;
          }
          else
          {
            lcd.print("Start!");
            doneBlink = 0;
          } 
          lastChanged = elapsedTime;
        }
        readClick(2005);
      }
    }
  }
}

void displayBrightness()
{
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Matrix");
  lcd.setCursor(11, 0);
  lcd.print("LCD");
  lcd.setCursor(6, 1);
  lcd.print("Back");
}

void readJoystickBrightness()
{
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);

    if ((xValue < 250 or xValue > 750) && !joyMovedX)
    {
      if (xIndex == 0) 
      {
        xIndex = 1;
        displayBrightness();
      }
      else 
      {
        xIndex = 0;
        displayBrightness();
      }
      joyMovedX = true;
    }
    
    if ((yValue < 250 or yValue > 750) && !joyMovedY)
    {
      if (yIndex == 0) 
      {
        yIndex = 1;
        displayBrightness();
      }
      else 
      {
        yIndex = 0;
        displayBrightness();
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
          lcd.setCursor(2, 0);
          if (matrixBrightnessBlink == 0)
          {
            lcd.print("      ");
            matrixBrightnessBlink = 1;
          }
          else
          {
            lcd.print("Matrix");
            matrixBrightnessBlink = 0;
          }
          lastChanged = elapsedTime;
        }
      }
      else
      {
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 250)
        {
          lcd.setCursor(11, 0);
          if (lcdBrightnessBlink == 0)
          {
            lcd.print("   ");
            lcdBrightnessBlink = 1;
          }
          else
          {
            lcd.print("LCD");
            lcdBrightnessBlink = 0;
          }
          lastChanged = elapsedTime;
        }
      }
     }
     else
     {
      unsigned int elapsedTime = millis();
      if (elapsedTime - lastChanged > 250)
      {
        lcd.setCursor(6, 1);
        if (contrastBlink == 0)
        {
          lcd.print("    ");
          contrastBlink = 1;
        }
        else
        {
          lcd.print("Back");
          contrastBlink = 0;
        }
        lastChanged = elapsedTime;
      }
     }
}

void brightnessSettings()
{
  brightnessBack = false;
  displayBrightness();
  
  while (!brightnessBack)
  {
    readJoystickBrightness();
    readClick(2009);
  }
}

void matrixBrightnessSettings()
{    
  for (int lin = 0; lin <= 15; lin++)
  {
    for (int col = 0; col <= 15; col++)
    {
      lc.setLed(0, lin, col, true);
    }
  }
  
  int brightnessLevel = brightness;
  int nameIndex = 0;
  
  matrixBrightnessBack = false;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Change brightness:");
  lcd.setCursor(4, 1);
  lcd.print("Default");
  lcd.setCursor(12, 1);
  lcd.print("Back");

  while (!matrixBrightnessBack)
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
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 400)
        {
          brightnessLevel++;
          lastChanged = elapsedTime;
        }
        if (brightnessLevel > 15)
        {
          brightnessLevel = 0;
        }
      }

      if (xValue > 750 and nameIndex == 0)
      {
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 400)
        {
          brightnessLevel--;
          lastChanged = elapsedTime;
        }
        if (brightnessLevel < 0)
        {
          brightnessLevel = 15;
        }
      }
    
      lcd.setCursor(0, 1);
      if (brightnessLevel < 10)
      {
        lcd.print("0");
      }
      lcd.print(brightnessLevel);
      readClick(brightnessLevel + 10);
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
      readClick(2008);
      brightnessLevel = brightness;
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
      readClick(5000);
    }
  }
}

void contrastSettings()
{
  int contrastLevel = contrast;
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
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 100)
        {
          contrastLevel++;
          lastChanged = elapsedTime;
        }
        if (contrastLevel > 150)
        {
          contrastLevel = 0;
        }
      }

      if (xValue > 750 and nameIndex == 0)
      {
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 100)
        {
          contrastLevel--;
          lastChanged = elapsedTime;
        }
        if (contrastLevel < 0)
        {
          contrastLevel = 150;
        }
      }
    
      lcd.setCursor(0, 1);
      if (10 < contrastLevel and contrastLevel < 100)
      {
        lcd.print("0");
      }
      else if (contrastLevel < 10)
      {
        lcd.print("00");
      }
      lcd.print(contrastLevel);
      readClick(contrastLevel + 100);
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
      contrastLevel = contrast;
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

void backBrightness()
{
  displaySettings();
  lc.clearDisplay(0);
  brightnessBack = true;
}

void backSettings()
{
  displayMenu();
  lc.clearDisplay(0);
  settingsBack = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void matrixAbout()
{
  lc.clearDisplay(0);

  lc.setLed(0, 3, 2, true);
  lc.setLed(0, 1, 2, true);
  lc.setLed(0, 1, 5, true);
  lc.setLed(0, 2, 2, true);
  lc.setLed(0, 2, 6, true);
  lc.setLed(0, 3, 6, true);
  lc.setLed(0, 3, 7, true);
  lc.setLed(0, 4, 6, true);
  lc.setLed(0, 4, 7, true);
  lc.setLed(0, 5, 1, true);
  lc.setLed(0, 5, 2, true);
  lc.setLed(0, 5, 1, true);
  lc.setLed(0, 5, 6, true);
  lc.setLed(0, 6, 1, true);
  lc.setLed(0, 6, 2, true);
  lc.setLed(0, 6, 5, true);

//  lc.setLed(0, 1, 1, true);
//  lc.setLed(0, 2, 1, true);
}

void about()
{

  bool emojiBlink = 0;
  aboutBack = false;

  matrixAbout();
  
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Skystar made by Mihai Vartic.");
  lcd.setCursor(1, 1);
  lcd.print("Github: Varothex/Skystar");

  while(aboutBack == false)
  {
    unsigned int elapsedTime = millis();
    if (elapsedTime - lastChanged > 700)
    {
      lcd.scrollDisplayLeft();
      
      if (emojiBlink == 0)
      {
        lc.setLed(0, 1, 1, true);
        lc.setLed(0, 2, 1, true);
        lc.setLed(0, 3, 2, false);
        emojiBlink = 1;
      }
      else
      {
        lc.setLed(0, 1, 1, false);
        lc.setLed(0, 2, 1, false);
        lc.setLed(0, 3, 2, true);
        emojiBlink = 0;
      } 
      
      lastChanged = elapsedTime;
    }

    readClick(3000);
  }

  lc.clearDisplay(0);
  displayMenu();
}
