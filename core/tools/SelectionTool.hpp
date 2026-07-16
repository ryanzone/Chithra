#pragma once

#include "../selection/Selection.hpp"
#include "Tool.hpp"
#include <chrono>
#include <vector>


class SelectionTool : public Tool {
private:
  Selection selection;

  // Unified drag states
  bool isDraggingSelection;
  bool isMovingPixels;
  std::chrono::steady_clock::time_point lastCallTime;

  // Selection drawing states
  int anchorX;
  int anchorY;

  // Selection moving states
  int offsetX;
  int offsetY;
  int dragStartStartX;
  int dragStartStartY;
  int dragStartEndX;
  int dragStartEndY;

  // High-performance localized backup and rendering buffers
  std::vector<uint8_t> grabbedPixels;
  std::vector<uint8_t> backupPixels; // Holds the original canvas state
  int grabbedW;
  int grabbedH;
  int grabbedStartXOffset;
  int grabbedStartYOffset;

  // Tracks only the localized region drawn in the previous frame to cleanly
  // restore it
  int lastDrawnStartX;
  int lastDrawnStartY;
  int lastDrawnW;
  int lastDrawnH;

  // Cursor tracking coordinates to distinguish individual click-and-drag
  // gestures
  int lastX;
  int lastY;

public:
  SelectionTool();

  void apply(ToolContext &context, int x, int y) override;

  Selection &getSelection();
};