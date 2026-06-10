#include "FillTool.hpp"

#include "../document/Layer.hpp"

FillTool::FillTool(Color color) : fillColor(color) {}

void FillTool::apply(ToolContext &context, int x, int y) {
  Layer *layer = context.layerManager.getActiveLayer();

  if (!layer) {
    return;
  }

  auto &pixels = layer->getImage().getPixels();

  for (size_t i = 0; i < pixels.size(); i += 4) {
    pixels[i] = fillColor.r;
    pixels[i + 1] = fillColor.g;
    pixels[i + 2] = fillColor.b;
    pixels[i + 3] = fillColor.a;
  }
}