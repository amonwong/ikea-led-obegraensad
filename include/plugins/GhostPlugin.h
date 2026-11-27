#pragma once

#include "PluginManager.h"

// Amon's full-size outline ghost: gently bobs, blinks now and then and
// occasionally fades away and comes back.
class GhostPlugin : public Plugin
{
private:
  static const uint16_t TICK_MS = 400;

  uint32_t tickCount = 0;
  unsigned long nextTick = 0;
  bool raised = false;
  uint8_t blinkTicks = 0;
  uint8_t fade = 255;
  int8_t fadeStep = 0;
  uint8_t hiddenTicks = 0;

  uint8_t frame[256];

  void step();
  void render();

public:
  void setup() override;
  void loop() override;
  const char *getName() const override;
};
