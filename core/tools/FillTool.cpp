#include "FillTool.hpp"

#include "../document/Layer.hpp"
#include <queue>
#include <utility>

FillTool::FillTool(Color color) : fillColor(color) {}

void FillTool::apply(ToolContext &context, int startX, int startY) {
  Image *image;

  Layer *layer = context.layerManager.getActiveLayer();

  if (layer)
    image = &layer->getImage();
  else
    image = &context.canvas;

  auto &pixels = image->getPixels();

  int width = image->getWidth();
  int height = image->getHeight();

  if (startX < 0 || startY < 0 || startX >= width || startY >= height) {
    return;
  }

  int startIndex = (startY * width + startX) * 4;

  uint8_t targetR = pixels[startIndex];
  uint8_t targetG = pixels[startIndex + 1];
  uint8_t targetB = pixels[startIndex + 2];
  uint8_t targetA = pixels[startIndex + 3];

  // Prevent infinite loops / useless work if filling with the same color
  if (targetR == fillColor.r && targetG == fillColor.g &&
      targetB == fillColor.b && targetA == fillColor.a) {
    return;
  }

  std::queue<std::pair<int, int>> queue;

  // Paint the starting pixel immediately so neighbors don't re-push it
  pixels[startIndex] = fillColor.r;
  pixels[startIndex + 1] = fillColor.g;
  pixels[startIndex + 2] = fillColor.b;
  pixels[startIndex + 3] = fillColor.a;

  queue.push({startX, startY});

  int dx[4] = {1, -1, 0, 0};
  int dy[4] = {0, 0, 1, -1};

  while (!queue.empty()) {
    auto [x, y] = queue.front();
    queue.pop();

    for (int i = 0; i < 4; ++i) {
      int nx = x + dx[i];
      int ny = y + dy[i];

      if (nx >= 0 && ny >= 0 && nx < width && ny < height) {
        int nIndex = (ny * width + nx) * 4;

        // Check if neighbor matches the target color
        if (pixels[nIndex] == targetR && pixels[nIndex + 1] == targetG &&
            pixels[nIndex + 2] == targetB && pixels[nIndex + 3] == targetA) {

          // Paint immediately to mark it as visited
          pixels[nIndex] = fillColor.r;
          pixels[nIndex + 1] = fillColor.g;
          pixels[nIndex + 2] = fillColor.b;
          pixels[nIndex + 3] = fillColor.a;

          queue.push({nx, ny});
        }
      }
    }
  }
}

void FillTool::setColor(const Color &color) { fillColor = color; }