#include "FillTool.hpp"

#include "../document/Layer.hpp"
#include <queue>
#include <utility>
#include <vector>

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

  std::queue<std::pair<int, int>> queue;

  std::vector<bool> visited(width * height, false);

  queue.push({startX, startY});

  queue.push({startX, startY});

  while (!queue.empty()) {
    auto [x, y] = queue.front();

    queue.pop();

    if (x < 0 || y < 0 || x >= width || y >= height) {
      continue;
    }

    int pixelPos = y * width + x;

    if (visited[pixelPos]) {
      continue;
    }

    visited[pixelPos] = true;

    int index = pixelPos * 4;

    if (pixels[index] != targetR || pixels[index + 1] != targetG ||
        pixels[index + 2] != targetB || pixels[index + 3] != targetA) {
      continue;
    }

    pixels[index] = fillColor.r;
    pixels[index + 1] = fillColor.g;
    pixels[index + 2] = fillColor.b;
    pixels[index + 3] = fillColor.a;

    queue.push({x + 1, y});
    queue.push({x - 1, y});
    queue.push({x, y + 1});
    queue.push({x, y - 1});
  }
}

void FillTool::setColor(const Color &color) { fillColor = color; }