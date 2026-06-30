#pragma once

#include "../document/Document.hpp"

class LayerManager {
private:
  Document &document;
  int activeLayerIndex;

public:
  LayerManager(Document &document);

  void addLayer(const std::string &name);

  void removeLayer(size_t index);

  void setActiveLayer(int index);

  Layer *getActiveLayer();

  int getActiveLayerIndex() const;
  void clearActiveLayer();
  void reorderLayers(int srcRow, int destRow);
};