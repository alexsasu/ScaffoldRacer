#pragma once
#include "Consts.h"
#include "Controls.h"
#include "GameAssets.h"
#include "Leaderboard.h"
#include "Settings.h"

class Game {
  private:
    // Game states
    bool gameLoaded;
    bool gameInProgress;
    bool gameFinished;

    // Game mechanics
    bool recentGameLayoutInit;
    bool beginNewLife;
    bool readySetGoOccured;
    byte matrix[matrixSize][matrixSize];
    unsigned long scaffoldsLastMoveTime;
    int scaffoldsMoveInterval;
    byte lastScaffoldGeneratedPos;
    char lastScaffoldGeneratedSide;
    byte lastScaffoldGeneratedPlayerStandingPos;
    char lastScaffoldGeneratedPlayerStandingSide;    
    byte minScaffoldLength;
    byte maxScaffoldLength;
    byte nextGeneratedScaffoldLength;
    byte leftSideScaffoldsLengths[matrixSize];
    byte rightSideScaffoldsLengths[matrixSize];
    byte leftSideScaffoldsLastPlayerStandingLengths[matrixSize];
    byte rightSideScaffoldsLastPlayerStandingLengths[matrixSize];
    byte minScaffoldsDistance;
    byte maxScaffoldsDistance;
    byte nextGeneratedScaffoldDistance;
    bool generatedBox;
    unsigned long lastAddPointsTime;
    byte currentDifficultyPoints;
    int playerLine;
    int playerCol;
    int lastPlayerLine;
    int lastPlayerCol;
    int lastPlayerStandingLine;
    int lastPlayerStandingCol;
    byte playerLedState;
    bool playerJustJumped;
    unsigned long lastPlayerBlinkTime;
    unsigned long lastPlayerMoveTime;
    unsigned long lastPlayerJumpTime;
    unsigned long lastPlayerFallTime;
    int playerFallInterval;
    bool layoutChanged;

    // In-game stats
    char playerName[playerNameLength + 1];
    char difficulty;
    int score;
    byte lives;

    // Post game info
    int currentInfo;
    byte currentInfoLcdRow;
    bool postGameInfoDisplayed;
    bool postGameInfoInit;
    char postGameInfo[postGameInfoLinesCount][postGameInfoMaxLettersPerLine + 1];
    int playersBestedCount;

  public:
    Game();

    bool getGameLoaded();
    bool getGameInProgress();
    bool getGameFinished();

    void displayGameStats(GameAssets&);
    void initializePostGameInfo(int);
    void showPostGameInfo(GameAssets&, Controls&, Sounds&);

    void loadGame(GameAssets&, Settings&);
    void prepareToBegin(Controls&, Sounds&);
    void carryOutGame(GameAssets&, Controls&, Sounds&);
    void endGame(GameAssets&, Leaderboard&, Controls&, Sounds&);

    bool checkScoreAndAttemptToIncreaseDifficulty();

    void decideCurrentDifficultyPoints();

    void gameInitScaffoldsLayout();
    void attemptToMoveScaffoldsUp();
    void attemptToGenerateScaffold();
    void decideScaffoldsMoveInterval();
    void setScaffoldsLengthRange();
    void setScaffoldsDistanceRange();
    byte decideNextGeneratedScaffoldLength();
    byte decideNextGeneratedScaffoldDistance();

    void attemptToGenerateBox();
    
    void highlightPlayer(GameAssets&);
    void attemptToExecutePlayerFalling();
    void attemptToMovePlayer(int);
    void attemptToUpdateLastPlayerStandingPos();
    bool checkIfPlayerLostLife(GameAssets&, Sounds&);

    void displayLayout(GameAssets&);
};