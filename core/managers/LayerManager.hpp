#pragma once

#include "../document/Document.hpp"

class LayerManager {
private:
  Document &document;
  size_t activeLayerIndex;

public:
  LayerManager(Document &document);

  void addLayer(const std::string &name);

  void removeLayer(size_t index);

  void setActiveLayer(size_t index);

  Layer *getActiveLayer();

  size_t getActiveLayerIndex() const;
};