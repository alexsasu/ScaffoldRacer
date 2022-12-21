#pragma once

class Sounds {
  private:
    bool buzzerEnabled;
    int buzzerFreq;
    int buzzerOnDuration;

  public:
    Sounds();
    
    void setBuzzerEnabled(int); 

    void attemptToPlayMenuNavSound();
    void attemptToPlayInteractWithMenuOptionSound();

    void attemptToPlayIncreaseDifficultySound();

    void attemptToPlayBeginLifeSound();
    void attemptToPlayLoseLifeSound();
};