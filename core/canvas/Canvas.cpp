#include "Canvas.hpp"

Image Canvas::render(const Document &document) {
  Image result(document.getWidth(), document.getHeight());

  auto &resultPixels = result.getPixels();

  for (const auto &layer : document.getLayers()) {
    if (!layer.isVisible()) {
      continue;
    }

    const auto &layerPixels = layer.getImage().getPixels();

    float opacity = layer.getOpacity();

    for (size_t i = 0; i < resultPixels.size(); i += 4) {
      float srcAlpha = (layerPixels[i + 3] / 255.0f) * opacity;

      float dstAlpha = resultPixels[i + 3] / 255.0f;

      float outAlpha = srcAlpha + dstAlpha * (1.0f - srcAlpha);

      if (outAlpha <= 0.0f) {
        continue;
      }

      resultPixels[i] = static_cast<uint8_t>(
          (layerPixels[i] * srcAlpha +
           resultPixels[i] * dstAlpha * (1.0f - srcAlpha)) /
          outAlpha);

      resultPixels[i + 1] = static_cast<uint8_t>(
          (layerPixels[i + 1] * srcAlpha +
           resultPixels[i + 1] * dstAlpha * (1.0f - srcAlpha)) /
          outAlpha);

      resultPixels[i + 2] = static_cast<uint8_t>(
          (layerPixels[i + 2] * srcAlpha +
           resultPixels[i + 2] * dstAlpha * (1.0f - srcAlpha)) /
          outAlpha);

      resultPixels[i + 3] = static_cast<uint8_t>(outAlpha * 255.0f);
    }
  }

  return result;
}