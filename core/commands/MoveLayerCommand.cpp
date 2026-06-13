#include "MoveLayerCommand.hpp"

MoveLayerCommand::MoveLayerCommand(Document &document, size_t fromIndex,
                                   size_t toIndex)
    : document(document), fromIndex(fromIndex), toIndex(toIndex) {}

void MoveLayerCommand::execute() {
  if (toIndex > fromIndex) {
    document.moveLayerUp(fromIndex);
  } else {
    document.moveLayerDown(fromIndex);
  }
}

void MoveLayerCommand::undo() {
  if (toIndex > fromIndex) {
    document.moveLayerDown(toIndex);
  } else {
    document.moveLayerUp(toIndex);
  }
}