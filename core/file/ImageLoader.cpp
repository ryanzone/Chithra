#include "ImageLoader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb_image.h"

#include <stdexcept>

Image ImageLoader::loadPNG(const std::string &filepath) {
  int width;
  int height;
  int channels;

  unsigned char *data =
      stbi_load(filepath.c_str(), &width, &height, &channels, 4);

  if (!data) {
    throw std::runtime_error("Failed to load image");
  }

  Image image(width, height);

  auto &pixels = image.getPixels();

  pixels.assign(data, data + (width * height * 4));

  stbi_image_free(data);

  return image;
}