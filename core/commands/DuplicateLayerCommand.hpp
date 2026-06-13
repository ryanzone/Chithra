#pragma once

#include "../document/Document.hpp"
#include "Command.hpp"


class DuplicateLayerCommand : public Command {
private:
  Document &document;

  size_t index;

public:
  DuplicateLayerCommand(Document &document, size_t index);

  void execute() override;

  void undo() override;
};