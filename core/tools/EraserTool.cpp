#include "EraserTool.hpp"

EraserTool::EraserTool(int size) : eraserSize(size) {}

void EraserTool::setSize(int size) { eraserSize = size; }
void EraserTool::apply(ToolContext &context, int x, int y) {
  Layer *layer = context.layerManager.getActiveLayer();

  if (layer) {
    erase(layer->getImage(), x, y, false);
  } else {
    erase(context.canvas, x, y, true);
  }
}

void EraserTool::erase(Image &image, int centerX, int centerY,
                       bool background) {
  auto &pixels = image.getPixels();

  int width = image.getWidth();

  int height = image.getHeight();

  for (int y = centerY - eraserSize; y <= centerY + eraserSize; y++) {
    for (int x = centerX - eraserSize; x <= centerX + eraserSize; x++) {
      if (x < 0 || y < 0 || x >= width || y >= height) {
        continue;
      }

      int index = (y * width + x) * 4;

      if (background) {
        pixels[index] = 255;
        pixels[index + 1] = 255;
        pixels[index + 2] = 255;
        pixels[index + 3] = 255;
      } else {
        pixels[index] = 0;
        pixels[index + 1] = 0;
        pixels[index + 2] = 0;
        pixels[index + 3] = 0;
      }
    }
  }
}