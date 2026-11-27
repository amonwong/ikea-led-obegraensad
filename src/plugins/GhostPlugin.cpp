#include "plugins/GhostPlugin.h"
#include "plugins/GhostSprites.h"

void GhostPlugin::setup()
{
  tickCount = 0;
  nextTick = 0;
  raised = false;
  blinkTicks = 0;
  fade = 255;
  fadeStep = 0;
  hiddenTicks = 0;
  render();
}

void GhostPlugin::loop()
{
  if (millis() < nextTick)
  {
    return;
  }
  nextTick = millis() + TICK_MS;

  step();
  render();
}

void GhostPlugin::step()
{
  tickCount++;

  // bob: two frames down, two frames up
  if (tickCount % 2 == 0)
  {
    raised = !raised;
  }

  if (blinkTicks > 0)
  {
    blinkTicks--;
  }
  else if (random(100) < 6)
  {
    blinkTicks = 1;
  }

  // now and then, vanish for a moment and fade back in
  if (fadeStep == 0 && hiddenTicks == 0 && random(1000) < 8)
  {
    fadeStep = -1;
  }
  if (fadeStep < 0)
  {
    if (fade <= 60)
    {
      fade = 0;
      fadeStep = 0;
      hiddenTicks = 6;
    }
    else
    {
      fade -= 60;
    }
  }
  else if (hiddenTicks > 0)
  {
    if (--hiddenTicks == 0)
    {
      fadeStep = 1;
    }
  }
  else if (fadeStep > 0)
  {
    if (fade >= 195)
    {
      fade = 255;
      fadeStep = 0;
    }
    else
    {
      fade += 60;
    }
  }
}

void GhostPlugin::render()
{
  memset(frame, 0, sizeof(frame));

  if (fade > 0)
  {
    int offset = raised ? -1 : 0;
    for (int row = 0; row < 16; row++)
    {
      int y = row + offset;
      if (y < 0 || y > 15)
      {
        continue;
      }
      uint16_t bits = GHOST_FULL[row];
      if (blinkTicks > 0 && (row == GHOST_FULL_EYE_ROWS[0] || row == GHOST_FULL_EYE_ROWS[1]))
      {
        bits &= ~GHOST_FULL_EYE_MASK;
      }
      for (int x = 0; x < 16; x++)
      {
        if (bits & (0x8000 >> x))
        {
          frame[y * 16 + x] = fade;
        }
      }
    }
  }

  Screen.setRenderBuffer(frame, true);
}

const char *GhostPlugin::getName() const
{
  return "Ghost";
}
