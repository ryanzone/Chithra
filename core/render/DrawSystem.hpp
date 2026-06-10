#pragma once

#include "../managers/LayerManager.hpp"
#include "../tools/BrushTool.hpp"

class DrawSystem {
public:
  static void draw(LayerManager &layerManager, BrushTool &brush, int x, int y);
};