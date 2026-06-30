#pragma once

#include "Layer.hpp"
#include <vector>

class Document {
private:
  int width;
  int height;

  Image canvas;
  std::vector<Layer> layers;

public:
  Document(int width, int height);

  Image &getCanvas();
  const Image &getCanvas() const;

  size_t addLayer(const std::string &name);
  void removeLayer(size_t index);
  void moveLayerUp(size_t index);
  void moveLayerDown(size_t index);
  void duplicateLayer(size_t index);
  void renameLayer(size_t index, const std::string &name);
  void reorderLayers(int srcRow, int destRow);

  std::vector<Layer> &getLayers();
  const std::vector<Layer> &getLayers() const;

  int getWidth() const;
  int getHeight() const;
};