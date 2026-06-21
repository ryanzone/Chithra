#pragma once

#include "Command.hpp"

#include "../selection/Selection.hpp"

class MoveSelectionCommand : public Command {
private:
  Selection &selection;

  int oldStartX;
  int oldStartY;

  int oldEndX;
  int oldEndY;

  int newStartX;
  int newStartY;

public:
  MoveSelectionCommand(Selection &selection, int newX, int newY);

  void execute() override;

  void undo() override;
};