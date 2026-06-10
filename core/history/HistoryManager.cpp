#include "HistoryManager.hpp"

void HistoryManager::executeCommand(std::unique_ptr<Command> command) {
  command->execute();

  undoStack.push(std::move(command));

  while (!redoStack.empty()) {
    redoStack.pop();
  }
}

void HistoryManager::undo() {
  if (undoStack.empty()) {
    return;
  }

  auto command = std::move(undoStack.top());

  undoStack.pop();

  command->undo();

  redoStack.push(std::move(command));
}

void HistoryManager::redo() {
  if (redoStack.empty()) {
    return;
  }

  auto command = std::move(redoStack.top());

  redoStack.pop();

  command->execute();

  undoStack.push(std::move(command));
}