#include "Image.hpp"

Image::Image() : width(0), height(0) {}

Image::Image(int width, int height)
    : width(width), height(height), pixels(width * height * 4, 0) {
  for (int i = 0; i < width * height; i++) {
    pixels[i * 4 + 3] = 255;
  }
}

int Image::getWidth() const { return width; }

int Image::getHeight() const { return height; }

std::vector<uint8_t> &Image::getPixels() { return pixels; }

const std::vector<uint8_t> &Image::getPixels() const { return pixels; }