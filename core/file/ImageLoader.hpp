#pragma once

#include "../image/Image.hpp"
#include <string>

class ImageLoader {
public:
  static Image loadPNG(const std::string &filepath);
};