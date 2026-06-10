#include "Canvas.hpp"

Image Canvas::render(const Document &document) {
  Image result(document.getWidth(), document.getHeight());

  auto &resultPixels = result.getPixels();

  for (const auto &layer : document.getLayers()) {
    if (!layer.isVisible())
      continue;

    const auto &layerPixels = layer.getImage().getPixels();

    resultPixels = layerPixels;
  }

  return result;
}