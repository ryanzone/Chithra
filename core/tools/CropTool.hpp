#pragma once

#include "Tool.hpp"

#include "../selection/Selection.hpp"

class CropTool : public Tool {
private:
  Selection *selection;

public:
  CropTool(Selection *selection);

  void apply(ToolContext &context, int x, int y) override;
};