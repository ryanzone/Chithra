#include "Document.hpp"

Document::Document(int width, int height) : width(width), height(height) {}

size_t Document::addLayer(const std::string &name) {
  layers.emplace_back(name, width, height);

  return layers.size() - 1;
}

void Document::removeLayer(size_t index) {
  if (index < layers.size()) {
    layers.erase(layers.begin() + index);
  }
}

std::vector<Layer> &Document::getLayers() { return layers; }

int Document::getWidth() const { return width; }

int Document::getHeight() const { return height; }
const std::vector<Layer> &Document::getLayers() const { return layers; }