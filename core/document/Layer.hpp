#pragma once

#include "../image/Image.hpp"
#include <string>

enum class BlendMode { Normal, Multiply, Screen, Overlay };
class Layer {
private:
  std::string name;
  Image image;
  bool visible;
  float opacity;

  BlendMode blendMode;

public:
  Layer(const std::string &name, int width, int height);
  BlendMode getBlendMode() const;
  void setBlendMode(BlendMode mode);
  const std::string &getName() const;
  void setName(const std::string &name);
  bool isVisible() const;
  float getOpacity() const;
  const Image &getImage() const;
  void setVisible(bool visible);
  void setOpacity(float opacity);
  Image &getImage();
};