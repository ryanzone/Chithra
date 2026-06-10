#pragma once

#include "Tool.hpp"

#include "../graphics/Color.hpp"
#include "../image/Image.hpp"
#include "ToolContext.hpp"

class BrushTool : public Tool {
private:
  int brushSize;
  Color color;

public:
  BrushTool(int size = 10, Color color = Color(255, 0, 0, 255));

  void apply(ToolContext &context, int x, int y) override;

  void draw(Image &image, int centerX, int centerY);

  void setBrushSize(int size);

  void setColor(const Color &color);
};