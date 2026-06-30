#include "Document.hpp"

Document::Document(int width, int height)
    : width(width), height(height), canvas(width, height) {
  auto &pixels = canvas.getPixels();

  for (size_t i = 0; i < pixels.size(); i += 4) {
    pixels[i] = 255;
    pixels[i + 1] = 255;
    pixels[i + 2] = 255;
    pixels[i + 3] = 255;
  }
}
Image &Document::getCanvas() { return canvas; }

const Image &Document::getCanvas() const { return canvas; }

size_t Document::addLayer(const std::string &name) {
  layers.emplace_back(name, width, height);

  return layers.size() - 1;
}

void Document::removeLayer(size_t index) {
  if (index >= layers.size()) {
    return;
  }

  layers.erase(layers.begin() + index);
}

void Document::moveLayerUp(size_t index) {
  if (index + 1 >= layers.size())
    return;

  std::swap(layers[index], layers[index + 1]);
}

void Document::moveLayerDown(size_t index) {
  if (index == 0)
    return;

  std::swap(layers[index], layers[index - 1]);
}
void Document::duplicateLayer(size_t index) {
  if (index >= layers.size())
    return;

  Layer copy = layers[index];

  copy.setName(copy.getName() + " Copy");

  layers.insert(layers.begin() + index + 1, copy);
}
std::vector<Layer> &Document::getLayers() { return layers; }

void Document::renameLayer(size_t index, const std::string &name) {
  if (index >= layers.size()) {
    return;
  }

  layers[index].setName(name);
}

void Document::reorderLayers(int srcRow, int destRow) {
  if (srcRow < 0 || srcRow >= static_cast<int>(layers.size()))
    return;
  if (destRow < 0 || destRow >= static_cast<int>(layers.size()))
    return;
  if (srcRow == destRow)
    return;

  auto it = layers.begin() + srcRow;
  Layer layerToMove = std::move(*it);
  layers.erase(it);
  layers.insert(layers.begin() + destRow, std::move(layerToMove));
}

int Document::getWidth() const { return width; }

int Document::getHeight() const { return height; }
const std::vector<Layer> &Document::getLayers() const { return layers; }
