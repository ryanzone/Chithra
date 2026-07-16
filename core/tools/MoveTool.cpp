#include "MoveTool.hpp"
#include "../document/Layer.hpp"
#include <algorithm>

MoveTool::MoveTool(Selection *selection)
    : selection(selection), isDragging(false), offsetX(0), offsetY(0),
      dragStartStartX(0), dragStartStartY(0), dragStartEndX(0),
      dragStartEndY(0), grabbedW(0), grabbedH(0), grabbedStartXOffset(0),
      grabbedStartYOffset(0), lastDrawnStartX(0), lastDrawnStartY(0),
      lastDrawnW(0), lastDrawnH(0) {
  // Initialize the call timer to a far-past point
  lastCallTime = std::chrono::steady_clock::now() - std::chrono::seconds(1);
}

void MoveTool::apply(ToolContext &context, int x, int y) {
  if (!selection || !selection->active) {
    return;
  }

  // Retrieve the correct target image context
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

  auto now = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - lastCallTime)
          .count();
  lastCallTime = now;

  // If the user pauses dragging or releases the mouse, treat the next move as a
  // new drag start
  if (elapsed > 200) {
    isDragging = false;
  }

  auto &pixels = image->getPixels();

  if (!isDragging) {
    // ─── 1. START OF DRAG: Copy selection content and erase original spot ───
    offsetX = x - selection->startX;
    offsetY = y - selection->startY;
    dragStartStartX = selection->startX;
    dragStartStartY = selection->startY;
    dragStartEndX = selection->endX;
    dragStartEndY = selection->endY;

    // Clamp selection bounds to image boundaries to prevent out-of-bounds reads
    int selStartX = std::max(0, std::min(selection->startX, imgW));
    int selStartY = std::max(0, std::min(selection->startY, imgH));
    int selEndX = std::max(0, std::min(selection->endX, imgW));
    int selEndY = std::max(0, std::min(selection->endY, imgH));

    grabbedW = selEndX - selStartX;
    grabbedH = selEndY - selStartY;
    grabbedStartXOffset = selStartX - selection->startX;
    grabbedStartYOffset = selStartY - selection->startY;

    if (grabbedW > 0 && grabbedH > 0) {
      grabbedPixels.resize(grabbedW * grabbedH * 4);

      // Copy pixel data from original position
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

      // Erase original selection contents (set to transparent)
      for (int sy = 0; sy < grabbedH; ++sy) {
        for (int sx = 0; sx < grabbedW; ++sx) {
          int srcX = selStartX + sx;
          int srcY = selStartY + sy;
          int srcIdx = (srcY * imgW + srcX) * 4;

          pixels[srcIdx] = 0;
          pixels[srcIdx + 1] = 0;
          pixels[srcIdx + 2] = 0;
          pixels[srcIdx + 3] = 0;
        }
      }

      // Initialize localized tracking bounds
      lastDrawnStartX = selStartX;
      lastDrawnStartY = selStartY;
      lastDrawnW = grabbedW;
      lastDrawnH = grabbedH;

      isDragging = true;
    }
  }

  // ─── 2. DRAGGING ACTIVE: Erase previous frame's copy and draw at new
  // position ───
  if (isDragging && grabbedW > 0 && grabbedH > 0) {

    // Erase ONLY the localized region where we drew the shape in the previous
    // frame
    for (int sy = 0; sy < lastDrawnH; ++sy) {
      for (int sx = 0; sx < lastDrawnW; ++sx) {
        int eraseX = lastDrawnStartX + sx;
        int eraseY = lastDrawnStartY + sy;
        if (eraseX >= 0 && eraseX < imgW && eraseY >= 0 && eraseY < imgH) {
          int idx = (eraseY * imgW + eraseX) * 4;
          pixels[idx] = 0;
          pixels[idx + 1] = 0;
          pixels[idx + 2] = 0;
          pixels[idx + 3] = 0;
        }
      }
    }

    // Calculate drag offsets
    int dx = (x - offsetX) - dragStartStartX;
    int dy = (y - offsetY) - dragStartStartY;

    // Slide the selection box boundaries
    selection->startX = dragStartStartX + dx;
    selection->startY = dragStartStartY + dy;
    selection->endX = dragStartEndX + dx;
    selection->endY = dragStartEndY + dy;

    // Localize the new render destination
    int destStartX = selection->startX + grabbedStartXOffset;
    int destStartY = selection->startY + grabbedStartYOffset;

    int drawStartX = std::max(0, std::min(destStartX, imgW));
    int drawStartY = std::max(0, std::min(destStartY, imgH));
    int drawEndX = std::max(0, std::min(destStartX + grabbedW, imgW));
    int drawEndY = std::max(0, std::min(destStartY + grabbedH, imgH));

    int drawW = drawEndX - drawStartX;
    int drawH = drawEndY - drawStartY;

    if (drawW > 0 && drawH > 0) {
      // Paint the grabbed pixels at the new coordinates
      for (int sy = 0; sy < drawH; ++sy) {
        for (int sx = 0; sx < drawW; ++sx) {
          int destX = drawStartX + sx;
          int destY = drawStartY + sy;
          int destIdx = (destY * imgW + destX) * 4;

          int grabX = destX - destStartX;
          int grabY = destY - destStartY;
          int srcIdx = (grabY * grabbedW + grabX) * 4;

          pixels[destIdx] = grabbedPixels[srcIdx];
          pixels[destIdx + 1] = grabbedPixels[srcIdx + 1];
          pixels[destIdx + 2] = grabbedPixels[srcIdx + 2];
          pixels[destIdx + 3] = grabbedPixels[srcIdx + 3];
        }
      }

      // Save this frame's bounds so we can selectively wipe them next frame
      lastDrawnStartX = drawStartX;
      lastDrawnStartY = drawStartY;
      lastDrawnW = drawW;
      lastDrawnH = drawH;
    } else {
      lastDrawnW = 0;
      lastDrawnH = 0;
    }
  }
}