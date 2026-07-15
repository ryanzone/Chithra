#include "LayerManager.hpp"

LayerManager::LayerManager(Document &document)
    : document(document), activeLayerIndex(-1) {}

void LayerManager::addLayer(const std::string &name) {
  document.addLayer(name);

  activeLayerIndex = static_cast<int>(document.getLayers().size()) - 1;
}

void LayerManager::removeLayer(size_t index) {
  if (index >= document.getLayers().size()) {
    return;
  }

  if (activeLayerIndex > static_cast<int>(index)) {
    activeLayerIndex--;
  }

  document.removeLayer(index);

  if (document.getLayers().empty()) {
    activeLayerIndex = -1;
    return;
  }

  if (activeLayerIndex >= static_cast<int>(document.getLayers().size())) {
    activeLayerIndex = static_cast<int>(document.getLayers().size()) - 1;
  }
}

void LayerManager::setActiveLayer(int index) {
  if (index < 0) {
    activeLayerIndex = -1;
  } else if (static_cast<size_t>(index) < document.getLayers().size()) {
    activeLayerIndex = index;
  }
}
void LayerManager::clearActiveLayer() { activeLayerIndex = -1; }
Layer *LayerManager::getActiveLayer() {
  if (activeLayerIndex < 0) {
    return nullptr;
  }

  if (activeLayerIndex >= static_cast<int>(document.getLayers().size())) {
    return nullptr;
  }

  return &document.getLayers()[activeLayerIndex];
}

int LayerManager::getActiveLayerIndex() const { return activeLayerIndex; }
void LayerManager::reorderLayers(int srcRow, int destRow) {
  document.reorderLayers(srcRow, destRow);

  if (activeLayerIndex == srcRow) {
    activeLayerIndex = destRow;
  } else if (srcRow < activeLayerIndex && destRow >= activeLayerIndex) {
    activeLayerIndex--;
  } else if (srcRow > activeLayerIndex && destRow <= activeLayerIndex) {
    activeLayerIndex++;
  }
}