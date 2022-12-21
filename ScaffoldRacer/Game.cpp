#include "Consts.h"
#include "Game.h"
#include "GameAssets.h"
#include "LCDUtils.h"
#include "Macros.h"
#include "MatrixUtils.h"

Game::Game() {
  gameLoaded = false;
  gameInProgress = false;
}

bool Game::getGameLoaded() {
  return gameLoaded;
}

bool Game::getGameInProgress() {
  return gameInProgress;
}

bool Game::getGameFinished() {
  return gameFinished;
}

void Game::loadGame(GameAssets &gameAssets, Settings &settings) {
  gameAssets.lcdClear();
  gameAssets.matrixClearDisplay(0);
  gameAssets.lcdSetCursor(6, 1);
  gameAssets.lcdPrint("loading...");

  // Animation for loading the game
  for (int row = 0; row < matrixSize; ++row) {
    for (int col = 0; col < matrixSize; ++col) {
      gameAssets.matrixSetLed(0, row, col, true);
      delay(20);
    }
  }

  LCDUtils::initializeGameStatsIcons(gameAssets);

  // Game stats initialization
  strcpy(playerName, settings.getPlayerName());
  difficulty = settings.getDifficulty()[0];
  score = 0;
  lives = maxLivesCount;

  // Game mechanics initialization
  for (int row = 0; row < matrixSize; ++row) {
    for (int col = 0; col < matrixSize; ++col) {
      if (col == 0 || col == matrixSize - 1) {
        matrix[row][col] = 1;
      } else {
        matrix[row][col] = 0;
      }
    }
  }
  decideCurrentDifficultyPoints();
  for (int scaffold = 0; scaffold < matrixSize; ++scaffold) {
    leftSideScaffoldsLengths[scaffold] = rightSideScaffoldsLengths[scaffold] = 0;
  }
  decideScaffoldsMoveInterval();
  setScaffoldsLengthRange();
  setScaffoldsDistanceRange();
  playerLine = 0;
  playerCol = 1;
  matrix[playerLine][playerCol] = 1;
  lastPlayerLine = 0;
  lastPlayerCol = 1;
  lastPlayerStandingLine = 0;
  lastPlayerStandingCol = 1;
  playerLedState = 1;
  playerJustJumped = false;
  lastPlayerBlinkTime = 0;
  lastPlayerMoveTime = 0;
  lastPlayerJumpTime = 0;
  lastPlayerFallTime = 0;
  playerFallInterval = normalPlayerFallingSpeed;
  beginNewLife = true;
  readySetGoOccured = false;
  randomSeed(analogRead(seedGenerator));
  gameInitScaffoldsLayout();

  // Post game info mechanics initialization
  postGameInfoDisplayed = false;
  postGameInfoInit = false;
  playersBestedCount = 0;

  // Animation for finishing the loading
  for (int row = 0; row < matrixSize; ++row) {
    for (int col = 0; col < matrixSize; ++col) {
      if (!matrix[row][col]) {
        gameAssets.matrixSetLed(0, row, col, false);
      }
      delay(20);
    }
  }

  gameLoaded = true;
  gameInProgress = true;
  gameFinished = false;
  gameAssets.lcdClear();
}

bool Game::checkScoreAndAttemptToIncreaseDifficulty() {
  if (difficulty == 'E') {
    if (score >= mediumDifficultyThreshold) {
      difficulty = 'M';
      decideCurrentDifficultyPoints();
      decideScaffoldsMoveInterval();
      setScaffoldsLengthRange();
      setScaffoldsDistanceRange();

      return true;
    }
  } else if (difficulty == 'M') {
    if (score >= hardDifficultyThreshold) {
      difficulty = 'H';
      decideCurrentDifficultyPoints();
      decideScaffoldsMoveInterval();
      setScaffoldsLengthRange();
      setScaffoldsDistanceRange();
      
      return true;
    }
  }

  return false;
}

void Game::gameInitScaffoldsLayout() {
  leftSideScaffoldsLengths[1] = decideNextGeneratedScaffoldLength();
  for (int i = 1; i <= leftSideScaffoldsLengths[1]; ++i) {
    matrix[1][i] = 1;
  }
  lastScaffoldGeneratedPos = 1;
  lastScaffoldGeneratedSide = 'L';

  decideNextGeneratedScaffoldDistance();

  while ((matrixSize - 1) - lastScaffoldGeneratedPos > nextGeneratedScaffoldDistance) {
    if (lastScaffoldGeneratedSide == 'L') {
      rightSideScaffoldsLengths[lastScaffoldGeneratedPos + (nextGeneratedScaffoldDistance + 1)] = decideNextGeneratedScaffoldLength();
      for (int i = 1; i <= rightSideScaffoldsLengths[lastScaffoldGeneratedPos + (nextGeneratedScaffoldDistance + 1)]; ++i) {
        matrix[lastScaffoldGeneratedPos + (nextGeneratedScaffoldDistance + 1)][(matrixSize - 1) - i] = 1;
      }
      lastScaffoldGeneratedPos += (nextGeneratedScaffoldDistance + 1);
      lastScaffoldGeneratedSide = 'R';
    } else if (lastScaffoldGeneratedSide == 'R') {
      leftSideScaffoldsLengths[lastScaffoldGeneratedPos + (nextGeneratedScaffoldDistance + 1)] = decideNextGeneratedScaffoldLength();
      for (int i = 1; i <= leftSideScaffoldsLengths[lastScaffoldGeneratedPos + (nextGeneratedScaffoldDistance + 1)]; ++i) {
        matrix[lastScaffoldGeneratedPos + (nextGeneratedScaffoldDistance + 1)][i] = 1;
      }
      lastScaffoldGeneratedPos += (nextGeneratedScaffoldDistance + 1);
      lastScaffoldGeneratedSide = 'L';
    }

    decideNextGeneratedScaffoldDistance();
  }

  recentGameLayoutInit = true;
  layoutChanged = true;
}

void Game::decideCurrentDifficultyPoints() {
  if (difficulty == 'E') {
    currentDifficultyPoints = easyDifficultyPoints;
  } else if (difficulty == 'M') {
    currentDifficultyPoints = mediumDifficultyPoints;
  } else if (difficulty == 'H') {
    currentDifficultyPoints = hardDifficultyPoints;
  }
}

void Game::decideScaffoldsMoveInterval() {
  if (difficulty == 'E') {
    scaffoldsMoveInterval = easyDifficultyScaffoldsMoveInterval;
  } else if (difficulty == 'M') {
    scaffoldsMoveInterval = mediumDifficultyScaffoldsMoveInterval;
  } else if (difficulty == 'H') {
    scaffoldsMoveInterval = hardDifficultyScaffoldsMoveInterval;
  }
}

void Game::setScaffoldsLengthRange() {
  if (difficulty == 'E') {
    minScaffoldLength = 2;
    maxScaffoldLength = 3;
  } else if (difficulty == 'M') {
    minScaffoldLength = 2;
    maxScaffoldLength = 5;
  } else if (difficulty == 'H') {
    minScaffoldLength = 2;
    maxScaffoldLength = 4;
  }
}

void Game::setScaffoldsDistanceRange() {
  if (difficulty == 'E') {
    minScaffoldsDistance = 1;
    maxScaffoldsDistance = 3;
  } else if (difficulty == 'M') {
    minScaffoldsDistance = 1;
    maxScaffoldsDistance = 2;
  } else if (difficulty == 'H') {
    minScaffoldsDistance = 1;
    maxScaffoldsDistance = 2;
  }
}

byte Game::decideNextGeneratedScaffoldLength() {
  nextGeneratedScaffoldLength = random(minScaffoldLength, maxScaffoldLength + 1);
  return nextGeneratedScaffoldLength;
}

byte Game::decideNextGeneratedScaffoldDistance() {
  nextGeneratedScaffoldDistance = random(minScaffoldsDistance, maxScaffoldsDistance + 1);
  return nextGeneratedScaffoldDistance;
}

void Game::attemptToMoveScaffoldsUp() {
  if (millis() - scaffoldsLastMoveTime > scaffoldsMoveInterval) {
    if (recentGameLayoutInit) {
      recentGameLayoutInit = false;
      scaffoldsLastMoveTime = millis();
    } else {
      matrix[playerLine][playerCol] = 0; // Turn off the player led so that it doesn't get mixed up with the scaffolds' ascension

      for (int row = 0; row < matrixSize - 1; ++row) {
        leftSideScaffoldsLengths[row] = leftSideScaffoldsLengths[row + 1];
        if (leftSideScaffoldsLengths[row] > 100) { // Box coordinates are stored instead of a scaffold length
          leftSideScaffoldsLengths[row] -= 10;
        }
        rightSideScaffoldsLengths[row] = rightSideScaffoldsLengths[row + 1];
        if (rightSideScaffoldsLengths[row] > 100) { // Box coordinates are stored instead of a scaffold length
          rightSideScaffoldsLengths[row] -= 10;
        }
        for (int col = 1; col < matrixSize - 1; ++col) {
          matrix[row][col] = matrix[row + 1][col];
        }
      }
      leftSideScaffoldsLengths[matrixSize - 1] = rightSideScaffoldsLengths[matrixSize - 1] = 0;
      for (int col = 1; col < matrixSize - 1; ++col) {
        matrix[matrixSize - 1][col] = 0;
      }
      --lastScaffoldGeneratedPos;

      if (matrix[playerLine][playerCol]) { // The player was previously standing on a solid surface
        lastPlayerLine = playerLine;
        lastPlayerCol = playerCol;
        --playerLine;
        if (playerLine >= 0) {
          matrix[playerLine][playerCol] = playerLedState;
        }
      } else { // The player was previously standing in mid-air
        matrix[playerLine][playerCol] = playerLedState;
      }

      layoutChanged = true;
      scaffoldsLastMoveTime = millis();
    }
  }
}

void Game::attemptToGenerateScaffold() {
  if ((matrixSize - 1) - lastScaffoldGeneratedPos > nextGeneratedScaffoldDistance) {
    decideNextGeneratedScaffoldLength();

    attemptToGenerateBox();

    if (lastScaffoldGeneratedSide == 'L') {
      rightSideScaffoldsLengths[matrixSize - 1] = nextGeneratedScaffoldLength;
      for (int i = 1; i <= rightSideScaffoldsLengths[matrixSize - 1]; ++i) {
        matrix[matrixSize - 1][(matrixSize - 1) - i] = 1;
      }
      lastScaffoldGeneratedPos = matrixSize - 1;
      lastScaffoldGeneratedSide = 'R';
    } else if (lastScaffoldGeneratedSide == 'R') {
      leftSideScaffoldsLengths[matrixSize - 1] = nextGeneratedScaffoldLength;
      for (int i = 1; i <= leftSideScaffoldsLengths[matrixSize - 1]; ++i) {
        matrix[matrixSize - 1][i] = 1;
      }
      lastScaffoldGeneratedPos = matrixSize - 1;
      lastScaffoldGeneratedSide = 'L';
    }

    layoutChanged = true;

    decideNextGeneratedScaffoldDistance();
  }
}

void Game::attemptToGenerateBox() {
  if (difficulty == 'H') {
    byte probabilityToGenerateBox = random(boxGenerationProbabIntervalLowerBound, boxGenerationProbabIntervalUpperBound + 1);
    if (!probabilityToGenerateBox) {
      char nextScaffoldGeneratedSide = (lastScaffoldGeneratedSide == 'L') ? 'R' : 'L';
      bool enoughSpace = false;
      bool enoughLengthCovered = false;
      byte lengthCovered;

      if (nextGeneratedScaffoldDistance >= 2) {
        enoughSpace = true;
      }
      if (lastScaffoldGeneratedSide == 'L') {
        if (nextGeneratedScaffoldLength + leftSideScaffoldsLengths[lastScaffoldGeneratedPos] <= 6) {
          enoughLengthCovered = true;
          lengthCovered = nextGeneratedScaffoldLength + leftSideScaffoldsLengths[lastScaffoldGeneratedPos];
        }
      } else if (lastScaffoldGeneratedSide == 'R') {
        if (nextGeneratedScaffoldLength + rightSideScaffoldsLengths[lastScaffoldGeneratedPos] <= 6) {
          enoughLengthCovered = true;
          lengthCovered = nextGeneratedScaffoldLength + rightSideScaffoldsLengths[lastScaffoldGeneratedPos];
        }
      }

      if (enoughSpace) {
        byte boxPosOnScaffold = random(1, nextGeneratedScaffoldLength + 1);

        if (nextScaffoldGeneratedSide == 'L') {
          leftSideScaffoldsLengths[matrixSize - 2] = 100 + (matrixSize - 2) * 10 + boxPosOnScaffold; // We retain the line and column of the box in a 3 digit number, where the first digit specifies that we store a box instead of a scaffold length, and the second and third digit are the line and column of the box
          matrix[matrixSize - 2][boxPosOnScaffold] = 1;
        } else if (nextScaffoldGeneratedSide == 'R') {
          rightSideScaffoldsLengths[matrixSize - 2] = 100 + (matrixSize - 2) * 10 + ((matrixSize - 1) - (nextGeneratedScaffoldLength + 1) + boxPosOnScaffold); // We retain the line and column of the box in a 3 digit number, where the first digit specifies that we store a box instead of a scaffold length, and the second and third digit are the line and column of the box
          matrix[matrixSize - 2][(matrixSize - 1) - (nextGeneratedScaffoldLength + 1) + boxPosOnScaffold] = 1;
        }

        layoutChanged = true;
        generatedBox = true;
      } else if (enoughLengthCovered) {
        byte boxPosOnScaffold = (lengthCovered == 6) ?
          random(1, nextGeneratedScaffoldLength) :
          random(1, nextGeneratedScaffoldLength + 1);
        
        if (nextScaffoldGeneratedSide == 'L') {
          leftSideScaffoldsLengths[matrixSize - 2] = 100 + (matrixSize - 2) * 10 + boxPosOnScaffold; // We retain the line and column of the box in a 3 digit number, where the first digit specifies that we store a box instead of a scaffold length, and the second and third digit are the line and column of the box
          matrix[matrixSize - 2][boxPosOnScaffold] = 1;
        } else if (nextScaffoldGeneratedSide == 'R') {
          if (lengthCovered == 6) {
            ++boxPosOnScaffold;
          }
          rightSideScaffoldsLengths[matrixSize - 2] = 100 + (matrixSize - 2) * 10 + ((matrixSize - 1) - (nextGeneratedScaffoldLength + 1) + boxPosOnScaffold); // We retain the line and column of the box in a 3 digit number, where the first digit specifies that we store a box instead of a scaffold length, and the second and third digit are the line and column of the box
          matrix[matrixSize - 2][(matrixSize - 1) - (nextGeneratedScaffoldLength + 1) + boxPosOnScaffold] = 1;
        }
      }
    }
  }
}

void Game::carryOutGame(GameAssets &gameAssets, Controls &controls, Sounds &sounds) {
  if (checkScoreAndAttemptToIncreaseDifficulty()) {
    sounds.attemptToPlayIncreaseDifficultySound();
  }

  displayGameStats(gameAssets);

  if (readySetGoOccured) {
    highlightPlayer(gameAssets);
  }

  attemptToExecutePlayerFalling();
  if (layoutChanged) {
    displayLayout(gameAssets);
  }
  if (checkIfPlayerLostLife(gameAssets, sounds)) {
    if (lives > 0) {
      layoutChanged = true;
      displayLayout(gameAssets);
    } else {
      gameInProgress = false;
      gameFinished = true;
    }
  } else {
    attemptToUpdateLastPlayerStandingPos();

    if (beginNewLife) {
      prepareToBegin(controls, sounds);

      if (millis() - lastPlayerMoveTime > playerMoveInterval) {
        int joyMoveDirection = controls.processJoyPlayerMovement();
        attemptToMovePlayer(joyMoveDirection);
        if (layoutChanged) {
          displayLayout(gameAssets);
        }
        attemptToUpdateLastPlayerStandingPos();
      }
    } else {
      attemptToMoveScaffoldsUp();
      if (layoutChanged) {
        displayLayout(gameAssets);
      }
      if (checkIfPlayerLostLife(gameAssets, sounds)) {
        if (lives > 0) {
          layoutChanged = true;
          displayLayout(gameAssets);
        } else {
          gameInProgress = false;
          gameFinished = true;
        }
      } else {
        attemptToUpdateLastPlayerStandingPos();

        attemptToGenerateScaffold();
        if (layoutChanged) {
          displayLayout(gameAssets);
        }
        if (millis() - lastPlayerMoveTime > playerMoveInterval) {
          int joyMoveDirection = controls.processJoyPlayerMovement();
          attemptToMovePlayer(joyMoveDirection);
          if (layoutChanged) {
            displayLayout(gameAssets);
          }
          attemptToUpdateLastPlayerStandingPos();
        }
      }

      if (lives > 0 && millis() - lastAddPointsTime > addPointsInterval) {
        score += currentDifficultyPoints;
        if (score >= maxPossibleScore) {
          score = maxPossibleScore;
          gameInProgress = false;
          gameFinished = true;
        } else {
          lastAddPointsTime = millis();
        }
      }
    }
  }

  if (controls.joySWChangeState()) {
    gameInProgress = false;
    gameFinished = true;
  }
}

void Game::prepareToBegin(Controls &controls, Sounds &sounds) {
  if (!readySetGoOccured) {
    sounds.attemptToPlayBeginLifeSound();
    readySetGoOccured = true;
  }
}

void Game::highlightPlayer(GameAssets &gameAssets) {
  if (millis() - lastPlayerBlinkTime > playerBlinkInterval) {
    lastPlayerBlinkTime = millis();

    playerLedState = !playerLedState;
    matrix[playerLine][playerCol] = playerLedState;
    gameAssets.matrixSetLed(0, playerLine, playerCol, matrix[playerLine][playerCol]);
  }
}

void Game::attemptToExecutePlayerFalling() {
  if (millis() - lastPlayerFallTime > playerFallInterval) {
    if (millis() - lastPlayerJumpTime > playerGoBackDownInterval) {
      if (playerLine + 1 < matrixSize) {
        if (!matrix[playerLine + 1][playerCol]) {
          lastPlayerLine = playerLine;
          lastPlayerCol = playerCol;
          ++playerLine;
          matrix[lastPlayerLine][lastPlayerCol] = 0;
          matrix[playerLine][playerCol] = playerLedState;
          layoutChanged = true;

          if (beginNewLife && (playerLine != lastPlayerLine)) {
            beginNewLife = false;
            lastAddPointsTime = millis();
          }

          lastPlayerFallTime = millis();
        }
      }
    }
  }
}

void Game::attemptToMovePlayer(int joyMoveDirection) {
  if (joyMoveDirection == JOY_MOVED_UP && !playerJustJumped) {
    if (playerLine - 1 >= 0) {
      if (!matrix[playerLine - 1][playerCol] && matrix[playerLine + 1][playerCol]) {
        lastPlayerLine = playerLine;
        lastPlayerCol = playerCol;
        --playerLine;
        matrix[lastPlayerLine][lastPlayerCol] = 0;
        matrix[playerLine][playerCol] = playerLedState;
        layoutChanged = true;

        playerJustJumped = true;
        lastPlayerMoveTime = lastPlayerJumpTime = millis();
      }
    }
  }
  if (joyMoveDirection == JOY_MOVED_DOWN) {
    lastPlayerMoveTime = millis();
    playerFallInterval = acceleratedPlayerFallingSpeed;
  }
  if ((joyMoveDirection == JOY_MOVED_LEFT) || (joyMoveDirection == JOY_MOVED_UP_LEFT && playerJustJumped)) {
    if (!matrix[playerLine][playerCol - 1]) {
      lastPlayerLine = playerLine;
      lastPlayerCol = playerCol;
      --playerCol;
      matrix[lastPlayerLine][lastPlayerCol] = 0;
      matrix[playerLine][playerCol] = playerLedState;
      layoutChanged = true;

      lastPlayerMoveTime = millis();
    }
  }
  if ((joyMoveDirection == JOY_MOVED_RIGHT) || (joyMoveDirection == JOY_MOVED_UP_RIGHT && playerJustJumped)) {
    if (!matrix[playerLine][playerCol + 1]) {
      lastPlayerLine = playerLine;
      lastPlayerCol = playerCol;
      ++playerCol;
      matrix[lastPlayerLine][lastPlayerCol] = 0;
      matrix[playerLine][playerCol] = playerLedState;
      layoutChanged = true;

      lastPlayerMoveTime = millis();
    }
  }
  if (joyMoveDirection == JOY_MOVED_UP_LEFT && !playerJustJumped) {
    if (playerLine - 1 >= 0) {
      if (!matrix[playerLine - 1][playerCol - 1] && matrix[playerLine + 1][playerCol]) {
        lastPlayerLine = playerLine;
        lastPlayerCol = playerCol;
        --playerLine;
        --playerCol;
        matrix[lastPlayerLine][lastPlayerCol] = 0;
        matrix[playerLine][playerCol] = playerLedState;
        layoutChanged = true;
        
        playerJustJumped = true;
        lastPlayerMoveTime = lastPlayerJumpTime = millis();
      }
    }
  }
  if (joyMoveDirection == JOY_MOVED_UP_RIGHT && !playerJustJumped) {
    if (playerLine - 1 >= 0) {
      if (!matrix[playerLine - 1][playerCol + 1] && matrix[playerLine + 1][playerCol]) {
        lastPlayerLine = playerLine;
        lastPlayerCol = playerCol;
        --playerLine;
        ++playerCol;
        matrix[lastPlayerLine][lastPlayerCol] = 0;
        matrix[playerLine][playerCol] = playerLedState;
        layoutChanged = true;

        playerJustJumped = true;
        lastPlayerMoveTime = lastPlayerJumpTime = millis();
      }
    }
  }
  if (joyMoveDirection == JOY_MOVED_DOWN_LEFT) {
    if (!matrix[playerLine][playerCol - 1]) {
      lastPlayerLine = playerLine;
      lastPlayerCol = playerCol;
      --playerCol;
      matrix[lastPlayerLine][lastPlayerCol] = 0;
      matrix[playerLine][playerCol] = playerLedState;
      layoutChanged = true;

      playerFallInterval = acceleratedPlayerFallingSpeed;
      lastPlayerMoveTime = millis();
    }
  }
  if (joyMoveDirection == JOY_MOVED_DOWN_RIGHT) {
    if (!matrix[playerLine][playerCol + 1]) {
      lastPlayerLine = playerLine;
      lastPlayerCol = playerCol;
      ++playerCol;
      matrix[lastPlayerLine][lastPlayerCol] = 0;
      matrix[playerLine][playerCol] = playerLedState;
      layoutChanged = true;

      playerFallInterval = acceleratedPlayerFallingSpeed;
      lastPlayerMoveTime = millis();
    }
  }

  if (joyMoveDirection != JOY_MOVED_DOWN && joyMoveDirection != JOY_MOVED_DOWN_LEFT && joyMoveDirection != JOY_MOVED_DOWN_RIGHT) {
    playerFallInterval = normalPlayerFallingSpeed;
  }
  if (joyMoveDirection != JOY_MOVED_UP && joyMoveDirection != JOY_MOVED_UP_LEFT && joyMoveDirection != JOY_MOVED_UP_RIGHT) {
    playerJustJumped = false;
  }
}

void Game::attemptToUpdateLastPlayerStandingPos() {
  if (playerLine + 1 < matrixSize) {
    if (matrix[playerLine + 1][playerCol]) {
      lastPlayerStandingLine = playerLine;
      lastPlayerStandingCol = playerCol;

      for (int scaffold = 0; scaffold < matrixSize; ++scaffold) {
        leftSideScaffoldsLastPlayerStandingLengths[scaffold] = leftSideScaffoldsLengths[scaffold];
        rightSideScaffoldsLastPlayerStandingLengths[scaffold] = rightSideScaffoldsLengths[scaffold];

        if (leftSideScaffoldsLengths[scaffold] > 0 && !(leftSideScaffoldsLengths[scaffold] / 10)) {
          lastScaffoldGeneratedPlayerStandingSide = 'L';
          lastScaffoldGeneratedPlayerStandingPos = scaffold;
        } else if (rightSideScaffoldsLengths[scaffold] > 0 && !(rightSideScaffoldsLengths[scaffold] / 10)) {
          lastScaffoldGeneratedPlayerStandingSide = 'R';
          lastScaffoldGeneratedPlayerStandingPos = scaffold;
        }
      }
    }
  }
}

bool Game::checkIfPlayerLostLife(GameAssets &gameAssets, Sounds &sounds) {
  if (playerLine < 0 || playerLine >= matrixSize - 1) {
    gameAssets.lcdClear();
    gameAssets.lcdPrint("    You lost    ");
    gameAssets.lcdSetCursor(0, 1);
    gameAssets.lcdPrint("     a life!    ");

    sounds.attemptToPlayLoseLifeSound();

    --lives;

    gameAssets.lcdClear();
    displayGameStats(gameAssets);

    lastScaffoldGeneratedSide = lastScaffoldGeneratedPlayerStandingSide;
    lastScaffoldGeneratedPos = lastScaffoldGeneratedPlayerStandingPos;
    for (int scaffold = 0; scaffold < matrixSize; ++scaffold) {
      leftSideScaffoldsLengths[scaffold] = leftSideScaffoldsLastPlayerStandingLengths[scaffold];
      rightSideScaffoldsLengths[scaffold] = rightSideScaffoldsLastPlayerStandingLengths[scaffold];
    }

    for (int row = 0; row < matrixSize; ++row) {
      for (int col = 1; col < matrixSize - 1; ++col) {
        if (leftSideScaffoldsLengths[row] / 10) {
          if (col == leftSideScaffoldsLengths[row] % 10) {
            matrix[row][col] = 1;
          } else {
            matrix[row][col] = 0;
          }
        } else if (rightSideScaffoldsLengths[row] / 10) {
          if (col == rightSideScaffoldsLengths[row] % 10) {
            matrix[row][col] = 1;
          } else {
            matrix[row][col] = 0;
          }
        } else if ((col <= leftSideScaffoldsLengths[row]) && !(leftSideScaffoldsLengths[row] / 10)) {
          matrix[row][col] = 1;
        } else if ((col >= (matrixSize - 1) - rightSideScaffoldsLengths[row]) && !(rightSideScaffoldsLengths[row] / 10)) {
          matrix[row][col] = 1;
        } else {
          matrix[row][col] = 0;
        }
      }
    }

    if (playerLine < 0) {
      // If the player is blocked by a box after they were taken up and lost a life, we try to find the next valid position on a scaffold
      if ((leftSideScaffoldsLengths[lastPlayerStandingLine + 1] && !(leftSideScaffoldsLengths[lastPlayerStandingLine + 1] / 10) && (leftSideScaffoldsLengths[lastPlayerStandingLine] % 10 > lastPlayerStandingCol)) ||
          (rightSideScaffoldsLengths[lastPlayerStandingLine + 1] && !(rightSideScaffoldsLengths[lastPlayerStandingLine + 1] / 10) && (rightSideScaffoldsLengths[lastPlayerStandingLine] % 10 < lastPlayerStandingCol) && (rightSideScaffoldsLengths[lastPlayerStandingLine] % 10))) {
            for (int row = lastPlayerStandingLine; row < matrixSize - 1; ++row) {
              int colStart;
              int colStop;
              if (row == lastPlayerStandingLine) {
                if (leftSideScaffoldsLengths[lastPlayerStandingLine] % 10) {
                  colStart = leftSideScaffoldsLengths[lastPlayerStandingLine] % 10 + 1;
                  colStop = leftSideScaffoldsLengths[lastPlayerStandingLine + 1] + 1;

                  for (int col = colStart; col < colStop; ++col) {
                    if (matrix[row + 1][col]) {
                      playerLine = row;
                      playerCol = col;
                      lastPlayerLine = row;
                      lastPlayerCol = col;

                      row = matrixSize;
                      col = colStop;
                    }
                  }
                } else {
                  colStart = rightSideScaffoldsLengths[lastPlayerStandingLine] % 10 - 1;
                  colStop = (matrixSize - 1) - rightSideScaffoldsLengths[lastPlayerStandingLine + 1] - 1;

                  for (int col = colStart; col > colStop; --col) {
                    if (matrix[row + 1][col]) {
                      playerLine = row;
                      playerCol = col;
                      lastPlayerLine = row;
                      lastPlayerCol = col;

                      row = matrixSize;
                      col = colStop;
                    }
                  }
                }
              } else {
                colStart = matrixSize - 2;
                colStop = 0;

                if (leftSideScaffoldsLengths[lastPlayerStandingLine] % 10) {
                  for (int col = colStart; col > colStop; --col) {
                    if (matrix[row + 1][col]) {
                      playerLine = row;
                      playerCol = col;
                      lastPlayerLine = row;
                      lastPlayerCol = col;

                      row = matrixSize;
                      col = colStop;
                    }
                  }
                } else {
                  colStart = 1;
                  colStop = matrixSize - 1;

                  for (int col = colStart; col < colStop; ++col) {
                    if (matrix[row + 1][col]) {
                      playerLine = row;
                      playerCol = col;
                      lastPlayerLine = row;
                      lastPlayerCol = col;

                      row = matrixSize;
                      col = colStop;
                    }
                  }
                }
              }
            }
      } else { // The player is not blocked by a box after they were taken up and lost a life. Nothing special to treat here
        playerLine = lastPlayerStandingLine;
        playerCol = lastPlayerStandingCol;
        lastPlayerLine = lastPlayerStandingLine;
        lastPlayerCol = lastPlayerStandingCol;
      }
    } else { // The player fell to the bottom of the screen and lost a life. Nothing special to treat here
      playerLine = lastPlayerStandingLine;
      playerCol = lastPlayerStandingCol;
      lastPlayerLine = lastPlayerStandingLine;
      lastPlayerCol = lastPlayerStandingCol;
    }

    playerLedState = 1;
    matrix[playerLine][playerCol] = playerLedState;

    recentGameLayoutInit = true;
    readySetGoOccured = false;
    beginNewLife = true;

    return true;
  } else {
    return false;
  }
}

void Game::displayLayout(GameAssets &gameAssets) {
  for (int row = 0; row < matrixSize; ++row) {
    for (int col = 0; col < matrixSize; ++col) {
      gameAssets.matrixSetLed(0, row, col, matrix[row][col]);
    }
  }

  layoutChanged = false;
}

void Game::endGame(GameAssets &gameAssets, Leaderboard &leaderboard, Controls &controls, Sounds &sounds) {
  if (!postGameInfoDisplayed) {
    gameAssets.matrixClearDisplay(0);
    gameAssets.lcdClear();

    byte cursorCol = 3;
    byte cursorLine = 0;
    gameAssets.lcdSetCursor(cursorCol, cursorLine);
    gameAssets.lcdPrint("GAME OVER!");
    delay(endGameNotificationDelay);

    if (score == maxPossibleScore) {
      gameAssets.lcdClear();

      cursorCol = 0;
      cursorLine = 0;
      gameAssets.lcdSetCursor(cursorCol, cursorLine);
      gameAssets.lcdPrint("    MAX SCORE   ");
      cursorCol = 0;
      cursorLine = 1;
      gameAssets.lcdSetCursor(cursorCol, cursorLine);
      gameAssets.lcdPrint("       GG!      ");
      delay(endGameNotificationDelay);
    }

    leaderboard.readLeaderboardDataFromEeprom();
    playersBestedCount = leaderboard.attemptToUpdateLeaderboard(playerName, score);

    gameAssets.lcdClear();

    if (playersBestedCount > 0) {
      cursorCol = 3;
      cursorLine = 0;
      gameAssets.lcdSetCursor(cursorCol, cursorLine);
      gameAssets.lcdPrint("You bested");

      cursorCol = 2;
      cursorLine = 1;
      gameAssets.lcdSetCursor(cursorCol, cursorLine);
      char playersBestedCountString[int(log(playersBestedCount)) + 2];
      byte numericalBase = 10;
      itoa(playersBestedCount, playersBestedCountString, numericalBase);
      gameAssets.lcdPrint(playersBestedCountString);
      gameAssets.lcdPrint(" player(s)!");

      delay(endGameNotificationDelay);

      gameAssets.lcdClear();
    }

    postGameInfoDisplayed = true;
  } else {
    if (!postGameInfoInit) {
      initializePostGameInfo(playersBestedCount);
    } else {
      showPostGameInfo(gameAssets, controls, sounds);
    }
  }
}

void Game::displayGameStats(GameAssets &gameAssets) {
  gameAssets.lcdSetCursor(0, 0);
  gameAssets.lcdPrint("Player:");
  gameAssets.lcdPrint(playerName);

  gameAssets.lcdSetCursor(0, 1);
  char scoreString[MAX_SCORE_LENGTH + 1];
  byte numericalBase = 10;
  itoa(score, scoreString, numericalBase);
  gameAssets.lcdPrint("S:");
  gameAssets.lcdPrint(scoreString);
  
  gameAssets.lcdSetCursor(MAX_SCORE_LENGTH + 3, 1); // 3 = 2 (the "S:" string) + 1 (the space right after the score)

  LCDUtils::displayLcdIcon(gameStatsLivesIconAddr, gameAssets);
  gameAssets.lcdWrite(':');
  gameAssets.lcdWrite(48 + lives);

  gameAssets.lcdWrite(' ');

  LCDUtils::displayLcdIcon(gameStatsDifficultyIconAddr, gameAssets);
  gameAssets.lcdWrite(':');
  gameAssets.lcdWrite(difficulty);
}

void Game::initializePostGameInfo(int playersBestedCount) {
  if (playersBestedCount != -1) {
    strcpy(postGameInfo[0], "      NEW     ");
    strcpy(postGameInfo[1], "   HIGHSCORE  ");
  } else {
    strcpy(postGameInfo[0], "     NO NEW   ");
    strcpy(postGameInfo[1], "  HIGHSCORE :(");
  }

  strcpy(postGameInfo[2], "Your stats:   ");

  strcpy(postGameInfo[3], "*Score: ");
  char scoreString[MAX_SCORE_LENGTH + 1];
  byte numericalBase = 10;
  itoa(score, scoreString, numericalBase);
  strcat(postGameInfo[3], scoreString);

  strcpy(postGameInfo[4], "*Lives left: ");
  char livesString[int(log(maxLivesCount)) + 2];
  itoa(lives, livesString, numericalBase);
  strcat(postGameInfo[4], livesString);

  strcpy(postGameInfo[5], "*Lvl reach.: ");
  char lvlString[2];
  lvlString[0] = difficulty;
  lvlString[1] = '\0';
  strcat(postGameInfo[5], lvlString);

  strcpy(postGameInfo[6], "[JS] Main Menu");

  currentInfo = 0;
  currentInfoLcdRow = 0;
  
  postGameInfoInit = true;
}

void Game::showPostGameInfo(GameAssets &gameAssets, Controls &controls, Sounds &sounds) {
  byte cursorCol = 0;
  
  gameAssets.lcdSetCursor(cursorCol, currentInfoLcdRow);
  gameAssets.lcdPrint(postGameInfo[currentInfo]);

  byte otherInfo = !currentInfoLcdRow ? currentInfo + 1 : currentInfo - 1;
  byte otherInfoLcdRow = 1 - currentInfoLcdRow;
  gameAssets.lcdSetCursor(cursorCol, otherInfoLcdRow);
  gameAssets.lcdPrint(postGameInfo[otherInfo]);

  LCDUtils::displayUpDownNavArrows(currentInfo, currentInfoLcdRow, postGameInfoLinesCount, gameAssets);

  int joyMoveDirection = controls.processJoyMenuMovement();
  if (joyMoveDirection == JOY_MOVED_UP) {
    LCDUtils::attemptToMoveUp(GAME_FINISHED, currentInfo, currentInfoLcdRow, gameAssets, sounds);
  } else if (joyMoveDirection == JOY_MOVED_DOWN) {
    LCDUtils::attemptToMoveDown(GAME_FINISHED, currentInfo, currentInfoLcdRow, postGameInfoLinesCount, gameAssets, sounds);
  }

  if (controls.joySWChangeState()) {
    sounds.attemptToPlayInteractWithMenuOptionSound();

    LCDUtils::goToPreviousMenu(gameAssets);

    gameFinished = false;
    gameLoaded = false;
  }
}