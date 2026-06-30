#include "Layer.hpp"

Layer::Layer(const std::string &name, int width, int height)
    : name(name), image(width, height), visible(true), opacity(1.0f),
      blendMode(BlendMode::Normal) {}

const std::string &Layer::getName() const { return name; }

bool Layer::isVisible() const { return visible; }

float Layer::getOpacity() const { return opacity; }

void Layer::setOpacity(float opacity) { this->opacity = opacity; }

void Layer::setVisible(bool visible) { this->visible = visible; }

const Image &Layer::getImage() const { return image; }

Image &Layer::getImage() { return image; }
BlendMode Layer::getBlendMode() const { return blendMode; }

void Layer::setBlendMode(BlendMode mode) { blendMode = mode; }
void Layer::setName(const std::string &name) { this->name = name; }
