#pragma once

#include "Command.hpp"

#include "../selection/Selection.hpp"

class MoveSelectionCommand : public Command {
private:
  Selection &selection;

  int oldX;
  int oldY;

  int newX;
  int newY;

public:
  MoveSelectionCommand(Selection &selection, int newX, int newY);

  void execute() override;

  void undo() override;
};