#include "DeleteLayerCommand.hpp"

DeleteLayerCommand::DeleteLayerCommand(Document &document, size_t index)
    : document(document), index(index),
      deletedLayer(document.getLayers()[index]) {}

void DeleteLayerCommand::execute() { document.removeLayer(index); }

void DeleteLayerCommand::undo() {
  document.getLayers().insert(document.getLayers().begin() + index,
                              deletedLayer);
}