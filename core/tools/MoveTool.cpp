#include "MoveTool.hpp"

MoveTool::MoveTool(Selection *selection) : selection(selection) {}

void MoveTool::apply(ToolContext &context, int x, int y) {
  if (!selection) {
    return;
  }

  if (!selection->active) {
    return;
  }

  int width = selection->endX - selection->startX;

  int height = selection->endY - selection->startY;

  selection->startX = x;
  selection->startY = y;

  selection->endX = x + width;

  selection->endY = y + height;
}