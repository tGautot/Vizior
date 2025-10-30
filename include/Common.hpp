#ifndef VIZIOR_COMMON_HPP
#define VIZIOR_COMMON_HPP

#include <cstdint>

namespace vzr {

  constexpr double C_PI = 3.14159265358979323846;
  constexpr double C_2PI = 2*C_PI;
  constexpr double C_E = 2.71828182845904523536;
  constexpr double C_SQRT2 = 1.41421356237309504880;
  constexpr double C_PHI = 1.61803398874989484820;


  enum ANCHOR {
      ANCHOR_TR, ANCHOR_TL, ANCHOR_BR, ANCHOR_BL, ANCHOR_C // Top-Bottom Left-Right Center
  };

  typedef struct {
      uint8_t r, g, b, a;
  } Color;


  inline double rad2deg(double rad){
    return rad * (180.0f/C_PI);
  }

  inline double deg2rad(double deg){
    return  deg * (C_PI/180.0f);
  }
}

#endif