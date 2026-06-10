#include "DrawSystem.hpp"

void DrawSystem::draw(LayerManager &layerManager, BrushTool &brush, int x,
                      int y) {
  Layer *layer = layerManager.getActiveLayer();

  if (!layer) {
    return;
  }

  brush.draw(layer->getImage(), x, y);
}