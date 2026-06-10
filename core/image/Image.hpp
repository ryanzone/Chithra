#pragma once

#include <cstdint>
#include <vector>

class Image {
private:
  int width;
  int height;
  std::vector<uint8_t> pixels;

public:
  Image();
  Image(int width, int height);

  int getWidth() const;
  int getHeight() const;

  std::vector<uint8_t> &getPixels();
  const std::vector<uint8_t> &getPixels() const;
};