#pragma once

#include "../image/Image.hpp"
#include <string>

class Layer {
private:
  std::string name;
  bool visible;
  float opacity;

  Image image;

public:
  Layer(const std::string &name, int width, int height);

  const std::string &getName() const;
  bool isVisible() const;
  float getOpacity() const;
  const Image &getImage() const;
  void setVisible(bool visible);
  void setOpacity(float opacity);

  Image &getImage();
};