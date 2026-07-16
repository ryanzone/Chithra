#include "SelectionTool.hpp"
#include "../document/Layer.hpp"
#include <algorithm>
#include <cmath>

SelectionTool::SelectionTool()
    : isDraggingSelection(false), isMovingPixels(false), anchorX(0), anchorY(0),
      offsetX(0), offsetY(0), dragStartStartX(0), dragStartStartY(0),
      dragStartEndX(0), dragStartEndY(0), grabbedW(0), grabbedH(0),
      grabbedStartXOffset(0), grabbedStartYOffset(0), lastDrawnStartX(0),
      lastDrawnStartY(0), lastDrawnW(0), lastDrawnH(0), lastX(0), lastY(0) {
  lastCallTime = std::chrono::steady_clock::now() - std::chrono::seconds(1);
  selection.active = false;
}

void SelectionTool::apply(ToolContext &context, int x, int y) {
  Image *image;
  Layer *layer = context.layerManager.getActiveLayer();
  if (layer)
    image = &layer->getImage();
  else
    image = &context.canvas;

  int imgW = image->getWidth();
  int imgH = image->getHeight();
  if (imgW <= 0 || imgH <= 0) {
    return;
  }

  // 1. SMART GESTURE DETECTION (No external library dependencies needed)
  auto now = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - lastCallTime)
          .count();
  lastCallTime = now;

  int dx = x - lastX;
  int dy = y - lastY;
  double distance = std::sqrt(dx * dx + dy * dy);

  lastX = x;
  lastY = y;

  bool isNewGesture = false;
  if (elapsed > 350) {
    // A long pause between updates always represents a new mouse click
    isNewGesture = true;
  } else if (elapsed > 100 && distance > 15) {
    // If the frame rate stutters slightly but the mouse jump is large,
    // it indicates a release and re-click elsewhere on the canvas
    isNewGesture = true;
  }

  // 2. GESTURE RECOGNITION INITIALIZATION
  if (isNewGesture) {
    isDraggingSelection = false;
    isMovingPixels = false;
    backupPixels.clear(); // Free memory

    // Check if the click occurred INSIDE an active selection box
    if (selection.active && x >= selection.startX && x < selection.endX &&
        y >= selection.startY && y < selection.endY) {

      // ─── START MOVE MODE ───
      isMovingPixels = true;

      offsetX = x - selection.startX;
      offsetY = y - selection.startY;
      dragStartStartX = selection.startX;
      dragStartStartY = selection.startY;
      dragStartEndX = selection.endX;
      dragStartEndY = selection.endY;

      int selStartX = std::max(0, std::min(selection.startX, imgW));
      int selStartY = std::max(0, std::min(selection.startY, imgH));
      int selEndX = std::max(0, std::min(selection.endX, imgW));
      int selEndY = std::max(0, std::min(selection.endY, imgH));

      grabbedW = selEndX - selStartX;
      grabbedH = selEndY - selStartY;
      grabbedStartXOffset = selStartX - selection.startX;
      grabbedStartYOffset = selStartY - selection.startY;

      if (grabbedW > 0 && grabbedH > 0) {
        grabbedPixels.resize(grabbedW * grabbedH * 4);
        auto &pixels = image->getPixels();

        // Copy active pixels to transfer buffer
        for (int sy = 0; sy < grabbedH; ++sy) {
          for (int sx = 0; sx < grabbedW; ++sx) {
            int srcX = selStartX + sx;
            int srcY = selStartY + sy;
            int srcIdx = (srcY * imgW + srcX) * 4;
            int dstIdx = (sy * grabbedW + sx) * 4;

            grabbedPixels[dstIdx] = pixels[srcIdx];
            grabbedPixels[dstIdx + 1] = pixels[srcIdx + 1];
            grabbedPixels[dstIdx + 2] = pixels[srcIdx + 2];
            grabbedPixels[dstIdx + 3] = pixels[srcIdx + 3];
          }
        }

        // Back up original canvas
        backupPixels = pixels;

        // Cut out selected pixels, filling with clean background white (255,
        // 255, 255)
        for (int sy = 0; sy < grabbedH; ++sy) {
          for (int sx = 0; sx < grabbedW; ++sx) {
            int srcX = selStartX + sx;
            int srcY = selStartY + sy;
            int srcIdx = (srcY * imgW + srcX) * 4;

            // Transparent selection detection: Treat white and alpha-0 as
            // background[cite: 14]
            bool isBg = (pixels[srcIdx + 3] == 0) ||
                        (pixels[srcIdx] == 255 && pixels[srcIdx + 1] == 255 &&
                         pixels[srcIdx + 2] == 255);

            if (!isBg) {
              pixels[srcIdx] = 255;
              pixels[srcIdx + 1] = 255;
              pixels[srcIdx + 2] = 255;
              pixels[srcIdx + 3] = 255;

              backupPixels[srcIdx] = 255;
              backupPixels[srcIdx + 1] = 255;
              backupPixels[srcIdx + 2] = 255;
              backupPixels[srcIdx + 3] = 255;
            }
          }
        }

        lastDrawnStartX = selStartX;
        lastDrawnStartY = selStartY;
        lastDrawnW = grabbedW;
        lastDrawnH = grabbedH;
      }
    } else {
      // ─── START DRAW SELECTION MODE ───
      isDraggingSelection = true;
      anchorX = x;
      anchorY = y;

      selection.startX = x;
      selection.startY = y;
      selection.endX = x;
      selection.endY = y;
      selection.active = true;
    }
  }

  // 3. ACTIVE DRAGGING EXECUTION
  auto &pixels = image->getPixels();

  if (isMovingPixels && grabbedW > 0 && grabbedH > 0 && !backupPixels.empty()) {
    // Restore ONLY the localized rectangle where the selection sat in the
    // previous frame
    for (int sy = 0; sy < lastDrawnH; ++sy) {
      for (int sx = 0; sx < lastDrawnW; ++sx) {
        int restoreX = lastDrawnStartX + sx;
        int restoreY = lastDrawnStartY + sy;
        if (restoreX >= 0 && restoreX < imgW && restoreY >= 0 &&
            restoreY < imgH) {
          int idx = (restoreY * imgW + restoreX) * 4;
          pixels[idx] = backupPixels[idx];
          pixels[idx + 1] = backupPixels[idx + 1];
          pixels[idx + 2] = backupPixels[idx + 2];
          pixels[idx + 3] = backupPixels[idx + 3];
        }
      }
    }

    // Move selection boundaries smoothly[cite: 14]
    int dragDx = (x - offsetX) - dragStartStartX;
    int dragDy = (y - offsetY) - dragStartStartY;

    selection.startX = dragStartStartX + dragDx;
    selection.startY = dragStartStartY + dragDy;
    selection.endX = dragStartEndX + dragDx;
    selection.endY = dragStartEndY + dragDy;

    int destStartX = selection.startX + grabbedStartXOffset;
    int destStartY = selection.startY + grabbedStartYOffset;

    int drawStartX = std::max(0, std::min(destStartX, imgW));
    int drawStartY = std::max(0, std::min(destStartY, imgH));
    int drawEndX = std::max(0, std::min(destStartX + grabbedW, imgW));
    int drawEndY = std::max(0, std::min(destStartY + grabbedH, imgH));

    int drawW = drawEndX - drawStartX;
    int drawH = drawEndY - drawStartY;

    if (drawW > 0 && drawH > 0) {
      // Render selection content with MS Paint style transparent selection
      // filter[cite: 14]
      for (int sy = 0; sy < drawH; ++sy) {
        for (int sx = 0; sx < drawW; ++sx) {
          int destX = drawStartX + sx;
          int destY = drawStartY + sy;
          int destIdx = (destY * imgW + destX) * 4;

          int grabX = destX - destStartX;
          int grabY = destY - destStartY;
          int srcIdx = (grabY * grabbedW + grabX) * 4;

          bool isBg = (grabbedPixels[srcIdx + 3] == 0) ||
                      (grabbedPixels[srcIdx] == 255 &&
                       grabbedPixels[srcIdx + 1] == 255 &&
                       grabbedPixels[srcIdx + 2] == 255);

          if (!isBg) {
            pixels[destIdx] = grabbedPixels[srcIdx];
            pixels[destIdx + 1] = grabbedPixels[srcIdx + 1];
            pixels[destIdx + 2] = grabbedPixels[srcIdx + 2];
            pixels[destIdx + 3] = grabbedPixels[srcIdx + 3];
          }
        }
      }

      // Record bounds so we can restore them cleanly next update
      lastDrawnStartX = drawStartX;
      lastDrawnStartY = drawStartY;
      lastDrawnW = drawW;
      lastDrawnH = drawH;
    } else {
      lastDrawnW = 0;
      lastDrawnH = 0;
    }
  } else if (isDraggingSelection) {
    // Normalizing coordinates so start is Top-Left and end is
    // Bottom-Right[cite: 14]
    selection.startX = std::min(anchorX, x);
    selection.startY = std::min(anchorY, y);
    selection.endX = std::max(anchorX, x);
    selection.endY = std::max(anchorY, y);
    selection.active = true;
  }
}

Selection &SelectionTool::getSelection() { return selection; }