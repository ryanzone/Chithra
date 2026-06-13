#include "Document.hpp"

Document::Document(int width, int height) : width(width), height(height) {}

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
  if (index >= layers.size()) {
    return;
  }

  layers.insert(layers.begin() + index + 1, layers[index]);
}
std::vector<Layer> &Document::getLayers() { return layers; }

void Document::renameLayer(size_t index, const std::string &name) {
  if (index >= layers.size()) {
    return;
  }

  layers[index].setName(name);
}

int Document::getWidth() const { return width; }

int Document::getHeight() const { return height; }
const std::vector<Layer> &Document::getLayers() const { return layers; }
