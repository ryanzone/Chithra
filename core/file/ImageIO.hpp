#pragma once

#include "../image/Image.hpp"
#include <string>

class ImageIO {
public:
  static bool savePNG(const Image &image, const std::string &filepath);
};