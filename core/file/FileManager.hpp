#pragma once

#include "../image/Image.hpp"

#include <string>

class FileManager {
public:
  Image openImage(const std::string &filepath);

  bool saveImage(const Image &image, const std::string &filepath);
};