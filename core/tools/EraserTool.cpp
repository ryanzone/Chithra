#include "EraserTool.hpp"

EraserTool::EraserTool(int size) : eraserSize(size) {}

void EraserTool::setSize(int size) { eraserSize = size; }
void EraserTool::apply(ToolContext &context, int x, int y) {
  Layer *layer = context.layerManager.getActiveLayer();

  if (!layer) {
    return;
  }

  erase(layer->getImage(), x, y);
}

void EraserTool::erase(Image &image, int centerX, int centerY) {
  auto &pixels = image.getPixels();

  int width = image.getWidth();

  int height = image.getHeight();

  for (int y = centerY - eraserSize; y <= centerY + eraserSize; y++) {
    for (int x = centerX - eraserSize; x <= centerX + eraserSize; x++) {
      if (x < 0 || y < 0 || x >= width || y >= height) {
        continue;
      }

      int index = (y * width + x) * 4;

      pixels[index] = 0;
      pixels[index + 1] = 0;
      pixels[index + 2] = 0;
      pixels[index + 3] = 0;
    }
  }
}