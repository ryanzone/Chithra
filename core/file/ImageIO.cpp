#include "ImageIO.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../external/stb_image_write.h"

bool ImageIO::savePNG(const Image &image, const std::string &filepath) {
  return stbi_write_png(filepath.c_str(), image.getWidth(), image.getHeight(),
                        4, image.getPixels().data(), image.getWidth() * 4);
}