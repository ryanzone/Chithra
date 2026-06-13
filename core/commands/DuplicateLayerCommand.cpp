#include "DuplicateLayerCommand.hpp"

DuplicateLayerCommand::DuplicateLayerCommand(Document &document, size_t index)
    : document(document), index(index) {}

void DuplicateLayerCommand::execute() { document.duplicateLayer(index); }

void DuplicateLayerCommand::undo() { document.removeLayer(index + 1); }