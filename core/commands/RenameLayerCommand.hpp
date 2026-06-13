#pragma once

#include "../document/Document.hpp"
#include "Command.hpp"


#include <string>

class RenameLayerCommand : public Command {
private:
  Document &document;

  size_t index;

  std::string oldName;
  std::string newName;

public:
  RenameLayerCommand(Document &document, size_t index,
                     const std::string &newName);

  void execute() override;

  void undo() override;
};