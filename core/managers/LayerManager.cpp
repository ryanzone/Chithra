#include "LayerManager.hpp"

LayerManager::LayerManager(Document &document)
    : document(document), activeLayerIndex(0) {}

void LayerManager::addLayer(const std::string &name) {
  document.addLayer(name);

  activeLayerIndex = document.getLayers().size() - 1;
}

void LayerManager::removeLayer(size_t index) {
  document.removeLayer(index);

  if (activeLayerIndex >= document.getLayers().size() &&
      !document.getLayers().empty()) {
    activeLayerIndex = document.getLayers().size() - 1;
  }
}

void LayerManager::setActiveLayer(size_t index) {
  if (index < document.getLayers().size()) {
    activeLayerIndex = index;
  }
}

Layer *LayerManager::getActiveLayer() {
  if (document.getLayers().empty()) {
    return nullptr;
  }

  return &document.getLayers()[activeLayerIndex];
}

size_t LayerManager::getActiveLayerIndex() const { return activeLayerIndex; }