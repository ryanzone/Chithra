#include "BrushTool.hpp"

BrushTool::BrushTool(int size, Color color) : brushSize(size), color(color) {}

void BrushTool::setBrushSize(int size) { brushSize = size; }

void BrushTool::setColor(const Color &color) { this->color = color; }
void BrushTool::apply(ToolContext &context, int x, int y) {
  Layer *layer = context.layerManager.getActiveLayer();

  if (layer)
    draw(layer->getImage(), x, y);
  else
    draw(context.canvas, x, y);
}
void BrushTool::draw(Image &image, int centerX, int centerY) {
  auto &pixels = image.getPixels();

  int width = image.getWidth();

  int height = image.getHeight();

  for (int y = centerY - brushSize; y <= centerY + brushSize; y++) {
    for (int x = centerX - brushSize; x <= centerX + brushSize; x++) {
      if (x < 0 || y < 0 || x >= width || y >= height) {
        continue;
      }

      int index = (y * width + x) * 4;

      pixels[index] = color.r;
      pixels[index + 1] = color.g;
      pixels[index + 2] = color.b;
      pixels[index + 3] = color.a;
    }
  }
}