#include "reactions.h"
#include "ReactionFrames.h"
#include "constants.h"
#include "screen.h"

ReactionPlayer_ &ReactionPlayer_::getInstance()
{
  static ReactionPlayer_ instance;
  return instance;
}

ReactionPlayer_ &ReactionPlayer = ReactionPlayer.getInstance();

int ReactionPlayer_::request(const char *name)
{
  if (name == nullptr)
  {
    return 0;
  }
  if (pending >= 0 || playing >= 0)
  {
    return -1;
  }
  for (uint8_t i = 0; i < REACTION_COUNT; i++)
  {
    if (strcmp(REACTIONS[i].name, name) == 0)
    {
      pending = i;
      return 1;
    }
  }
  return 0;
}

bool ReactionPlayer_::isPlaying() const
{
  return playing >= 0 || pending >= 0;
}

const char *ReactionPlayer_::nameAt(uint8_t index)
{
  return index < REACTION_COUNT ? REACTIONS[index].name : nullptr;
}

uint8_t ReactionPlayer_::count()
{
  return REACTION_COUNT;
}

void ReactionPlayer_::drawFrame()
{
  const uint8_t *packed = REACTIONS[playing].frames[frameIndex];
  for (int i = 0; i < 128; i++)
  {
    // brightness 0-15 per pixel, scaled to 0-255
    frame[i * 2] = (packed[i] >> 4) * 17;
    frame[i * 2 + 1] = (packed[i] & 0x0F) * 17;
  }
  Screen.setRenderBuffer(frame, true);
}

void ReactionPlayer_::finish()
{
  playing = -1;
  frameIndex = 0;
  // put back exactly what was on screen; the paused mode simply carries on
  // (no setup() re-run, so nothing is reset, re-downloaded or re-opened)
  Screen.setRenderBuffer(saved, true);
  currentStatus = NONE;
}

void ReactionPlayer_::update()
{
  if (playing < 0)
  {
    if (pending < 0)
    {
      return;
    }
    if (currentStatus != NONE)
    {
      // something else (an update, a loading plugin) owns the screen; wait
      return;
    }
    playing = pending;
    pending = -1;
    frameIndex = 0;
    currentStatus = LOADING; // pauses the active plugin, like WeatherPlugin does
    memcpy(saved, Screen.getRenderBuffer(), sizeof(saved));
    nextFrameAt = 0;
  }

  if (millis() < nextFrameAt)
  {
    return;
  }

  if (frameIndex >= REACTIONS[playing].frameCount)
  {
    finish();
    return;
  }

  drawFrame();
  frameIndex++;
  nextFrameAt = millis() + REACTIONS[playing].frameMs;
}
