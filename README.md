# Skystar
The Arduino game I made for my robotics class. It is a Space Invaders game, that uses a joystick and a button to move and fire at enemies. It was inspired by Demonstar 2, a game I played when I was little. I had lots of fun making it!

The game is very simple, you control a small spaceship at the bottom of the LCD with a joystick. Your goal is to obtain the highest score possible by shooting as many enemies as you can. It has 5 levels that become more and more difficult:
- the first one is just a tutorial, to get ajusted with the movement and the shooting. You have three stationary enemies that spawn always at the exact same location. Just shoot them to advance.
- from the second level forward you have a limited time/level to shoot enemies. Now you have two random generated enemies that move around very fast. Each time you thake a pair down another random pair will apair.
- from the third one things get serious, as now the enemies will fire back at you and can kill you if you are shot three times, so be careful! Also, like before, if you shoot them all they will be replaced, only now there are three at a time.
- the forth level adds four enemies at the same time that follow you with the intent to kill you!
- if you survived through all this, at the fifth and final level you will encounter the boss, that has two standard attacks and a special deadly attack! Beat him and you beat the game!

<b>Video:</b><a href="" target="_blank"> A video demonstration. </a>

# Setup:
<b>Components (not including wires):</b>
- 1 Arduino UNO;
- 1 8x8 matrix;
- 1 MAX7219 Driver;
- 1 10k ohms resistor (for the Driver);
- 1 electrolytic capacitor of 10 μF;
- 1 ceramic capacitor of 104 pF;
- 1 LCD screen;
- 1 220 ohms resistor (for the LCD);
- 1 passive buzzer;
- 1 100 ohms resistor (for the buzzer);
- 1 joystick.

<details>
  <summary>Pictures</summary>
![signal-2021-12-26-020334_001](https://user-images.githubusercontent.com/61534490/147395861-a42232b8-db18-4759-b25b-5c05fdb6a2c1.jpeg)
![signal-2021-12-26-020334_002](https://user-images.githubusercontent.com/61534490/147395863-40552ba9-d245-4a6d-856d-d92363f4f82f.jpeg)
![signal-2021-12-26-020334_003](https://user-images.githubusercontent.com/61534490/147395865-11419652-8777-4272-bca0-8bb347942e74.jpeg)
</details>

# Stage progress:
Stage 1:
- I added the main menu with:
  - start;
  - score;
  - settings (button only);
  - about;
- You can play the first level;
- The highest score will be saved in EEPROM;
- The about section shows the game name, the author and has a link to this Github.

Stage 2:
- I added the settings menu with:
  - name settings;
  - matrix brightness settings;
  - LCD contrast settings;
- I added the ability to exit About early;
- I added a second level, with random generated and moving enemies;
- While playing, the LCD will show the level and player health.

Stage 3:
- I added a buzzer and various sounds (a start melody, the boss theme, joystick movement...);
- I added the second and third highest scores and all scores are now saved in EEPROM;
- I added a matrix animation for each menu button, as well as a start level, done level and enemy spawn animation and a projectile animation;
- I added the LCD brightness, I updated the values of brightness and contrast that the player can set and now all values are saved in EEPROM;
- you now set your name before starting lvl 1;
- I added a special screen for setting a new score (1st, 2nd or 3rd);
- I added a set time for each level, in witch the player can shoot as many enemies as possible for the highest score;
- to complement the timer, each level will now generate new enemies upon the older ones dying;
- I fixed the second level ghost enemy;
- I added the third level, in witch enemies fire back;
- I added the forth level, with more enemies that follow you;
- I added the fifth and final level, a boss fight, with two standard attacks and a super attack;
- I fixed the 0 Hp bug, witch would not display the main menu.

Stage 4 (final):
- I fixed the remaining bug of the boss fight;
- I added a special screen upon finnishing the game.
