#pragma once

#include <cmath>
#include <string_view>

int sign(int x) { return x >= 0 ? 1 : -1; }

bool nvu(int x1, int v1, int x2, int v2) {
  if (x1 == x2) {
    return v1 == v2;
  }
  if (v2 == v1) {
    return x2 == x1;
  }

  int start_dx = x1 - x2;
  int v_dx = v2 - v1;
  if (sign(start_dx) != sign(v_dx)) {
    return false;
  }

  return std::abs(start_dx) % std::abs(v_dx) == 0;
}

std::string_view kangaroo(int x1, int v1, int x2, int v2) {
  return nvu(x1, v1, x2, v2) ? "YES" : "NO";
}