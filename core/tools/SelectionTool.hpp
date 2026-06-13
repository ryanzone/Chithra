#pragma once

#include "Tool.hpp"

#include "../selection/Selection.hpp"

class SelectionTool : public Tool {
private:
  Selection selection;

public:
  void apply(ToolContext &context, int x, int y) override;

  Selection &getSelection();
};