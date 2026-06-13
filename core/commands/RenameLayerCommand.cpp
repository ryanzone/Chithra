#include "RenameLayerCommand.hpp"

RenameLayerCommand::RenameLayerCommand(Document &document, size_t index,
                                       const std::string &newName)
    : document(document), index(index),
      oldName(document.getLayers()[index].getName()), newName(newName) {}

void RenameLayerCommand::execute() { document.renameLayer(index, newName); }

void RenameLayerCommand::undo() { document.renameLayer(index, oldName); }