#pragma once

#include "Tool.hpp"

#include "../selection/Selection.hpp"

class MoveTool : public Tool {
private:
  Selection *selection;

public:
  MoveTool(Selection *selection);

  void apply(ToolContext &context, int x, int y) override;
};