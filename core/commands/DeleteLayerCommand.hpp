#pragma once

#include "../document/Document.hpp"
#include "../document/Layer.hpp"
#include "Command.hpp"


class DeleteLayerCommand : public Command {
private:
  Document &document;

  size_t index;

  Layer deletedLayer;

public:
  DeleteLayerCommand(Document &document, size_t index);

  void execute() override;

  void undo() override;
};