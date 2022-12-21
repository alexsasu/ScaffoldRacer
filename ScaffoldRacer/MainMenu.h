#pragma once
#include "Arduino.h"
#include "Controls.h"
#include "GameAssets.h"
#include "Sounds.h"

class MainMenu {
  private:
    int currentOption;
    byte currentOptionLcdRow;
    bool matrixIconInitialized;

  public:
    MainMenu();

    byte getCurrentOption();

    void displayMainMenu(GameAssets&, Controls&, Sounds&);
    void attemptToInitializeMatrixIcon(GameAssets&);
};