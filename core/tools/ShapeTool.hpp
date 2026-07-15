#pragma once

#include "Tool.hpp"
#include "../graphics/Color.hpp"
#include "../image/Image.hpp"

enum class ShapeType { Rectangle, Ellipse, Line };

class ShapeTool : public Tool {
private:
  ShapeType shapeType = ShapeType::Rectangle;
  Color color;
  int thickness;
  bool filled;

  int startX = 0, startY = 0;
  int endX = 0, endY = 0;
  bool dragging = false;

public:
  ShapeTool(ShapeType type = ShapeType::Rectangle,
            Color color = Color(0, 0, 0, 255), int thickness = 2,
            bool filled = false);

  void setShapeType(ShapeType type);
  ShapeType getShapeType() const;
  void setColor(const Color &color);
  void setThickness(int t);
  void setFilled(bool f);

  void apply(ToolContext &context, int x, int y) override;
  void onPress(ToolContext &context, int x, int y) override;
  void onMove(ToolContext &context, int x, int y) override;
  void onRelease(ToolContext &context, int x, int y) override;
  bool needsPreview() const override { return true; }

  bool isDragging() const { return dragging; }
  int getStartX() const { return startX; }
  int getStartY() const { return startY; }
  int getEndX() const { return endX; }
  int getEndY() const { return endY; }

  void drawShape(Image &image, int x1, int y1, int x2, int y2);

private:
  void drawRect(Image &image, int x1, int y1, int x2, int y2);
  void drawFilledRect(Image &image, int x1, int y1, int x2, int y2);
  void drawEllipse(Image &image, int x1, int y1, int x2, int y2);
  void drawFilledEllipse(Image &image, int x1, int y1, int x2, int y2);
  void drawLine(Image &image, int x1, int y1, int x2, int y2);
  void setPixel(Image &image, int x, int y);
};
