#pragma once

struct Selection {
  int startX;
  int startY;

  int endX;
  int endY;

  bool active;

  Selection() : startX(0), startY(0), endX(0), endY(0), active(false) {}

  int width() const { return endX - startX; }

  int height() const { return endY - startY; }
};