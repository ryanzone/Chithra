#pragma once

#include "Command.hpp"

#include "../document/Document.hpp"

class MoveLayerCommand : public Command {
private:
  Document &document;

  size_t fromIndex;
  size_t toIndex;

public:
  MoveLayerCommand(Document &document, size_t fromIndex, size_t toIndex);

  void execute() override;
  void undo() override;
};