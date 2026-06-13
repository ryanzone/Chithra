#include "MoveTool.hpp"

MoveTool::MoveTool(Selection *selection) : selection(selection) {}

void MoveTool::apply(ToolContext &context, int x, int y) {
  if (!selection) {
    return;
  }

  if (!selection->active) {
    return;
  }

  selection->x = x;
  selection->y = y;
}