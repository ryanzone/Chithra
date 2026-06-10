#pragma once

#include "Layer.hpp"
#include <vector>

class Document {
private:
  int width;
  int height;

  std::vector<Layer> layers;

public:
  Document(int width, int height);

  size_t addLayer(const std::string &name);
  void removeLayer(size_t index);

  std::vector<Layer> &getLayers();
  const std::vector<Layer> &getLayers() const;
  int getWidth() const;
  int getHeight() const;
};