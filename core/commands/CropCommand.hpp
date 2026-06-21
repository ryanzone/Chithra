#pragma once

#include "Command.hpp"

#include "../document/Document.hpp"
#include "../selection/Selection.hpp"

class CropCommand : public Command {
private:
  Document &document;

  Selection selection;

public:
  CropCommand(Document &document, const Selection &selection);

  void execute() override;

  void undo() override;
};