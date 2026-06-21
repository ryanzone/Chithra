#include "CropTool.hpp"

CropTool::CropTool(Selection *selection) : selection(selection) {}

void CropTool::apply(ToolContext &context, int x, int y) {
  if (!selection) {
    return;
  }

  if (!selection->active) {
    return;
  }

  selection->endX = x;
  selection->endY = y;
}