#pragma once

struct Selection {
  int x;
  int y;

  int width;
  int height;

  bool active;

  Selection() : x(0), y(0), width(0), height(0), active(false) {}
};