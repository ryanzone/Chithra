#pragma once

#include "Tool.hpp"

#include "../graphics/Color.hpp"

class FillTool : public Tool {
private:
  Color fillColor;

public:
  FillTool(Color color = Color(255, 255, 0));
  void setColor(const Color &color);
  void apply(ToolContext &context, int x, int y) override;
};