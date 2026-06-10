#include "Layer.hpp"

Layer::Layer(const std::string &name, int width, int height)
    : name(name), visible(true), opacity(1.0f), image(width, height) {}

const std::string &Layer::getName() const { return name; }

bool Layer::isVisible() const { return visible; }

float Layer::getOpacity() const { return opacity; }

void Layer::setVisible(bool visible) { this->visible = visible; }

void Layer::setOpacity(float opacity) { this->opacity = opacity; }
const Image &Layer::getImage() const { return image; }

Image &Layer::getImage() { return image; }