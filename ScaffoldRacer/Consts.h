#pragma once
#include "Arduino.h"


// Consts for the game
//------------------------------------------------------
const byte seedGenerator = A4;
const byte maxLivesCount = 3;

const int maxPossibleScore = 9999;

const int addPointsInterval = 1000;
const byte easyDifficultyPoints = 5;
const int mediumDifficultyThreshold = 125;
const byte mediumDifficultyPoints = 10;
const int hardDifficultyThreshold = 420;
const byte hardDifficultyPoints = 25;

const int easyDifficultyScaffoldsMoveInterval = 600;
const int mediumDifficultyScaffoldsMoveInterval = 350;
const int hardDifficultyScaffoldsMoveInterval = 225;

const byte boxGenerationProbabIntervalLowerBound = 0;
const byte boxGenerationProbabIntervalUpperBound = 1;

const byte playerBlinkInterval = 150;
const byte playerMoveInterval = 100;
const int playerGoBackDownInterval = 250;
const byte normalPlayerFallingSpeed = 200;
const byte acceleratedPlayerFallingSpeed = 100;

const byte postGameInfoLinesCount = 7;
const byte postGameInfoMaxLettersPerLine = 14;
const unsigned int endGameNotificationDelay = 3000;
//------------------------------------------------------


// Consts for the EEPROM
//------------------------------------------------------
const int eepromLcdBrightnessAddress = 0;
const int eepromMatrixLedIntensityAddress = 1;
const int eepromBuzzerSoundAddress = 2;
const int eepromHighscoresCountAddress = 100;
//------------------------------------------------------



// Consts for the LCD
//------------------------------------------------------
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 3;
const byte d6 = 5;
const byte d7 = 4;

const byte lcdBrightnessPin = 6;

const byte minLcdBrightness = 51;
const byte maxLcdBrightness = 255;
const byte minLcdMappedBrightness = 1;
const byte maxLcdMappedBrightness = 5;
const byte lcdBrightnessIncrement = 51;

const byte lcdBlockHeight = 8;
const byte lcdRowLength = 16;
const byte lcdFirstRow = 0;
const byte lcdSecondRow = 1;

const byte lcdUpArrowIconAddr = 0;
const byte lcdUpArrowIcon[lcdBlockHeight] = {0b00000, 0b00000, 0b00000, 0b00100, 0b01110, 0b11111, 0b11111, 0b11111};
const byte lcdDownArrowIconAddr = 1;
const byte lcdDownArrowIcon[lcdBlockHeight] = {0b11111, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000, 0b00000, 0b00000};

const byte mainMenuOptionsCount = 5;
const char* const mainMenuOptions[] PROGMEM = {"Start game", "Instructions", "Leaderboard", "Settings", "Credits"};

const byte gameStatsLivesIconAddr = 2;
const byte gameStatsLivesIcon[lcdBlockHeight] = {0b00000, 0b01010, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000, 0b00000};
const byte gameStatsDifficultyIconAddr = 3;
const byte gameStatsDifficultyIcon[lcdBlockHeight] = {0b00000, 0b01110, 0b10101, 0b11011, 0b01110, 0b01110, 0b00000, 0b00000};

const char instructionsString[] PROGMEM = "Move the JS left and right to traverse the scaffolds. Move the JS up to jump one block. Move the JS down to speed up the player's falling. Press the JS to conclude your run earlier. Get over obstacles generated on scaffolds. If the player is taken above the screen by any of the scaffolds, or if they fall to the bottom of the screen without landing on a scaffold, they lose a life. While inside any sub-menu, move the JS left to go back to the previous menu.";
const int instructionsLength = strlen(instructionsString);
const byte instructionCharactersPerLine = 13;
const byte instructionLinesCount = (instructionsLength / instructionCharactersPerLine) + (!(instructionsLength % instructionCharactersPerLine)? 0 : 1);

const int maxHighscoresCount = 10;

const byte settingsOptionsCount = 6;
const char* const settingsOptions[] PROGMEM = {"Player", "Level", "Del. hiscores", "LCD bright.", "Mat bright.", "Sounds"};

const byte playerNameLength = 5;

const byte difficultyOptionsCount = 3;
const byte difficultyOptionMaxLength = 6;
const char difficultyOptions[difficultyOptionsCount][lcdRowLength] = {"Easy  ", "Medium", "Hard  "};

const unsigned int resetHighscoresMessageDuration = 2500;

const byte creditsInfoCount = 3;
const byte creditsInfoMaxLength = 25;
const char creditsInfoTypes[creditsInfoCount][lcdRowLength] = {"Game", "Dev", "GitHub"};
const char creditsInfo[creditsInfoCount][creditsInfoMaxLength] = {"Scaffold Racer ", "Alexandru Sasu ", "github.com/alexsasu "};
const unsigned int scrollDelay = 500;
//------------------------------------------------------


// Consts for the LED matrix
//------------------------------------------------------
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;

const byte driverNo = 1;

const byte matrixSize = 8;

const byte minMatrixIntensity = 1;
const byte maxMatrixIntensity = 13;
const byte minMatrixMappedIntensity = 1;
const byte maxMatrixMappedIntensity = 5;
const byte matrixIntensityIncrement = 3;

const byte matrixIconBytesCount = 8;
const uint64_t matrixStartGameIcon = 0x0204062d58367020;
const uint64_t matrixInstructionsIcon = 0x181800183860663c;
const uint64_t matrixLeaderboardIcon = 0x3c18183cffbdff3c;
const uint64_t matrixSettingsIcon = 0x06050f1c38f0a060;
const uint64_t matrixCreditsIcon = 0xfefe7c287c547c38;
//------------------------------------------------------


// Consts for the joystick
//------------------------------------------------------
const byte joyXPin = A0;
const byte joyYPin = A1;
const byte joySWPin = A2;

const unsigned int joySWDebounceDelay = 100;

const int joyMinThreshold = 400;
const int joyMaxThreshold = 600;
const byte joyMoveInterval = 100;
const unsigned int joySensitivityBypassAmount = 50;
//------------------------------------------------------


// Consts for the buzzer
//------------------------------------------------------
const byte buzzerPin = A5;

const int buzzerMenuNavFreq = 250;
const int buzzerMenuNavOnDuration = 100;
const int buzzerMenuSelectOptionFreq = 300;
const int buzzerMenuSelectOptionOnDuration = 50;

const int buzzerIncreaseDifficultyFreq = 150;
const int buzzerIncreaseDifficultyOnDuration = 100;

const int buzzerPlayerReadyFreq = 500;
const int buzzerPlayerReadyOnDuration = 150;
const int buzzerPlayerSetFreq = 700;
const int buzzerPlayerSetOnDuration = 150;
const int buzzerPlayerGoFreq = 900;
const int buzzerPlayerGoOnDuration = 750;
const int buzzerPlayerLoseLifeSound1Freq = 200;
const int buzzerPlayerLoseLifeSound1OnDuration = 200;
const int buzzerPlayerLoseLifeSound2Freq = 100;
const int buzzerPlayerLoseLifeSound2OnDuration = 700;
//------------------------------------------------------