#pragma once

#include <memory>
#include <stack>

#include "../commands/Command.hpp"

class HistoryManager {
private:
  std::stack<std::unique_ptr<Command>> undoStack;
  std::stack<std::unique_ptr<Command>> redoStack;

public:
  void executeCommand(std::unique_ptr<Command> command);

  void undo();

  void redo();
};