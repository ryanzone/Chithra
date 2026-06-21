#include "ProjectSerializer.hpp"

#include <fstream>
#include <string>

bool ProjectSerializer::save(const Document &document,
                             const std::string &path) {
  std::ofstream file(path);

  if (!file) {
    return false;
  }

  file << "CHI1\n";

  file << document.getWidth() << "\n";
  file << document.getHeight() << "\n";

  file << document.getLayers().size() << "\n";

  for (const auto &layer : document.getLayers()) {
    file << layer.getName() << "\n";

    file << layer.isVisible() << "\n";

    file << layer.getOpacity() << "\n";

    file << static_cast<int>(layer.getBlendMode()) << "\n";
    const auto &pixels = layer.getImage().getPixels();

    file << pixels.size() << "\n";

    for (uint8_t pixel : pixels) {
      file << static_cast<int>(pixel) << " ";
    }

    file << "\n";
  }

  return true;
}
bool ProjectSerializer::load(Document &document, const std::string &path) {
  std::ifstream file(path);

  if (!file) {
    return false;
  }

  std::string header;

  std::getline(file, header);

  if (header != "CHI1") {
    return false;
  }

  int width;
  int height;

  file >> width;
  file >> height;

  size_t layerCount;

  file >> layerCount;

  Document loaded(width, height);

  std::string dummy;
  std::getline(file, dummy);

  for (size_t i = 0; i < layerCount; i++) {
    std::string name;

    std::getline(file, name);

    loaded.addLayer(name);

    int visible;

    float opacity;

    int blendMode;

    file >> visible;
    file >> opacity;
    file >> blendMode;

    std::getline(file, dummy);

    auto &layer = loaded.getLayers().back();

    layer.setVisible(visible);

    layer.setOpacity(opacity);

    layer.setBlendMode(static_cast<BlendMode>(blendMode));

    size_t pixelCount;

    file >> pixelCount;

    auto &pixels = layer.getImage().getPixels();

    pixels.resize(pixelCount);
  }

  document = loaded;

  return true;
}