#pragma once

#include "../image/Image.hpp"
#include "../managers/LayerManager.hpp"


struct ToolContext {
  LayerManager &layerManager;
  Image &canvas;
};