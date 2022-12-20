# ScaffoldRacer

## About

This repository presents a game - named ScaffoldRacer - developed in Arduino, as a project during the Introduction to Robotics course taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest.

## Project Details 📄

<details>
<summary><h2>Functionality Details 📑</h2></summary>
<br>

The project features a game displayed on an 8x8 LED matrix, as well as a menu for said-game, displayed through an LCD.

### Game Details 🚧🏃‍♂️

**Story:** You are playing as a parkour enjoyer, that loves nothing more than to partake in brisk, long, running sessions. As you stroll around the city, you find yourself near a construction site for a very tall building, and decide to put your skills to use by climbing the whole thing to the top. After reaching the top, you suddenly remember that you forgot to finish your project for the robotics course at your university, and so you decide to go and finish it, by hurrying back down and descending on some nearby scaffolds, while doing your daily running session. Be careful, though, there are construction props laid on the scaffolds, that will try to thwart your descent, or worse yet, be careful not to off the scaffolds!

**Gameplay mechanics:** Move the joystick in any direction to determine your descent on a neverending stream of randomly generated platforms, that display different properties depending on the current difficulty of the game. The speed at which the platforms move, their lengths, and the distance between them, will make the player's run increasingly harder as the current difficulty of the game advances (Easy, Medium, Hard). The difficulty of the game rises as you accumulate more points. Point accumulation is done constantly, by progressing through the game. Obstacles will appear, only on the "Hard" difficulty. Avoid getting slowed down by obstacles placed on the scaffolds. Each time you start a new game, you begin with 3 lives. If you are taken above the matrix, by scaffolds or obstacles, or if you fall to the bottom of the matrix without landing on a scaffold or an obstacle, you lose a life. Once all lives are spent, or once the player presses the joystick mid-game, it's game over and you are taken on the LCD to the post-game info.

### Menu Details 📃

**Navigation properties:** Move the joystick up or down to navigate through the options of the menu. Press the joystick to access a sub-menu, or interact with certain options of the menu. Move the joystick left to exit any sub-menu.

**Menu options properties:**
* Start game: starts a new game
* Instructions: details about how to play the game and navigate the menu
* Leaderboard: top 10 scores, in descending order
* Settings: modify the properties of the game, leaderboard, matrix, LCD (press the joystick to lock on a certain option, and then move it up, down, left, or right, to interact with the option, or press the joystick again to end your lock)
  * Player: name of the current player
  * Level: starting difficulty of the game (Easy, Medium, Hard)
  * Del. hiscores: resets the leaderboard
  * LCD bright.: lowers or increases the LCD brightness (1-5 steps)
  * Mat bright.: lowers or increases the matrix intensity (1-5 steps)
  * Sounds: ON/OFF (applies both to the game, and the menu navigation)
* Credits: displays the name of the game, and the name and GitHub profile link of the person that made the game

### Other Details 🎛
* The buzzer attempts to generate sounds (when the sound is enabled ("ON") in the "Settings" sub-menu) in the following scenarios: the user utilizes the joystick to navigate the menu or interact with it, the player begins a new life, the player loses a life, the difficulty is increased during a game
* The potentiometer can be used to modify the contrast of the LCD

</details>

<details>
<summary><h2>Backstory 📖</h2></summary>
<br>

The game's idea wasn't inspired from a particular game, but rather it came to me naturally while I was thinking about the theme of the project. Given that I like the types of games that go on forever, where the player's score is constantly increasing, I found it engaging and addicting to play such a game, so I began developing one around these aspects, in the classical form of a platformer.

### Difficulties Encountered 😕
* Making it so that the player's movement doesn't generate any bugs
* Making it so that, after the player loses a life and respawns, the layout of the matrix is the same as the one when the player last stood on a solid surface, or a layout in which the player can continue their descent, without having their path blocked by an obstacle (impossible for them to overcome it, due to a game bug)
* Generating obstacles on the platforms

</details>

<details>
<summary><h2>Project Requirements 📋</h2></summary>

### Menu Requirements
* When powering up a game, a greeting message should be shown for a few moments.
* Should contain roughly the following categories:
  * Start game, starts the initial level of the game
  * Highscore:
    * Initially, there are 0 highscores.
    * Updated when the game is done. Highest possible score should be achieved by starting at a higher level.
    * Saves the top 5+ values in EEPROM with name and score.
  * Settings:
    * Enter name. The name should be shown in highscore. Maybe somewhere else, as well?
    * Starting level: Set the starting level value. The idea is to be able to start from a higher level as well. Can be replaced with difficulty.
    * LCD contrast control (optional, it replaces the potentiometer). Save it to EEPROM.
    * LCD brightness control (mandatory, must change LED wire that’s directly connected to 5v). Save it to EEPROM.
    * Matrix brightness control (see function setIntesnity from the ledControl library). Save it to EEPROM.
    * Sounds ON or OFF. Save it to EEPROM.
    * Extra stuff can include items specific to the game mechanics, or other settings such as chosen theme song, etc. Again, save it to EEPROM.
  * About: should include details about the creator(s) of the game. At least game name, author, and GitHub link or user. (use scrolling text?)
  * How to play: short and informative description.
* While playing the game: display all relevant info
  * Lives
  * Level
  * Score
  * Time?
  * Player name?
  * etc
* Upon game ending:
  * Screen 1: a message such as "Congratulations on reaching level/score X". "You did better than y people.". etc. Switches to Screen 2 upon interaction (button press) or after a few moments.
  * Screen 2: display relevant game info: score, time, lives left, etc. Must inform the player if they beat the highscore. This menu should only be closed by the player, by pressing a button.
> Note: The menu should be intuitive and obvious how to use.

### Game Requirements

* It must have basic sounds (when "eating" food, when dying, when finishing the level, etc). Extra: add theme songs.
* It must be intuitive and fun to play.
* It must make sense in the current setup. Study the idea of a panning camera - aka the 8x8 led doesn’t need to be the entire map. It can only be the current field of view of the player.
* The levels must progress dynamically. Aka the level difficulty, score and other properties should be a function of the level number or time. However, the challenge here is for it to grow in a playable way - not too easy for a long time so it becomes boring, not too hard too fast so it’s not attractive. Also, it’s nice to have an ending, a final level, a boss etc. It shouldn’t necessarily go on forever (but that’s fine, though).

### Hardware Requirements

* 1 Arduino Uno
* 1 LCD
* 1 8x8 LED matrix
* 1 joystick
* 1 active/passive buzzer

</details>

<details>
<summary><h2>Hardware Used ⚙</h2></summary>
<br>

* 1 Arduino Uno
* 1 LCD
* 1 1088AS matrix
* 1 MAX7219 LED Driver
* 1 joystick
* 1 passive buzzer
* 1 potentiometer (for controlling the LCD contrast)
* 1 10μF, 50V electrolytic capacitor
* 1 (104) 0.1μF ceramic capacitor
* resistors and wires (per logic)
* 3 breadboards

</details>

<details>
<summary><h2>Showcase 🎮</h2></summary>

### Setup Pictures 📷

![above](https://user-images.githubusercontent.com/87432371/208773331-7c3f99f7-4bb4-4d4b-94bc-1eeddd21c71e.jpg)

![from behind](https://user-images.githubusercontent.com/87432371/208773348-06620a5e-f52f-4ada-97f5-aad407ba3293.jpg)

![from the front](https://user-images.githubusercontent.com/87432371/208773357-a39b8339-62b4-4e65-a022-8946003f6305.jpg)

</details>
