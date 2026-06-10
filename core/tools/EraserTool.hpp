#pragma once

#include "Tool.hpp"

#include "../image/Image.hpp"
#include "ToolContext.hpp"

class EraserTool : public Tool {
private:
  int eraserSize;

public:
  EraserTool(int size = 10);

  void apply(ToolContext &context, int x, int y) override;

  void erase(Image &image, int centerX, int centerY);

  void setSize(int size);
};