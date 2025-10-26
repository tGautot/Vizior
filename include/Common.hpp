#ifndef VIZIOR_COMMON_HPP
#define VIZIOR_COMMON_HPP

#include <cstdint>

namespace vzr {
  enum ANCHOR {
      ANCHOR_TR, ANCHOR_TL, ANCHOR_BR, ANCHOR_BL, ANCHOR_C // Top-Bottom Left-Right Center
  };

  typedef struct {
      uint8_t r, g, b, a;
  } Color;
}

#endif