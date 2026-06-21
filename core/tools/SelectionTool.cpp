#include "SelectionTool.hpp"

void SelectionTool::apply(ToolContext &context, int x, int y) {
  selection.startX = x;
  selection.startY = y;

  selection.endX = x + 100;
  selection.endY = y + 100;

  selection.active = true;
}

Selection &SelectionTool::getSelection() { return selection; }