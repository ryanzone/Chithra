#pragma once

#include "../selection/Selection.hpp"
#include "Tool.hpp"
#include <chrono>
#include <vector>


class MoveTool : public Tool {
private:
  Selection *selection;

  // Drag tracking state
  bool isDragging;
  std::chrono::steady_clock::time_point lastCallTime;

  int offsetX;
  int offsetY;
  int dragStartStartX;
  int dragStartStartY;
  int dragStartEndX;
  int dragStartEndY;

  // Pixel buffers for moving selection contents
  std::vector<uint8_t> grabbedPixels;
  int grabbedW;
  int grabbedH;
  int grabbedStartXOffset;
  int grabbedStartYOffset;

  // Tracks only the localized area we drew in the previous frame to erase it
  int lastDrawnStartX;
  int lastDrawnStartY;
  int lastDrawnW;
  int lastDrawnH;

public:
  MoveTool(Selection *selection);

  void apply(ToolContext &context, int x, int y) override;
};