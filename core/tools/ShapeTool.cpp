#include "ShapeTool.hpp"
#include "../document/Layer.hpp"
#include <algorithm>
#include <cmath>

ShapeTool::ShapeTool(ShapeType type, Color color, int thickness, bool filled)
    : shapeType(type), color(color), thickness(thickness), filled(filled) {}

void ShapeTool::setShapeType(ShapeType type) { shapeType = type; }
ShapeType ShapeTool::getShapeType() const { return shapeType; }
void ShapeTool::setColor(const Color &c) { color = c; }
void ShapeTool::setThickness(int t) { thickness = std::max(1, t); }
void ShapeTool::setFilled(bool f) { filled = f; }

void ShapeTool::apply(ToolContext &context, int x, int y) {
  (void)context;
  (void)x;
  (void)y;
}

void ShapeTool::onPress(ToolContext &context, int x, int y) {
  (void)context;
  startX = x;
  startY = y;
  endX = x;
  endY = y;
  dragging = true;
}

void ShapeTool::onMove(ToolContext &context, int x, int y) {
  (void)context;
  if (dragging) {
    endX = x;
    endY = y;
  }
}

void ShapeTool::onRelease(ToolContext &context, int x, int y) {
  if (!dragging)
    return;

  endX = x;
  endY = y;
  dragging = false;

  Layer *layer = context.layerManager.getActiveLayer();
  Image *image = layer ? &layer->getImage() : &context.canvas;

  drawShape(*image, startX, startY, endX, endY);
}

void ShapeTool::setPixel(Image &image, int x, int y) {
  int w = image.getWidth();
  int h = image.getHeight();
  if (x < 0 || y < 0 || x >= w || y >= h)
    return;

  auto &pixels = image.getPixels();
  int idx = (y * w + x) * 4;
  pixels[idx] = color.r;
  pixels[idx + 1] = color.g;
  pixels[idx + 2] = color.b;
  pixels[idx + 3] = color.a;
}

void ShapeTool::drawShape(Image &image, int x1, int y1, int x2, int y2) {
  switch (shapeType) {
  case ShapeType::Rectangle:
    if (filled)
      drawFilledRect(image, x1, y1, x2, y2);
    else
      drawRect(image, x1, y1, x2, y2);
    break;
  case ShapeType::Ellipse:
    if (filled)
      drawFilledEllipse(image, x1, y1, x2, y2);
    else
      drawEllipse(image, x1, y1, x2, y2);
    break;
  case ShapeType::Line:
    drawLine(image, x1, y1, x2, y2);
    break;
  }
}

void ShapeTool::drawRect(Image &image, int x1, int y1, int x2, int y2) {
  int left = std::min(x1, x2);
  int right = std::max(x1, x2);
  int top = std::min(y1, y2);
  int bottom = std::max(y1, y2);

  for (int t = 0; t < thickness; t++) {
    for (int x = left - t; x <= right + t; x++) {
      setPixel(image, x, top - t);
      setPixel(image, x, bottom + t);
    }
    for (int y = top - t; y <= bottom + t; y++) {
      setPixel(image, left - t, y);
      setPixel(image, right + t, y);
    }
  }
}

void ShapeTool::drawFilledRect(Image &image, int x1, int y1, int x2, int y2) {
  int left = std::min(x1, x2);
  int right = std::max(x1, x2);
  int top = std::min(y1, y2);
  int bottom = std::max(y1, y2);

  for (int y = top; y <= bottom; y++) {
    for (int x = left; x <= right; x++) {
      setPixel(image, x, y);
    }
  }
}

void ShapeTool::drawEllipse(Image &image, int x1, int y1, int x2, int y2) {
  int cx = (x1 + x2) / 2;
  int cy = (y1 + y2) / 2;
  int rx = std::abs(x2 - x1) / 2;
  int ry = std::abs(y2 - y1) / 2;

  if (rx == 0 || ry == 0)
    return;

  for (int angle = 0; angle < 3600; angle++) {
    double rad = angle * 3.14159265358979 / 1800.0;
    double fx = cx + rx * std::cos(rad);
    double fy = cy + ry * std::sin(rad);

    for (int t = 0; t < thickness; t++) {
      double fx2 = cx + (rx + t) * std::cos(rad);
      double fy2 = cy + (ry + t) * std::sin(rad);
      setPixel(image, static_cast<int>(fx2), static_cast<int>(fy2));
    }
    setPixel(image, static_cast<int>(fx), static_cast<int>(fy));
  }
}

void ShapeTool::drawFilledEllipse(Image &image, int x1, int y1, int x2,
                                  int y2) {
  int cx = (x1 + x2) / 2;
  int cy = (y1 + y2) / 2;
  int rx = std::abs(x2 - x1) / 2;
  int ry = std::abs(y2 - y1) / 2;

  if (rx == 0 || ry == 0)
    return;

  for (int y = cy - ry; y <= cy + ry; y++) {
    for (int x = cx - rx; x <= cx + rx; x++) {
      double dx = static_cast<double>(x - cx) / rx;
      double dy = static_cast<double>(y - cy) / ry;
      if (dx * dx + dy * dy <= 1.0) {
        setPixel(image, x, y);
      }
    }
  }
}

void ShapeTool::drawLine(Image &image, int x1, int y1, int x2, int y2) {
  int dx = std::abs(x2 - x1);
  int dy = std::abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;

  while (true) {
    for (int t = -(thickness / 2); t <= thickness / 2; t++) {
      setPixel(image, x1 + t, y1);
      setPixel(image, x1, y1 + t);
    }

    if (x1 == x2 && y1 == y2)
      break;

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
}
