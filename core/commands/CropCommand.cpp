#include "CropCommand.hpp"

CropCommand::CropCommand(Document &document, const Selection &selection)
    : document(document), selection(selection) {}

void CropCommand::execute() {
  // TODO:
  // Actual image cropping
}

void CropCommand::undo() {
  // TODO:
  // Restore original image
}