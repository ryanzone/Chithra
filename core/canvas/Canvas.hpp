#pragma once

#include "../document/Document.hpp"
#include "../image/Image.hpp"

class Canvas {
public:
  Image render(const Document &document);
};