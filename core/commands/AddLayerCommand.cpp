#include "AddLayerCommand.hpp"

AddLayerCommand::AddLayerCommand(Document &document,
                                 const std::string &layerName)
    : document(document), layerName(layerName), layerIndex(0) {}

void AddLayerCommand::execute() { layerIndex = document.addLayer(layerName); }

void AddLayerCommand::undo() {
  if (layerIndex < document.getLayers().size()) {
    document.removeLayer(layerIndex);
  }
}