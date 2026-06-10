#pragma once

#include "../document/Document.hpp"
#include "Command.hpp"


#include <string>

class AddLayerCommand : public Command {
private:
  Document &document;
  std::string layerName;

  size_t layerIndex;

public:
  AddLayerCommand(Document &document, const std::string &layerName);

  void execute() override;
  void undo() override;
};