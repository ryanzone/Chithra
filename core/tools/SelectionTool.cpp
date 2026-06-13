#include "SelectionTool.hpp"

void SelectionTool::apply(ToolContext &context, int x, int y) {
  selection.x = x;
  selection.y = y;

  selection.width = 100;
  selection.height = 100;

  selection.active = true;
}

Selection &SelectionTool::getSelection() { return selection; }