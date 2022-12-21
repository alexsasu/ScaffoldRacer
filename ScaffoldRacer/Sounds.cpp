#include "Consts.h"
#include <EEPROM.h>
#include "Sounds.h"

Sounds::Sounds() {
  buzzerEnabled = EEPROM.read(eepromBuzzerSoundAddress);
}

void Sounds::setBuzzerEnabled(int buzzerEnabled) {
  this->buzzerEnabled = buzzerEnabled;
}

void Sounds::attemptToPlayMenuNavSound() {
  if (buzzerEnabled) {
    buzzerFreq = buzzerMenuNavFreq;
    buzzerOnDuration = buzzerMenuNavOnDuration;
    tone(buzzerPin, buzzerFreq, buzzerOnDuration);
  }
}

void Sounds::attemptToPlayInteractWithMenuOptionSound() {
  if (buzzerEnabled) {
    buzzerFreq = buzzerMenuSelectOptionFreq;
    buzzerOnDuration = buzzerMenuSelectOptionOnDuration;
    tone(buzzerPin, buzzerFreq, buzzerOnDuration);
  }
}

void Sounds::attemptToPlayIncreaseDifficultySound() {
  if (buzzerEnabled) {
    buzzerFreq = buzzerIncreaseDifficultyFreq;
    buzzerOnDuration = buzzerIncreaseDifficultyOnDuration;
    tone(buzzerPin, buzzerFreq, buzzerOnDuration);
  }
}

void Sounds::attemptToPlayBeginLifeSound() {
  if (buzzerEnabled) {
    buzzerFreq = buzzerPlayerReadyFreq;
    buzzerOnDuration = buzzerPlayerReadyOnDuration;
    tone(buzzerPin, buzzerFreq, buzzerOnDuration);
  }
  delay(700);
  if (buzzerEnabled) {
    buzzerFreq = buzzerPlayerSetFreq;
    buzzerOnDuration = buzzerPlayerSetOnDuration;
    tone(buzzerPin, buzzerFreq, buzzerOnDuration);
  }
  delay(700);
  if (buzzerEnabled) {
    buzzerFreq = buzzerPlayerGoFreq;
    buzzerOnDuration = buzzerPlayerGoOnDuration;
    tone(buzzerPin, buzzerFreq, buzzerOnDuration);
  }
  delay(buzzerPlayerGoOnDuration);
}

void Sounds::attemptToPlayLoseLifeSound() {
  if (buzzerEnabled) {
    buzzerFreq = buzzerPlayerLoseLifeSound1Freq;
    buzzerOnDuration = buzzerPlayerLoseLifeSound1OnDuration;
    tone(buzzerPin, buzzerFreq, buzzerOnDuration);
  }
  delay(buzzerPlayerLoseLifeSound2OnDuration);
  if (buzzerEnabled) {
    buzzerFreq = buzzerPlayerLoseLifeSound2Freq;
    buzzerOnDuration = buzzerPlayerLoseLifeSound2OnDuration;
    tone(buzzerPin, buzzerFreq, buzzerOnDuration);
  }
  delay(buzzerPlayerLoseLifeSound2OnDuration);
}