#include "MoveSelectionCommand.hpp"

MoveSelectionCommand::MoveSelectionCommand(Selection &selection, int newX,
                                           int newY)
    : selection(selection), oldStartX(selection.startX),
      oldStartY(selection.startY), oldEndX(selection.endX),
      oldEndY(selection.endY), newStartX(newX), newStartY(newY) {}

void MoveSelectionCommand::execute() {
  int width = oldEndX - oldStartX;

  int height = oldEndY - oldStartY;

  selection.startX = newStartX;

  selection.startY = newStartY;

  selection.endX = newStartX + width;

  selection.endY = newStartY + height;
}

void MoveSelectionCommand::undo() {
  selection.startX = oldStartX;

  selection.startY = oldStartY;

  selection.endX = oldEndX;

  selection.endY = oldEndY;
}