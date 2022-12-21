#include "Controls.h"
#include "Macros.h"

Controls::Controls() {
  // Joystick setup
  pinMode(joySWPin, INPUT_PULLUP);
  joySWReading = HIGH;
  joySWState = HIGH;
  joySWLastState = HIGH;
  joySWLastDebounceTime = 0;
  joyIsNeutral = true;
}

bool Controls::joySWChangeState() {
  joySWReading = digitalRead(joySWPin);

  if (buttonDebounce(joySWReading, joySWLastState, joySWLastDebounceTime, joySWDebounceDelay)) {
    if (joySWReading != joySWState) {
      joySWState = joySWReading;

      if (joySWState == HIGH) {
        joySWLastState = joySWReading;
        return true;
      } else {
        joySWLastState = joySWReading;
        return false;
      }
    }
  }

  joySWLastState = joySWReading;
  return false;
}

int Controls::processJoyMenuMovement() {
  int joyXValue = analogRead(joyXPin);
  int joyYValue = analogRead(joyYPin);

  if (joyXValue > joyMaxThreshold && joyXValue - joyMaxThreshold > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;
    return JOY_MOVED_UP;
  }
  if (joyXValue < joyMinThreshold && joyMinThreshold - joyXValue > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;
    return JOY_MOVED_DOWN;
  }
  if (joyYValue < joyMinThreshold && joyMinThreshold - joyYValue > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;
    return JOY_MOVED_LEFT;
  }
  if (joyYValue > joyMaxThreshold && joyYValue - joyMaxThreshold > joySensitivityBypassAmount && joyIsNeutral) {
    joyIsNeutral = false;
    return JOY_MOVED_RIGHT;
  }

  if (joyXValue >= joyMinThreshold && joyXValue <= joyMaxThreshold &&
      joyYValue >= joyMinThreshold && joyYValue <= joyMaxThreshold) {
    joyIsNeutral = true;
    return JOY_IS_NEUTRAL;
  }
}

int Controls::processJoyPlayerMovement() {
  int joyXValue = analogRead(joyXPin);
  int joyYValue = analogRead(joyYPin);

  if ((joyXValue > joyMaxThreshold && joyXValue - joyMaxThreshold > joySensitivityBypassAmount) &&
      (joyYValue < joyMinThreshold && joyMinThreshold - joyYValue > joySensitivityBypassAmount)) {
    return JOY_MOVED_UP_LEFT;
  }
  if ((joyXValue > joyMaxThreshold && joyXValue - joyMaxThreshold > joySensitivityBypassAmount) &&
      (joyYValue > joyMaxThreshold && joyYValue - joyMaxThreshold > joySensitivityBypassAmount)) {
    return JOY_MOVED_UP_RIGHT;
  }
  if ((joyXValue < joyMinThreshold && joyMinThreshold - joyXValue > joySensitivityBypassAmount) &&
      (joyYValue < joyMinThreshold && joyMinThreshold - joyYValue > joySensitivityBypassAmount)) {
    return JOY_MOVED_DOWN_LEFT;
  }
  if ((joyXValue < joyMinThreshold && joyMinThreshold - joyXValue > joySensitivityBypassAmount) &&
      (joyYValue > joyMaxThreshold && joyYValue - joyMaxThreshold > joySensitivityBypassAmount)) {
    return JOY_MOVED_DOWN_RIGHT;
  }
  if (joyXValue > joyMaxThreshold && joyXValue - joyMaxThreshold > joySensitivityBypassAmount) {
    return JOY_MOVED_UP;
  }
  if (joyXValue < joyMinThreshold && joyMinThreshold - joyXValue > joySensitivityBypassAmount) {
    return JOY_MOVED_DOWN;
  }
  if (joyYValue < joyMinThreshold && joyMinThreshold - joyYValue > joySensitivityBypassAmount) {
    return JOY_MOVED_LEFT;
  }
  if (joyYValue > joyMaxThreshold && joyYValue - joyMaxThreshold > joySensitivityBypassAmount) {
    return JOY_MOVED_RIGHT;
  }

  if (joyXValue >= joyMinThreshold && joyXValue <= joyMaxThreshold &&
      joyYValue >= joyMinThreshold && joyYValue <= joyMaxThreshold) {
    return JOY_IS_NEUTRAL;
  }
}

bool Controls::buttonDebounce(byte reading, byte lastButtonState, unsigned long &lastDebounceTime, unsigned int debounceDelay) {
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  return (millis() - lastDebounceTime > debounceDelay);
}