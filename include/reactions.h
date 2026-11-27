#pragma once

#include <Arduino.h>

// Plays short built-in reaction animations (X, thumbs up, ...) on top of
// whatever mode is running, then hands the screen back to that mode.
//
// - Requested from the web server (request() only records which reaction
//   to play; nothing is drawn from the network task).
// - Played from the main loop: the active plugin is paused the same way the
//   Weather plugin pauses it while loading (currentStatus = LOADING).
// - Never touches the scheduler: it is only paused for the few seconds the
//   reaction plays, and its list, position and stored state are unchanged.
class ReactionPlayer_
{
private:
  ReactionPlayer_() = default;

  volatile int pending = -1;
  int playing = -1;
  uint8_t frameIndex = 0;
  unsigned long nextFrameAt = 0;
  uint8_t frame[256];
  uint8_t saved[256]; // screen before the reaction, put back afterwards

  void drawFrame();
  void finish();

public:
  static ReactionPlayer_ &getInstance();

  ReactionPlayer_(const ReactionPlayer_ &) = delete;
  ReactionPlayer_ &operator=(const ReactionPlayer_ &) = delete;

  // Returns 1 when queued, 0 when the name is unknown, -1 when busy.
  int request(const char *name);
  bool isPlaying() const;
  void update();
  static const char *nameAt(uint8_t index);
  static uint8_t count();
};

extern ReactionPlayer_ &ReactionPlayer;
