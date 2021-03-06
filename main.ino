#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>
#include <WTV020SD16P.h>

// buzzer
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

int tempo = 180;
const int buzzer = 9;
int melody[] = 
{
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, 
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4, 
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2, 
};
int melodyBoss[] = 
{
  NOTE_A4,4, NOTE_A4,4, NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,//4
  NOTE_E5,4, NOTE_E5,4, NOTE_E5,4, NOTE_F5,-8, NOTE_C5,16, NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,
};
int notes = sizeof(melodyBoss) / sizeof(melodyBoss[0]) / 2;
int notesBoss = sizeof(melodyBoss) / sizeof(melodyBoss[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int wholenoteBoss = (60000 * 4) / 120;
int divider = 0, noteDuration = 0;

// name
int alphabetCounter = 0;
char alphabet[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
char name[4] = {'a', 'b' , 'c'};
int cursor = 0;

// back
bool settingsBack = false;
bool nameBack = false;
bool done = false;
bool brightnessBack = false;
bool contrastBack = false;
bool scoreBack = false;
bool aboutBack = false;
bool matrixBrightnessBack = false;
bool lcdBrightnessBack = false;

// LCD
const int RS = 13;
const int enable = 8;
const int d4 = 7;
const int potentiometer = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
const int lcdBrightnessPin = 6;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
int contrast;
int lcdBrightness;

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
const int minLin = 0;
const int maxLin = 7;
const int maxCol = 7;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
int brightness;

// joystick
const int lowerValue = 250; 
const int higherValue = 750;
const int n = 8;
const int pinSW = 0;
const int pinX = A0;
const int pinY = A1;
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

// modes
const int gameMode = 1000;
int nameCursorMode = 1001;
const int nameFirstLetterMode = 1001;
const int nameSecondLetterMode = 1002;
const int nameThirdLetterMode = 1003;
const int nameWaitMode = 2004;
const int nameStartMode = 1005;

const int scoreMode = 2000;

const int settingsMenuMode = 3000;
const int brightnessMenuMode = 3001;
const int backToSettingsMode = 3002;
int setMatrixBrightnessMode;
const int defaultMatrixBrightnessMode = 3003;
int setLcdBrightnessMode;
const int defaultLcdBrightnessMode = 3004;
const int backToBrightnessMode = 3005;
int setContrastMode;
const int defaultContrastMode = 3006;


const int aboutMode = 4000;

// game
int level;
int hp = 3;
int bossHp;
bool nameSet = false;
unsigned long long int gameTime;

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
int lvl3ship1poz;
int lvl3ship1col;
int lvl3ship2poz;
int lvl3ship2col;
int lvl3ship3poz;
int lvl3ship3col;
unsigned int lastChangedship3 = 0;

bool lvl4ship1 = true;
bool lvl4ship2 = true;
bool lvl4ship3 = true;
bool lvl4ship4 = true;
int lvl4ship1poz;
int lvl4ship1col;
int lvl4ship2poz;
int lvl4ship2col;
int lvl4ship3poz;
int lvl4ship3col;
int lvl4ship4poz;
int lvl4ship4col;
unsigned int lastChangedship4 = 0;

// EEPROM
int highscore1;
int highscore2;
int highscore3;
int presentScore = 0;
const byte byteMask = 0xFF;
const int byteLength = 8;
const byte contrastAddress = 20;
const byte brightnessAddress = 21;
const byte lcdBrightnessAddress = 22;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() 
{
//  EEPROM.update(contrastAddress, 90);
//  EEPROM.update(lcdBrightnessAddress, 10);
//  EEPROM.update(brightnessAddress, 2);
  
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(lcdBrightnessPin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcdBrightness = EEPROM.read(lcdBrightnessAddress);
  analogWrite(lcdBrightnessPin, lcdBrightness*20);

  contrast = EEPROM.read(contrastAddress);
  analogWrite(potentiometer, contrast);
  lcd.begin(16, 2);

//  writeHighscore1EEPROM(0);
//  writeHighscore2EEPROM(0);
//  writeHighscore3EEPROM(0);
  highscore1 = readHighscore1EEPROM(); 
  highscore2 = readHighscore2EEPROM(); 
  highscore3 = readHighscore3EEPROM(); 
  
  lc.shutdown(0, false);
  brightness = EEPROM.read(brightnessAddress);
  lc.setIntensity(0, brightness);
  lc.clearDisplay(0);

  randomSeed(analogRead(A5));
  
  // Greetings message
  lcd.setCursor(3, 0);
  lcd.print("Welcome to");
  lcd.setCursor(4, 1);
  lcd.print("Skystar!");

  analogWrite(buzzer, HIGH);

  // start music
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) 
  {
    divider = melody[thisNote + 1];
    if (divider > 0) 
    {
      noteDuration = (wholenote) / divider;
    } 
    else if (divider < 0) 
    {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }
    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }

  displayMenu();

  Serial.begin(9600);
}

void loop() 
{  
  noTone(buzzer);
  readJoystickMenu();
  readClick(gameMode);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void cursorSound()
{
  tone(buzzer, 500, 10);
}
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

    if ((xValue < lowerValue or xValue > higherValue) && !joyMovedX)
    {
      if (xIndex == 0) 
      {
        cursorSound();
        xIndex = 1;
        displayMenu();
      }
      else 
      {
        cursorSound();
        xIndex = 0;
        displayMenu();
      }
      joyMovedX = true;
    }
    
    if ((yValue < lowerValue or yValue > higherValue) && !joyMovedY)
    {
      if (yIndex == 0) 
      {
        cursorSound();
        yIndex = 1;
        displayMenu();
      }
      else 
      {
        cursorSound();
        yIndex = 0;
        displayMenu();
      }
      joyMovedY = true;
    }

    if (lowerValue <= xValue && xValue <= higherValue) 
    {
      joyMovedX = false;
    } 
    
    if (lowerValue <= yValue && yValue <= higherValue) 
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
      noTone(buzzer);
      buttonState = swState;
      if (buttonState == LOW) 
      {
        // main menu
        if (mode == gameMode)
        {
          if (xIndex == 0)
          {
            if (yIndex == 0)
            {
              nameSet = false;
              level = 1;
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

        // name
        if (mode == nameFirstLetterMode)
        {
          name[0] = alphabet[alphabetCounter];
          cursor = 1;
          nameCursorMode = nameSecondLetterMode;
        }
        if (mode == nameSecondLetterMode)
        {
          name[1] = alphabet[alphabetCounter];
          cursor = 2;
          nameCursorMode = nameThirdLetterMode;
        }
        if (mode == nameThirdLetterMode)
        {
          name[2] = alphabet[alphabetCounter];
          nameCursorMode = nameWaitMode;
        }
        if (mode == nameStartMode)
        {
          done = true;
          nameBack = true;
        }

        // shoot
        if (0 <= mode and mode <= 7)
        {
          if (level != 5)
          {
            for (int col = 0; col <= 5; col++)
            {
              tone(buzzer, 1000, 10);
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
              }
              if (level == 4)
              {
                if (mode == lvl4ship1poz and lvl4ship1)
                {
                  lvl4ship1 = false;
                  presentScore += 4;
                  displayGameScreen();
                }
                if (mode == lvl4ship2poz and lvl4ship2)
                {
                  lvl4ship2 = false;
                  presentScore += 4;
                  displayGameScreen();
                }
                if (mode == lvl4ship3poz and lvl4ship3)
                {
                  lvl4ship3 = false;
                  presentScore += 4;
                  displayGameScreen();
                }
                if (mode == lvl4ship4poz and lvl4ship4)
                {
                  lvl4ship4 = false;
                  presentScore += 4;
                  displayGameScreen();
                }
              }
            }
          }
          else
          {
            for (int col = 0; col <= 1; col++)
            {
              tone(buzzer, 1000, 10);
              lc.setLed(0, mode, 5 - col, true);
              delay(10);
              lc.setLed(0, mode, 5 - col, false);
            }
            
            if (bossHp)
            {
                bossHp--;
                presentScore++;
                displayGameScreen();
            }
          }
        }

        // score
        if (mode == scoreMode)
        {
          scoreBack = true;
          displayMenu();
        }

        // settings menu
        if (mode == settingsMenuMode)
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

        // settings brightness
        if (mode == brightnessMenuMode)
        {
          if (xIndex == 0)
          {
            if (yIndex == 0)
            {
              matrixBrightnessSettings();
            }
            else
            {
              lcdBrightnessSettings();
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
          EEPROM.update(brightnessAddress, brightness);
          lcd.setCursor(0, 1);
          if (brightness < 10)
          {
            lcd.print("0");
          }
          lcd.print(brightness);
          lc.setIntensity(0, brightness);
        } 

        if (mode == defaultMatrixBrightnessMode)
        {
          brightness = 2;
          EEPROM.update(brightnessAddress, brightness);
          lcd.setCursor(0, 1);
          lcd.print("02");
          lc.setIntensity(0, brightness);
        }

        if (50 <= mode and mode <= 60)
        {
          lcdBrightness = mode-50;
          EEPROM.update(lcdBrightnessAddress, lcdBrightness);
          lcd.setCursor(0, 1);
          if (lcdBrightness < 10)
          {
            lcd.print("0");
          }
          lcd.print(lcdBrightness);
          analogWrite(lcdBrightnessPin, lcdBrightness*20);          
        }

        if (mode == defaultLcdBrightnessMode)
        {
          lcdBrightness = 10;
          EEPROM.update(lcdBrightnessAddress, lcdBrightness);
          lcd.setCursor(0, 1);
          lcd.print("10");
          analogWrite(lcdBrightnessPin, lcdBrightness*20);          
        }

        // back to brightness menu
        if (mode == backToBrightnessMode)
        {
          matrixBrightnessBack = true;
          lcdBrightnessBack = true;
          lc.clearDisplay(0);
          displayBrightness();
          matrixSettings();
        }

        // settigs contrast
        if (100 <= mode and mode <= 250)
        {
          contrast = mode-100;
          EEPROM.update(contrastAddress, contrast);
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

        if (mode == defaultContrastMode)
        {
          contrast = 90;
          EEPROM.update(contrastAddress, contrast);
          lcd.setCursor(0, 1);
          lcd.print("090");
          analogWrite(potentiometer, contrast);
        }
        
        if (mode == backToSettingsMode)
        {
          contrastBack = true;
          lc.clearDisplay(0);
          displaySettings();
          matrixSettings();
        }

        // about
        if (mode == aboutMode)
        {
          positionCounter = 25;
          aboutBack = true;
          displayMenu();
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
  lcd.setCursor(6, 0);
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
  }
}

void enemyAnimation(int shipLin, int col)
{
  for (int shipCol = 0; shipCol <= col; shipCol++)
  {
    lc.setLed(0, shipLin, shipCol + 1, HIGH);
    lc.setLed(0, shipLin, shipCol, HIGH);
    tone(buzzer, 250, 10);
    delay(300);
    lc.setLed(0, shipLin, shipCol + 1, LOW);
    lc.setLed(0, shipLin, shipCol, LOW);
  }
}

void victoryAnimation(int shipLin)
{
  for (int shipCol = 6; shipCol >= 0; shipCol--)
  {
    lc.setLed(0, shipLin, shipCol - 1, HIGH);
    lc.setLed(0, shipLin, shipCol, HIGH);
    lc.setLed(0, shipLin + 1, shipCol, HIGH);
    lc.setLed(0, shipLin - 1, shipCol, HIGH);
    delay(200);
    lc.setLed(0, shipLin, shipCol - 1, LOW);
    lc.setLed(0, shipLin, shipCol, LOW);
    lc.setLed(0, shipLin + 1, shipCol, LOW);
    lc.setLed(0, shipLin - 1, shipCol, LOW);
  }
}

void displayVictoryScreen(int shipLin)
{
  lc.clearDisplay(0);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Good job!");
  lcd.setCursor(1, 1);
  lcd.print("Level ");
  lcd.print(level);
  lcd.print(" Cleared");
  victoryAnimation(shipLin);
  tone(buzzer,800,150);
  delay(150);
  tone(buzzer,500,500);
  delay(500);
  tone(buzzer,600,1000);
  delay(850);

  displayMenu();
}

void displayEnd()
{
  lc.clearDisplay(0);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Great job!");
  lcd.setCursor(1, 1);
  lcd.print("Game cleared!");
  
  for (int col = 7; col >= 2; col--)
  {
    lc.setLed(0, 4, col, HIGH);
    delay(200);
    lc.setLed(0, 4, col, LOW);
  }
  
  lc.setLed(0, 3, 2, HIGH);
  lc.setLed(0, 4, 1, HIGH);
  lc.setLed(0, 4, 3, HIGH);
  lc.setLed(0, 5, 2, HIGH);
  delay(200);
  lc.setLed(0, 3, 2, LOW);
  lc.setLed(0, 4, 1, LOW);
  lc.setLed(0, 4, 3, LOW);
  lc.setLed(0, 5, 2, LOW);
  
  lc.setLed(0, 2, 2, HIGH);
  lc.setLed(0, 3, 1, HIGH);
  lc.setLed(0, 3, 3, HIGH);
  lc.setLed(0, 4, 0, HIGH);
  lc.setLed(0, 4, 4, HIGH);
  lc.setLed(0, 5, 1, HIGH);
  lc.setLed(0, 5, 3, HIGH);
  lc.setLed(0, 6, 2, HIGH);
  delay(200);
  lc.setLed(0, 2, 2, LOW);
  lc.setLed(0, 3, 1, LOW);
  lc.setLed(0, 3, 3, LOW);
  lc.setLed(0, 4, 0, LOW);
  lc.setLed(0, 4, 4, LOW);
  lc.setLed(0, 5, 1, LOW);
  lc.setLed(0, 5, 3, LOW);
  lc.setLed(0, 6, 2, LOW);

  for (int col = 7; col >= 4; col--)
  {
    lc.setLed(0, 3, col, HIGH);
    delay(200);
    lc.setLed(0, 3, col, LOW);
  }
  
  lc.setLed(0, 2, 4, HIGH);
  lc.setLed(0, 3, 3, HIGH);
  lc.setLed(0, 3, 5, HIGH);
  lc.setLed(0, 4, 4, HIGH);
  delay(200);
  lc.setLed(0, 2, 4, LOW);
  lc.setLed(0, 3, 3, LOW);
  lc.setLed(0, 3, 5, LOW);
  lc.setLed(0, 4, 4, LOW);
  
  lc.setLed(0, 1, 4, HIGH);
  lc.setLed(0, 2, 3, HIGH);
  lc.setLed(0, 2, 5, HIGH);
  lc.setLed(0, 3, 2, HIGH);
  lc.setLed(0, 3, 6, HIGH);
  lc.setLed(0, 4, 3, HIGH);
  lc.setLed(0, 4, 5, HIGH);
  lc.setLed(0, 5, 4, HIGH);
  delay(200);
  lc.setLed(0, 1, 4, LOW);
  lc.setLed(0, 2, 3, LOW);
  lc.setLed(0, 2, 5, LOW);
  lc.setLed(0, 3, 4, LOW);
  lc.setLed(0, 3, 6, LOW);
  lc.setLed(0, 4, 3, LOW);
  lc.setLed(0, 4, 5, LOW);
  lc.setLed(0, 5, 4, LOW);
  
  displayMenu();
}

void displayDefeatScreen()
{
  tone(buzzer,600,1000);
  delay(150);
  tone(buzzer,700,150);
  delay(500);
  tone(buzzer,500,500);
  delay(150);
  tone(buzzer,400,500);
  delay(150);
  tone(buzzer,300,500);
  delay(150);
  tone(buzzer,200,500);
  delay(500);
  
  lc.clearDisplay(0);

  lc.setLed(0, 1, 1, true);
  lc.setLed(0, 1, 2, true);
  lc.setLed(0, 1, 6, true);
  lc.setLed(0, 2, 1, true);
  lc.setLed(0, 2, 2, true);
  lc.setLed(0, 2, 5, true);
  lc.setLed(0, 3, 5, true);
  lc.setLed(0, 4, 5, true);
  lc.setLed(0, 5, 1, true);
  lc.setLed(0, 5, 2, true);
  lc.setLed(0, 5, 5, true);
  lc.setLed(0, 6, 1, true);
  lc.setLed(0, 6, 2, true);
  lc.setLed(0, 6, 6, true);

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Oh, no!");
  lcd.setCursor(1, 1);
  lcd.print("Level ");
  lcd.print(level);
  lcd.print(" Failed!");
  delay(1500);
  lc.clearDisplay(0);
  displayScore();
  displayMenu();  
}

void displayScore()
{
  if (presentScore > highscore1)
  {
    highscore3 = highscore2;
    highscore2 = highscore1;
    lc.clearDisplay(0);
    lcd.clear();  
    lcd.setCursor(1, 0);
    lcd.print("New highscore!");
    lcd.setCursor(7, 1);
    lcd.print(presentScore);
    highscore1 = presentScore;
    writeHighscore1EEPROM(highscore1);
    writeHighscore2EEPROM(highscore2);
    writeHighscore3EEPROM(highscore3);
    delay(1500);
  }
  else if (presentScore > highscore2)
  {
    highscore3 = highscore2;
    lc.clearDisplay(0);
    lcd.clear();  
    lcd.setCursor(0, 0);
    lcd.print("New second score!");
    lcd.setCursor(7, 1);
    lcd.print(presentScore);
    highscore2 = presentScore;
    writeHighscore2EEPROM(highscore2);
    writeHighscore3EEPROM(highscore3);
    delay(1500);
  }
  else if (presentScore > highscore3)
  {
    lc.clearDisplay(0);
    lcd.clear();  
    lcd.setCursor(0, 0);
    lcd.print("New third score!");
    lcd.setCursor(7, 1);
    lcd.print(presentScore);
    highscore3 = presentScore;
    writeHighscore3EEPROM(highscore3);
    delay(1500);
  }  
}

void bossAnimation()
{
  // boss theme song
  for (int thisNote = 0; thisNote < notesBoss * 2; thisNote = thisNote + 2) 
  {
    divider = melodyBoss[thisNote + 1];
    if (divider > 0) 
    {
      noteDuration = (wholenoteBoss) / divider;
    } 
    else if (divider < 0) 
    {
      noteDuration = (wholenoteBoss) / abs(divider);
      noteDuration *= 1.5;
    }
    tone(buzzer, melodyBoss[thisNote], noteDuration*0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
  
  lc.setLed(0, 2, 0, true);
  lc.setLed(0, 5, 0, true);
  delay(1000);
  lc.setLed(0, 2, 0, false);
  lc.setLed(0, 5, 0, false);
  
  for (int lin = 0; lin <= 7; lin++)
  {
    lc.setLed(0, lin, 0, true);
  }
  lc.setLed(0, 2, 1, true);
  lc.setLed(0, 5, 1, true);
  delay(1000);
  for (int lin = 0; lin <= 7; lin++)
  {
    lc.setLed(0, lin, 0, false);
  }
  lc.setLed(0, 2, 1, false);
  lc.setLed(0, 5, 1, false);

  for (int lin = 1; lin <= 6; lin++)
  {
    lc.setLed(0, lin, 0, true);
  }
  for (int lin = 0; lin <= 7; lin++)
  {
    lc.setLed(0, lin, 1, true);
  }
  lc.setLed(0, 2, 2, true);
  lc.setLed(0, 5, 2, true);
  delay(1000);
  for (int lin = 1; lin <= 6; lin++)
  {
    lc.setLed(0, lin, 0, false);
  }
  for (int lin = 0; lin <= 7; lin++)
  {
    lc.setLed(0, lin, 0, false);
  }
  lc.setLed(0, 2, 2, false);
  lc.setLed(0, 5, 2, false);
}

void boss()
{
  for (int lin = 0; lin <= 7; lin++)
  {
    for (int col = 0; col <= 2; col++)
    {
        lc.setLed(0, lin, col, true);
    }
  }
  lc.setLed(0, 0, 1, false);
  lc.setLed(0, 1, 0, false);
  lc.setLed(0, 6, 0, false);
  lc.setLed(0, 7, 1, false);
  lc.setLed(0, 2, 3, true);
  lc.setLed(0, 5, 3, true);
}

void bossFireRight()
{
  for (int col = 4; col <= 7; col++)
  {
    lc.setLed(0, 2, col, true);
    cursorSound();
    delay(15);
    lc.setLed(0, 2, col, false);
  }  
}

void bossFireLeft()
{
  for (int col = 4; col <= 7; col++)
  {
    lc.setLed(0, 5, col, true);
    cursorSound();
    delay(15);
    lc.setLed(0, 5, col, false);
  }  
}

void bossUltimate1()
{
  for (int col = 4; col <= 7; col++)
  {
    cursorSound();
    lc.setLed(0, 2, col, true);
    if (col ==  5)
    {
      lc.setLed(0, 1, col, true);
      lc.setLed(0, 3, col, true);
    }
    if (col ==  7)
    {
      lc.setLed(0, 0, col, true);
      lc.setLed(0, 4, col, true);
    }
    delay(100);
    lc.setLed(0, 2, col, false);
    lc.setLed(0, 1, 5, false);
    lc.setLed(0, 3, 5, false);
    lc.setLed(0, 0, 7, false);
    lc.setLed(0, 4, 7, false);
  }
  
  for (int col = 4; col <= 7; col++)
  {
    cursorSound();
    lc.setLed(0, 5, col, true);
    if (col ==  5)
    {
      lc.setLed(0, 4, col, true);
      lc.setLed(0, 6, col, true);
    }
    if (col ==  7)
    {
      lc.setLed(0, 3, col, true);
      lc.setLed(0, 7, col, true);
    }
    delay(100);
    lc.setLed(0, 5, col, false);
    lc.setLed(0, 4, 5, false);
    lc.setLed(0, 6, 5, false);
    lc.setLed(0, 3, 7, false);
    lc.setLed(0, 7, 7, false);
  }   
}

void gotHit()
{
  tone(buzzer, NOTE_G4);
  delay(125);
  tone(buzzer, NOTE_C4);
  delay(250);
  noTone(buzzer);
}

void generateShipsLvl2()
{
  lvl2ship1 = true;
  lvl2ship2 = true;
    
  lvl2ship1poz = random(0, 5);
  lvl2ship1col = random(0, 2);
  enemyAnimation(lvl2ship1poz, lvl2ship1col);
  lc.setLed(0, lvl2ship1poz, lvl2ship1col, true);
  lc.setLed(0, lvl2ship1poz, lvl2ship1col+1, true);

  lvl2ship2poz = random(5, 8);
  lvl2ship2col = random(2, 4);
  enemyAnimation(lvl2ship2poz, lvl2ship2col);
  lc.setLed(0, lvl2ship2poz, lvl2ship2col, true);
  lc.setLed(0, lvl2ship2poz, lvl2ship2col+1, true);
}

void generateShipsLvl3()
{
  lvl3ship1 = true;
  lvl3ship2 = true;
  lvl3ship3 = true;
    
  lvl3ship1poz = random(0, 3);
  lvl3ship1col = 0;
  enemyAnimation(lvl3ship1poz, lvl3ship1col);
  lc.setLed(0, lvl3ship1poz, lvl3ship1col, true);
  lc.setLed(0, lvl3ship1poz, lvl3ship1col+1, true);
    
  lvl3ship2poz = random(3, 6);
  lvl3ship2col = 1;
  enemyAnimation(lvl3ship2poz, lvl3ship2col);
  lc.setLed(0, lvl3ship2poz, lvl3ship2col, true);
  lc.setLed(0, lvl3ship2poz, lvl3ship2col+1, true);

  lvl3ship3poz = random(6, 8);
  lvl3ship3col = 3;
  enemyAnimation(lvl3ship3poz, lvl3ship3col);
  lc.setLed(0, lvl3ship3poz, lvl3ship3col, true);
  lc.setLed(0, lvl3ship3poz, lvl3ship3col+1, true);
}

void generateShipsLvl4()
{
  lvl4ship1 = true;
  lvl4ship2 = true;
  lvl4ship3 = true;
  lvl4ship4 = true;
    
  lvl4ship1poz = random(0, 2);
  lvl4ship1col = 1;
  enemyAnimation(lvl4ship1poz, 1);
  lc.setLed(0, lvl4ship1poz, lvl4ship1col, true);
  lc.setLed(0, lvl4ship1poz, lvl4ship1col+1, true);

  lvl4ship2poz = random(2, 4);
  lvl4ship2col = 3;
  enemyAnimation(lvl4ship2poz, 3);
  lc.setLed(0, lvl4ship2poz, lvl4ship2col, true);
  lc.setLed(0, lvl4ship2poz, lvl4ship2col+1, true);

  lvl4ship3poz = random(4, 6);
  lvl4ship3col = 3;
  enemyAnimation(lvl4ship3poz, 3);
  lc.setLed(0, lvl4ship3poz, lvl4ship3col, true);
  lc.setLed(0, lvl4ship3poz, lvl4ship3col+1, true);

  lvl4ship4poz = random(6, 8);
  lvl4ship4col = 1;
  enemyAnimation(lvl4ship4poz, 1);
  lc.setLed(0, lvl4ship4poz, lvl4ship4col, true);
  lc.setLed(0, lvl4ship4poz, lvl4ship4col+1, true);
}

void startGame()
{
  bool playing = true;
  int shipLin = 4;
  int shipCol = 7;
  int lin, col;
  int charge = 0;
  hp = 3;
  bossHp = 100;

  if (!nameSet)
  {
    nameSettings();
    presentScore = 0;
    nameSet = true;
  }

  displayGameScreen();

  if (level == 1)
  {
    lvl1ship1 = true;
    lvl1ship2 = true;
    lvl1ship3 = true;

    enemyAnimation(1, 1);
    lc.setLed(0, 1, 1, true);
    lc.setLed(0, 1, 2, true);

    enemyAnimation(3, 2);
    lc.setLed(0, 3, 2, true);
    lc.setLed(0, 3, 3, true);

    enemyAnimation(5, 0);
    lc.setLed(0, 5, 0, true);
    lc.setLed(0, 5, 1, true);
  }
  
  if (level == 2)
  {
    generateShipsLvl2();
    gameTime = millis();
  }

  if (level == 3)
  {
    generateShipsLvl3();
  }

  if (level == 4)
  {
    generateShipsLvl4();
  }

  if (level == 5)
  {
    bossAnimation();
    boss(); 
  }

  while (playing)
  {
    lc.setLed(0, shipLin, shipCol - 1, HIGH);
    lc.setLed(0, shipLin, shipCol, HIGH);
    lc.setLed(0, shipLin + 1, shipCol, HIGH);
    lc.setLed(0, shipLin - 1, shipCol, HIGH);

    readClick(shipLin);

    yValue = analogRead(pinY);
    if (yValue < lowerValue && !joyMovedY)
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
    else if (yValue > higherValue && !joyMovedY)
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
    if (lowerValue <= yValue && yValue <= higherValue) 
    {
      joyMovedY = false;
    }

    if (level == 1 and lvl1ship1 == false and lvl1ship2 == false and lvl1ship3 == false)
    {
      displayVictoryScreen(shipLin);
      level = 2;
      playing = false;
      startGame();
    }
    
    if (level == 2) 
    {
      unsigned int elapsedTimeShip1 = millis();
      if (elapsedTimeShip1 - lastChangedship1 > 750)
      {
        if (lvl2ship1)
        {
          lin = random(0, 2);
          if (lin == 0 and lvl2ship1poz > minLin)
          {
            lc.setLed(0, lvl2ship1poz, lvl2ship1col, false);
            lc.setLed(0, lvl2ship1poz, lvl2ship1col+1, false);              
            lvl2ship1poz--;
            lc.setLed(0, lvl2ship1poz, lvl2ship1col, true);
            lc.setLed(0, lvl2ship1poz, lvl2ship1col+1, true); 
          }
          if (lin == 1 and lvl2ship1poz < maxLin)
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
          if (lin == 0 and lvl2ship2poz > minLin)
          {
            lc.setLed(0, lvl2ship2poz, lvl2ship2col, false);
            lc.setLed(0, lvl2ship2poz, lvl2ship2col+1, false);              
            lvl2ship2poz--;
            lc.setLed(0, lvl2ship2poz, lvl2ship2col, true);
            lc.setLed(0, lvl2ship2poz, lvl2ship2col+1, true); 
          }
          if (lin == 1 and lvl2ship2poz < maxLin)
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
        generateShipsLvl2();
      }

      if (millis() - gameTime > 20000) 
      {
        displayVictoryScreen(shipLin);
        level = 3;  
        gameTime = millis();
        playing = false;
        startGame();
      }
    }

    if (level == 3)
    {
      unsigned int elapsedTimeShip1 = millis();
      if (elapsedTimeShip1 - lastChangedship1 > 1000)
      {
        if (lvl3ship1 and playing)
        {
          bool damage = false;
          for (int col = lvl3ship1col+2; col <= maxCol; col++)
          {
            lc.setLed(0, lvl3ship1poz, col, true);
            cursorSound();
            delay(10);
            lc.setLed(0, lvl3ship1poz, col, false);
            if (shipLin == lvl3ship1poz and !damage)
            {
              if (hp)
              {
                hp--;
                displayGameScreen();
                damage = true;
                gotHit();
              }
              if (!hp)
              {
                displayDefeatScreen();
                playing = false;
                col = 8;  
                gameTime = millis();
              }
            }
          }

          if (playing)
          {
            lin = random(0, 2);
            if (lin == 0 and lvl3ship1poz > minLin)
            {
              lc.setLed(0, lvl3ship1poz, lvl3ship1col, false);
              lc.setLed(0, lvl3ship1poz, lvl3ship1col+1, false);              
              lvl3ship1poz--;
              lc.setLed(0, lvl3ship1poz, lvl3ship1col, true);
              lc.setLed(0, lvl3ship1poz, lvl3ship1col+1, true); 
            }
            if (lin == 1 and lvl3ship1poz < maxLin)
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
          }
        }
        lastChangedship1 = elapsedTimeShip1;
      }

      unsigned int elapsedTimeShip2 = millis();
      if (elapsedTimeShip2 - lastChangedship2 > 2000)
      {
        if (lvl3ship2 and playing)
        {
          bool damage = false;
          for (int col = lvl3ship2col+2; col <= maxCol; col++)
          {
            lc.setLed(0, lvl3ship2poz, col, true);
            cursorSound();
            delay(10);
            lc.setLed(0, lvl3ship2poz, col, false);
            if (shipLin == lvl3ship2poz and !damage)
            {
              if (hp)
              {
                hp--;
                displayGameScreen();
                damage = true;
                gotHit();
              }
              if (!hp)
              {
                displayDefeatScreen();
                playing = false;
                col = 8;  
                gameTime = millis();
              }
            }
          }

          if (playing)
          {
            lin = random(0, 2);
            if (lin == 0 and lvl3ship2poz > minLin)
            {
              lc.setLed(0, lvl3ship2poz, lvl3ship2col, false);
              lc.setLed(0, lvl3ship2poz, lvl3ship2col+1, false);              
              lvl3ship2poz--;
              lc.setLed(0, lvl3ship2poz, lvl3ship2col, true);
              lc.setLed(0, lvl3ship2poz, lvl3ship2col+1, true); 
            }
            if (lin == 1 and lvl3ship2poz < maxLin)
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
          }
        }
        lastChangedship2 = elapsedTimeShip2;
      }

      unsigned int elapsedTimeShip3 = millis();
      if (elapsedTimeShip3 - lastChangedship3 > 2500)
      {
        if (lvl3ship3 and playing)
        {
          bool damage = false;
          for (int col = lvl3ship3col+2; col <= maxCol; col++)
          {
            lc.setLed(0, lvl3ship3poz, col, true);
            cursorSound();
            delay(10);
            lc.setLed(0, lvl3ship3poz, col, false);
            if (shipLin == lvl3ship3poz and !damage)
            {
              if (hp)
              {
                hp--;
                displayGameScreen();
                damage = true;
                gotHit();
              }
              if (!hp)
              {
                displayDefeatScreen();
                playing = false;
                col = 8;  
                gameTime = millis();
              }
            }
          }
          
          if (playing)
          {
            lin = random(0, 2);
            if (lin == 0 and lvl3ship3poz > minLin)
            {
              lc.setLed(0, lvl3ship3poz, lvl3ship3col, false);
              lc.setLed(0, lvl3ship3poz, lvl3ship3col+1, false);              
              lvl3ship3poz--;
              lc.setLed(0, lvl3ship3poz, lvl3ship3col, true);
              lc.setLed(0, lvl3ship3poz, lvl3ship3col+1, true); 
            }
            if (lin == 1 and lvl3ship3poz < maxLin)
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
          }
        }
        lastChangedship3 = elapsedTimeShip3;
      }

      if (lvl3ship1 == false and lvl3ship2 == false and lvl3ship3 == false)
      {
        generateShipsLvl3();
      }

      if (millis() - gameTime > 30000 and playing)
      {
        col = 8;
        displayVictoryScreen(shipLin);
        level = 4;  
        gameTime = millis();
        playing = false;
        startGame();
      }
    }

    if (level == 4)
    {
      unsigned int elapsedTimeShip1 = millis();
      if (elapsedTimeShip1 - lastChangedship1 > 1000)
      {
        if (lvl4ship1 and playing)
        {
          bool damage = false;
          for (int col = lvl4ship1col+2; col <= maxCol; col++)
          {
            lc.setLed(0, lvl4ship1poz, col, true);
            cursorSound();
            delay(10);
            lc.setLed(0, lvl4ship1poz, col, false);
            if (shipLin == lvl4ship1poz and !damage)
            {
              if (hp)
              {
                hp--;
                displayGameScreen();
                damage = true;
                gotHit();
              }
              if (!hp)
              {
                displayDefeatScreen();
                playing = false;
                col = 8;  
                gameTime = millis();
              }
            }
          }

          if (playing)
          {
            if (lvl4ship1poz > shipLin)
            {
              lc.setLed(0, lvl4ship1poz, lvl4ship1col, false);
              lc.setLed(0, lvl4ship1poz, lvl4ship1col+1, false);              
              lvl4ship1poz--;
              lc.setLed(0, lvl4ship1poz, lvl4ship1col, true);
              lc.setLed(0, lvl4ship1poz, lvl4ship1col+1, true); 
            }
            if (lvl4ship1poz < shipLin)
            {
              lc.setLed(0, lvl4ship1poz, lvl4ship1col, false);
              lc.setLed(0, lvl4ship1poz, lvl4ship1col+1, false);
              lvl4ship1poz++;
              lc.setLed(0, lvl4ship1poz, lvl4ship1col, true);
              lc.setLed(0, lvl4ship1poz, lvl4ship1col+1, true);  
            }
          
            if (lvl4ship2)
            {
              lc.setLed(0, lvl4ship2poz, lvl4ship2col, true);
              lc.setLed(0, lvl4ship2poz, lvl4ship2col+1, true);            
            }
            if (lvl4ship3)
            {
              lc.setLed(0, lvl4ship3poz, lvl4ship3col, true);
              lc.setLed(0, lvl4ship3poz, lvl4ship3col+1, true);            
            }
            if (lvl4ship4)
            {
              lc.setLed(0, lvl4ship4poz, lvl4ship4col, true);
              lc.setLed(0, lvl4ship4poz, lvl4ship4col+1, true);            
            }
          }
        }
        lastChangedship1 = elapsedTimeShip1;
      }

      unsigned int elapsedTimeShip2 = millis();
      if (elapsedTimeShip2 - lastChangedship2 > 1500)
      {
        if (lvl4ship2 and playing)
        {
          bool damage = false;
          for (int col = lvl4ship2col+2; col <= maxCol; col++)
          {
            lc.setLed(0, lvl4ship2poz, col, true);
            cursorSound();
            delay(10);
            lc.setLed(0, lvl4ship2poz, col, false);
            if (shipLin == lvl4ship2poz and !damage)
            {
              if (hp)
              {
                hp--;
                displayGameScreen();
                damage = true;
                gotHit();
              }
              if (!hp)
              {
                displayDefeatScreen();
                playing = false;
                col = 8;  
                gameTime = millis();
              }
            }
          }

          if (playing)
          {
            if (lvl4ship2poz > shipLin)
            {
              lc.setLed(0, lvl4ship2poz, lvl4ship2col, false);
              lc.setLed(0, lvl4ship2poz, lvl4ship2col+1, false);              
              lvl4ship2poz--;
              lc.setLed(0, lvl4ship2poz, lvl4ship2col, true);
              lc.setLed(0, lvl4ship2poz, lvl4ship2col+1, true); 
            }
            if (lvl4ship2poz < shipLin)
            {
              lc.setLed(0, lvl4ship2poz, lvl4ship2col, false);
              lc.setLed(0, lvl4ship2poz, lvl4ship2col+1, false);
              lvl4ship2poz++;
              lc.setLed(0, lvl4ship2poz, lvl4ship2col, true);
              lc.setLed(0, lvl4ship2poz, lvl4ship2col+1, true);  
            }
          
            if (lvl4ship1)
            {
              lc.setLed(0, lvl4ship1poz, lvl4ship1col, true);
              lc.setLed(0, lvl4ship1poz, lvl4ship1col+1, true);            
            }
            if (lvl4ship3)
            {
              lc.setLed(0, lvl4ship3poz, lvl4ship3col, true);
              lc.setLed(0, lvl4ship3poz, lvl4ship3col+1, true);            
            }
            if (lvl4ship4)
            {
              lc.setLed(0, lvl4ship4poz, lvl4ship4col, true);
              lc.setLed(0, lvl4ship4poz, lvl4ship4col+1, true);            
            }
          }
        }
        lastChangedship2 = elapsedTimeShip2;
      }

      unsigned int elapsedTimeShip3 = millis();
      if (elapsedTimeShip3 - lastChangedship3 > 1250)
      {
        if (lvl4ship3 and playing)
        {
          bool damage = false;
          for (int col = lvl4ship3col+2; col <= maxCol; col++)
          {
            lc.setLed(0, lvl4ship3poz, col, true);
            cursorSound();
            delay(10);
            lc.setLed(0, lvl4ship3poz, col, false);
            if (shipLin == lvl4ship3poz and !damage)
            {
              if (hp)
              {
                hp--;
                displayGameScreen();
                damage = true;
                gotHit();
              }
              if (!hp)
              {
                displayDefeatScreen();
                playing = false;
                col = 8;  
                gameTime = millis();
              }
            }
          }

          if (playing)
          {
            if (lvl4ship3poz > shipLin)
            {
              lc.setLed(0, lvl4ship3poz, lvl4ship3col, false);
              lc.setLed(0, lvl4ship3poz, lvl4ship3col+1, false);              
              lvl4ship3poz--;
              lc.setLed(0, lvl4ship3poz, lvl4ship3col, true);
              lc.setLed(0, lvl4ship3poz, lvl4ship3col+1, true); 
            }
            if (lvl4ship3poz < shipLin)
            {
              lc.setLed(0, lvl4ship3poz, lvl4ship3col, false);
              lc.setLed(0, lvl4ship3poz, lvl4ship3col+1, false);
              lvl4ship3poz++;
              lc.setLed(0, lvl4ship3poz, lvl4ship3col, true);
              lc.setLed(0, lvl4ship3poz, lvl4ship3col+1, true);  
            }
          
            if (lvl4ship1)
            {
              lc.setLed(0, lvl4ship1poz, lvl4ship1col, true);
              lc.setLed(0, lvl4ship1poz, lvl4ship1col+1, true);            
            }
            if (lvl4ship2)
            {
              lc.setLed(0, lvl4ship2poz, lvl4ship2col, true);
              lc.setLed(0, lvl4ship2poz, lvl4ship2col+1, true);            
            }
            if (lvl4ship4)
            {
              lc.setLed(0, lvl4ship4poz, lvl4ship4col, true);
              lc.setLed(0, lvl4ship4poz, lvl4ship4col+1, true);            
            }
          }
        }
        lastChangedship3 = elapsedTimeShip3;
      }

      unsigned int elapsedTimeShip4 = millis();
      if (elapsedTimeShip4 - lastChangedship4 > 1750)
      {
        if (lvl4ship4 and playing)
        {
          bool damage = false;
          for (int col = lvl4ship4col+2; col <= maxCol; col++)
          {
            lc.setLed(0, lvl4ship4poz, col, true);
            cursorSound();
            delay(10);
            lc.setLed(0, lvl4ship4poz, col, false);
            if (shipLin == lvl4ship4poz and !damage)
            {
              if (hp)
              {
                hp--;
                displayGameScreen();
                damage = true;
                gotHit();
              }
              if (!hp)
              {
                displayDefeatScreen();
                playing = false;
                col = 8;  
                gameTime = millis();
              }
            }
          }

          if (playing)
          {
            if (lvl4ship4poz > shipLin)
            {
              lc.setLed(0, lvl4ship4poz, lvl4ship4col, false);
              lc.setLed(0, lvl4ship4poz, lvl4ship4col+1, false);              
              lvl4ship4poz--;
              lc.setLed(0, lvl4ship4poz, lvl4ship4col, true);
              lc.setLed(0, lvl4ship4poz, lvl4ship4col+1, true); 
            }
            if (lvl4ship4poz < shipLin)
            {
              lc.setLed(0, lvl4ship4poz, lvl4ship4col, false);
              lc.setLed(0, lvl4ship4poz, lvl4ship4col+1, false);
              lvl4ship4poz++;
              lc.setLed(0, lvl4ship4poz, lvl4ship4col, true);
              lc.setLed(0, lvl4ship4poz, lvl4ship4col+1, true);  
            }
          
            if (lvl4ship1)
            {
              lc.setLed(0, lvl4ship1poz, lvl4ship1col, true);
              lc.setLed(0, lvl4ship1poz, lvl4ship1col+1, true);            
            }
            if (lvl4ship2)
            {
              lc.setLed(0, lvl4ship2poz, lvl4ship2col, true);
              lc.setLed(0, lvl4ship2poz, lvl4ship2col+1, true);            
            }
            if (lvl4ship3)
            {
              lc.setLed(0, lvl4ship3poz, lvl4ship3col, true);
              lc.setLed(0, lvl4ship3poz, lvl4ship3col+1, true);            
            }
          }
        }
        lastChangedship4 = elapsedTimeShip4;
      }

      if (lvl4ship1 == false and lvl4ship2 == false and lvl4ship3 == false and lvl4ship4 == false)
      {
        generateShipsLvl4();
      }

      if (millis() - gameTime > 40000 and playing)
      {
        col = 8;
        displayVictoryScreen(shipLin);
        level = 5;  
        gameTime = millis();
        playing = false;
        startGame();
      }
    }

    if (level == 5)
    {
      unsigned int elapsedTimeShip1 = millis();
      if (elapsedTimeShip1 - lastChangedship1 > 2000)
      {
        lin = random(0, 2);
        if (lin == 0)
        {
          bossFireRight();
          charge++;
        }
        else
        {
          bossFireLeft(); 
          charge++;           
        }
        
        if (playing and bossHp)
        {
          bool damage = false;
          
          if (charge == 5)
          {
            bossUltimate1();
            if (shipLin != 1 and shipLin != 6 and !damage)
            {
              if (hp)
              {
                hp--;
                displayGameScreen();
                damage = true;
                gotHit();
              }
              if (!hp)
              {
                displayDefeatScreen();
                playing = false; 
              }
            }
            charge = 0;
          }
          
          else if (lin == 0 and shipLin == 2 and !damage)
          {
            if (hp)
            {
              hp--;
              displayGameScreen();
              damage = true;
              gotHit();
            }
            if (!hp)
            {
              displayDefeatScreen();
              playing = false; 
            }
          }
          if (lin == 1 and shipLin == 5 and !damage)
          {
            if (hp)
            {
              hp--;
              displayGameScreen();
              damage = true;
              gotHit();
            }
            if (!hp)
            {
              displayDefeatScreen();
              playing = false;
            }
          }
        }
       lastChangedship1 = elapsedTimeShip1;
      }

      if (bossHp == 0)
      {
        displayVictoryScreen(shipLin);
        level = 0;
        playing = false;
        displayScore();
        displayEnd();
      }
    } 
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void writeHighscore1EEPROM(int score) 
{
  byte firstByte = (score >> byteLength) & byteMask;
  byte secondByte = score & byteMask;
  EEPROM.update(0, firstByte);
  EEPROM.update(1, secondByte);
}

void writeHighscore1NameEEPROM(char name[]) 
{
  byte firstByte = name[0] & byteMask;
  byte secondByte = name[1] & byteMask;
  byte thirdByte = name[2] & byteMask;
  EEPROM.update(2, firstByte);
  EEPROM.update(3, secondByte);
  EEPROM.update(4, thirdByte);
}

void writeHighscore2EEPROM(int score) 
{
  byte firstByte = (score >> byteLength) & byteMask;
  byte secondByte = score & byteMask;
  EEPROM.update(5, firstByte);
  EEPROM.update(6, secondByte);
}

void writeHighscore3EEPROM(int score) 
{
  byte firstByte = (score >> byteLength) & byteMask;
  byte secondByte = score & byteMask;
  EEPROM.update(10, firstByte);
  EEPROM.update(11, secondByte);
}

int readHighscore1EEPROM() 
{
  byte firstByte = EEPROM.read(0);
  byte secondByte = EEPROM.read(1);
  return (firstByte << byteLength) + secondByte;
}

int readHighscore1NameEEPROM() 
{
  byte firstByte = EEPROM.read(2);
  byte secondByte = EEPROM.read(3);
  byte thirdByte = EEPROM.read(4);
  return ((firstByte << byteLength) << byteLength) + (secondByte << byteLength) + thirdByte;
}

int readHighscore2EEPROM() 
{
  byte firstByte = EEPROM.read(5);
  byte secondByte = EEPROM.read(6);
  return (firstByte << byteLength) + secondByte;
}

int readHighscore3EEPROM() 
{
  byte firstByte = EEPROM.read(10);
  byte secondByte = EEPROM.read(11);
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
  
  lcd.setCursor(5, 0);
  lcd.print("1: ");
  lcd.setCursor(8, 0);
  lcd.print(readHighscore1EEPROM());
  
//  lcd.setCursor(0, 1);
//  lcd.print("Name: ");
//  lcd.print(readHighscore1NameEEPROM());

  lcd.setCursor(1, 1);
  lcd.print("2: ");
  lcd.setCursor(4, 1);
  lcd.print(readHighscore2EEPROM());

  lcd.setCursor(9, 1);
  lcd.print("3: ");
  lcd.setCursor(12, 1);
  lcd.print(readHighscore3EEPROM());

  while(scoreBack == false)
  {
    readClick(scoreMode);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void displaySettings()
{ 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sound");
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

    if ((xValue < lowerValue or xValue > higherValue) && !joyMovedX)
    {
      if (xIndex == 0) 
      {
        cursorSound();
        xIndex = 1;
        displaySettings();
      }
      else 
      {
        cursorSound();
        xIndex = 0;
        displaySettings();
      }
      joyMovedX = true;
    }
    
    if ((yValue < lowerValue or yValue > higherValue) && !joyMovedY)
    {
      if (yIndex == 0) 
      {
        cursorSound();
        yIndex = 1;
        displaySettings();
      }
      else 
      {
        cursorSound();
        yIndex = 0;
        displaySettings();
      }
      joyMovedY = true;
    }

    if (lowerValue <= xValue && xValue <= higherValue) 
    {
      joyMovedX = false;
    } 
    
    if (lowerValue <= yValue && yValue <= higherValue) 
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
            lcd.print("     ");
            nameBlink = 1;
          }
          else
          {
            lcd.print("Sound");
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
    readClick(settingsMenuMode);
  }
}

void nameSettings()
{
  nameBack = false;

  cursor = 0;
  nameCursorMode = nameFirstLetterMode;
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
  
    if (xValue < lowerValue)
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

    if (xValue > higherValue)
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
    
    readClick(nameCursorMode);
    lcd.setCursor(cursor, 1);

    if (nameCursorMode == nameWaitMode)
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
        readClick(nameStartMode);
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

    if ((xValue < lowerValue or xValue > higherValue) && !joyMovedX)
    {
      if (xIndex == 0) 
      {
        cursorSound();
        xIndex = 1;
        displayBrightness();
      }
      else 
      {
        cursorSound();
        xIndex = 0;
        displayBrightness();
      }
      joyMovedX = true;
    }
    
    if ((yValue < lowerValue or yValue > higherValue) && !joyMovedY)
    {
      if (yIndex == 0) 
      {
        cursorSound();
        yIndex = 1;
        displayBrightness();
      }
      else 
      {
        cursorSound();
        yIndex = 0;
        displayBrightness();
      }
      joyMovedY = true;
    }

    if (lowerValue <= xValue && xValue <= higherValue) 
    {
      joyMovedX = false;
    } 
    
    if (lowerValue <= yValue && yValue <= higherValue) 
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
    readClick(brightnessMenuMode);
  }
}

void matrixBrightnessSettings()
{    
  for (int lin = 0; lin <= maxLin; lin++)
  {
    for (int col = 0; col <= maxCol; col++)
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

    if (yValue < lowerValue && !joyMovedY)
    {
      cursorSound();
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

    if (yValue > higherValue && !joyMovedY)
    {
      cursorSound();
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

    if (lowerValue <= yValue && yValue <= higherValue) 
    {
      joyMovedY = false;
    }

    if (nameIndex == 0)
    {
      xValue = analogRead(pinX);
      
      if (xValue < lowerValue) 
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

      if (xValue > higherValue and nameIndex == 0)
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
      readClick(defaultMatrixBrightnessMode);
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
      readClick(backToBrightnessMode);
    }
  }
}

void lcdBrightnessSettings()
{    
  int lcdBrightnessLevel = lcdBrightness;
  int nameIndex = 0;
  
  lcdBrightnessBack = false;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Change brightness:");
  lcd.setCursor(4, 1);
  lcd.print("Default");
  lcd.setCursor(12, 1);
  lcd.print("Back");

  while (!lcdBrightnessBack)
  {
    yValue = analogRead(pinY);

    if (yValue < lowerValue && !joyMovedY)
    {
      cursorSound();
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

    if (yValue > higherValue && !joyMovedY)
    {
      cursorSound();
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

    if (lowerValue <= yValue && yValue <= higherValue) 
    {
      joyMovedY = false;
    }

    if (nameIndex == 0)
    {
      xValue = analogRead(pinX);
      
      if (xValue < lowerValue) 
      {
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 400)
        {
          lcdBrightnessLevel++;
          lastChanged = elapsedTime;
        }
        if (lcdBrightnessLevel > 10)
        {
          lcdBrightnessLevel = 0;
        }
      }

      if (xValue > higherValue)
      {
        unsigned int elapsedTime = millis();
        if (elapsedTime - lastChanged > 400)
        {
          lcdBrightnessLevel--;
          lastChanged = elapsedTime;
        }
        if (lcdBrightnessLevel < 0)
        {
          lcdBrightnessLevel = 10;
        }
      }
    
      lcd.setCursor(0, 1);
      if (lcdBrightnessLevel < 10)
      {
        lcd.print("0");
      }
      lcd.print(lcdBrightnessLevel);
      readClick(lcdBrightnessLevel + 50);
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
      readClick(defaultLcdBrightnessMode);
      lcdBrightnessLevel = lcdBrightness;
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
      readClick(backToBrightnessMode);
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

    if (yValue < lowerValue && !joyMovedY)
    {
      cursorSound();
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

    if (yValue > higherValue && !joyMovedY)
    {
      cursorSound();
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

    if (lowerValue <= yValue && yValue <= higherValue) 
    {
      joyMovedY = false;
    }

    if (nameIndex == 0)
    {
      xValue = analogRead(pinX);
      
      if (xValue < lowerValue) 
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

      if (xValue > higherValue)
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
      readClick(defaultContrastMode);
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
      readClick(backToSettingsMode);
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
  lc.setLed(0, 1, 2, true);
  lc.setLed(0, 1, 5, true);
  lc.setLed(0, 2, 2, true);
  lc.setLed(0, 2, 6, true);
  lc.setLed(0, 3, 2, true);
  lc.setLed(0, 3, 6, true);
  lc.setLed(0, 3, 7, true);
  lc.setLed(0, 4, 6, true);
  lc.setLed(0, 4, 7, true);
  lc.setLed(0, 5, 1, true);
  lc.setLed(0, 5, 2, true);
  lc.setLed(0, 5, 6, true);
  lc.setLed(0, 6, 1, true);
  lc.setLed(0, 6, 2, true);
  lc.setLed(0, 6, 5, true);
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

    readClick(aboutMode);
  }

  lc.clearDisplay(0);
  displayMenu();
}
