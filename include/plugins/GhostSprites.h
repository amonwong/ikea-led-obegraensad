#pragma once

#include <Arduino.h>

// Ghost artwork by Amon. Each row is a bitmask, most significant bit = leftmost pixel.

// Full-size outline ghost (16x16).
static const uint16_t GHOST_FULL[16] = {
    0b0000000000000000,
    0b0000011111110000,
    0b0000100000001000,
    0b0001000000000100,
    0b0001000000000100,
    0b0001000000000100,
    0b0001000000000100,
    0b0001010010000100,
    0b0001010010000100,
    0b0001000000000100,
    0b0111000001110100,
    0b0100000001000100,
    0b0011000000110100,
    0b0001000000000010,
    0b0001000000000001,
    0b0000111111111111,
};

// Rows of GHOST_FULL that hold the eyes (cleared to blink).
static const uint8_t GHOST_FULL_EYE_ROWS[2] = {7, 8};
static const uint16_t GHOST_FULL_EYE_MASK = 0b0000010010000000;

// Mini ghost (10x10) used by the Ghost Clock. Bits 15..6 hold the 10 columns.
static const uint8_t GHOST_MINI_W = 10;
static const uint8_t GHOST_MINI_H = 10;
static const uint16_t GHOST_MINI[10] = {
    0b0001111000000000,
    0b0010000100000000,
    0b0100000010000000,
    0b0101010010000000,
    0b0100000010000000,
    0b1100011010000000,
    0b1000010010000000,
    0b0100001010000000,
    0b0100000001000000,
    0b0011111111000000,
};
static const uint8_t GHOST_MINI_EYE_ROW = 3;
static const uint16_t GHOST_MINI_EYE_MASK = 0b0001010000000000;

// 3x5 digits for the Ghost Clock, each row a 3-bit mask (bit 2 = left).
static const uint8_t GHOST_DIGITS[10][5] = {
    {0b111, 0b101, 0b101, 0b101, 0b111},
    {0b010, 0b110, 0b010, 0b010, 0b111},
    {0b111, 0b001, 0b111, 0b100, 0b111},
    {0b111, 0b001, 0b111, 0b001, 0b111},
    {0b101, 0b101, 0b111, 0b001, 0b001},
    {0b111, 0b100, 0b111, 0b001, 0b111},
    {0b111, 0b100, 0b111, 0b101, 0b111},
    {0b111, 0b001, 0b010, 0b010, 0b010},
    {0b111, 0b101, 0b111, 0b101, 0b111},
    {0b111, 0b101, 0b111, 0b001, 0b111},
};
