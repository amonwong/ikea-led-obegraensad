#include "plugins/GhostClockPlugin.h"
#include "plugins/GhostSprites.h"

void GhostClockPlugin::setup()
{
  mood = DRIFT;
  ghostX = 3;
  direction = 1;
  moodTicks = 20;
  fade = 255;
  flipped = false;
  peekHolding = false;
  blinkTicks = 0;
  tickCount = 0;
  nextTick = 0;
  haveTime = false;
  render();
}

void GhostClockPlugin::loop()
{
  if (millis() < nextTick)
  {
    return;
  }
  nextTick = millis() + TICK_MS;

  // non-blocking: only use the time once it has been synced
  time_t now = time(nullptr);
  haveTime = now > 1600000000;
  if (haveTime)
  {
    localtime_r(&now, &timeinfo);
  }
  step();
  render();
}

void GhostClockPlugin::pickNextMood()
{
  long r = random(100);

  if (r < 25)
  {
    mood = WANDER;
  }
  else if (r < 45)
  {
    mood = FADE_OUT;
  }
  else if (r < 62)
  {
    mood = PEEK;
    ghostX = -GHOST_MINI_W;
    peekHolding = false;
    moodTicks = 0;
    fade = 255;
  }
  else if (r < 82)
  {
    mood = UPSIDE_DOWN;
    flipped = true;
    moodTicks = 24;
  }
  else
  {
    mood = DRIFT;
    moodTicks = 20;
  }
}

void GhostClockPlugin::step()
{
  tickCount++;

  switch (mood)
  {
  case DRIFT:
  case UPSIDE_DOWN:
    if (tickCount % 2 == 0)
    {
      ghostX += direction;
      if (ghostX >= X_MAX || ghostX <= X_MIN)
      {
        direction = -direction;
      }
    }
    if (--moodTicks <= 0)
    {
      if (mood == UPSIDE_DOWN)
      {
        flipped = false;
        mood = DRIFT;
        moodTicks = 16;
      }
      else
      {
        pickNextMood();
      }
    }
    break;

  case WANDER:
    // keep going in the same direction, off one edge and back in the other
    if (tickCount % 2 == 0)
    {
      ghostX += direction;
      if (ghostX > 15)
      {
        ghostX = -GHOST_MINI_W;
      }
      else if (ghostX < -GHOST_MINI_W)
      {
        ghostX = 15;
      }
      if (ghostX >= X_MIN && ghostX <= X_MAX && random(100) < 6)
      {
        mood = DRIFT;
        moodTicks = 18;
      }
    }
    break;

  case FADE_OUT:
    if (fade <= 50)
    {
      fade = 0;
      mood = GONE;
      moodTicks = 12;
    }
    else
    {
      fade -= 50;
    }
    break;

  case GONE:
    if (--moodTicks <= 0)
    {
      ghostX = random(X_MIN, X_MAX + 1);
      mood = FADE_IN;
    }
    break;

  case FADE_IN:
    if (fade >= 205)
    {
      fade = 255;
      mood = DRIFT;
      moodTicks = 18;
    }
    else
    {
      fade += 50;
    }
    break;

  case PEEK:
    if (!peekHolding)
    {
      if (ghostX < -5)
      {
        ghostX++;
      }
      else
      {
        peekHolding = true;
        moodTicks = 10;
      }
    }
    else if (moodTicks > 0)
    {
      moodTicks--;
    }
    else
    {
      ghostX--;
      if (ghostX <= -GHOST_MINI_W)
      {
        ghostX = random(X_MIN, X_MAX + 1);
        fade = 0;
        mood = FADE_IN;
      }
    }
    break;
  }

  if (blinkTicks > 0)
  {
    blinkTicks--;
  }
  else if (random(100) < 4)
  {
    blinkTicks = 1;
  }
}

void GhostClockPlugin::put(int x, int y, uint8_t value)
{
  if (x < 0 || x > 15 || y < 0 || y > 15)
  {
    return;
  }
  uint8_t &px = frame[y * 16 + x];
  if (value > px)
  {
    px = value;
  }
}

void GhostClockPlugin::drawClock()
{
  if (!haveTime)
  {
    // dashes until the time has synced
    for (int i = 0; i < 4; i++)
    {
      for (int x = 0; x < 3; x++)
      {
        put(i * 4 + x, 2, CLOCK_BRIGHTNESS);
      }
    }
    return;
  }

  int digits[4] = {timeinfo.tm_hour / 10, timeinfo.tm_hour % 10, timeinfo.tm_min / 10, timeinfo.tm_min % 10};
  for (int i = 0; i < 4; i++)
  {
    for (int y = 0; y < 5; y++)
    {
      for (int x = 0; x < 3; x++)
      {
        if (GHOST_DIGITS[digits[i]][y] & (0b100 >> x))
        {
          put(i * 4 + x, y, CLOCK_BRIGHTNESS);
        }
      }
    }
  }
}

void GhostClockPlugin::drawGhost(int8_t gx)
{
  for (int row = 0; row < GHOST_MINI_H; row++)
  {
    int srcRow = flipped ? (GHOST_MINI_H - 1 - row) : row;
    uint16_t bits = GHOST_MINI[srcRow];
    if (blinkTicks > 0 && srcRow == GHOST_MINI_EYE_ROW)
    {
      bits &= ~GHOST_MINI_EYE_MASK;
    }
    for (int col = 0; col < GHOST_MINI_W; col++)
    {
      if (bits & (0x8000 >> col))
      {
        put(gx + col, GHOST_Y + row, fade);
      }
    }
  }
}

void GhostClockPlugin::render()
{
  memset(frame, 0, sizeof(frame));
  drawClock();

  if (mood != GONE && fade > 0)
  {
    drawGhost(ghostX);
  }

  Screen.setRenderBuffer(frame, true);
}

const char *GhostClockPlugin::getName() const
{
  return "Ghost Clock";
}
