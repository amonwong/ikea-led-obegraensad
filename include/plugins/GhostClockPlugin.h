#pragma once

#include "PluginManager.h"

// Clock on top, with Amon's mini ghost living underneath like a pet:
// it drifts, wanders off one edge and back in the other, fades away and
// reappears somewhere else, peeks in from the side and sometimes floats
// upside down.
class GhostClockPlugin : public Plugin
{
private:
  enum Mood
  {
    DRIFT,
    WANDER,
    FADE_OUT,
    GONE,
    FADE_IN,
    PEEK,
    UPSIDE_DOWN
  };

  static const uint16_t TICK_MS = 220;
  static const uint8_t CLOCK_BRIGHTNESS = 140;
  static const uint8_t GHOST_Y = 6;
  static const int8_t X_MIN = 0;
  static const int8_t X_MAX = 6;

  struct tm timeinfo;
  bool haveTime = false;

  Mood mood = DRIFT;
  int8_t ghostX = 3;
  int8_t direction = 1;
  int16_t moodTicks = 0;
  uint8_t fade = 255;
  uint8_t blinkTicks = 0;
  bool flipped = false;
  bool peekHolding = false;
  uint32_t tickCount = 0;
  unsigned long nextTick = 0;

  uint8_t frame[256];

  void pickNextMood();
  void step();
  void render();
  void drawClock();
  void drawGhost(int8_t x);
  void put(int x, int y, uint8_t value);

public:
  void setup() override;
  void loop() override;
  const char *getName() const override;
};
