#include "MoveSelectionCommand.hpp"

MoveSelectionCommand::MoveSelectionCommand(Selection &selection, int newX,
                                           int newY)
    : selection(selection), oldX(selection.x), oldY(selection.y), newX(newX),
      newY(newY) {}

void MoveSelectionCommand::execute() {
  selection.x = newX;
  selection.y = newY;
}

void MoveSelectionCommand::undo() {
  selection.x = oldX;
  selection.y = oldY;
}