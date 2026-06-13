#include <iostream>
#include <memory>

#include "../../core/document/Document.hpp"

#include "../../core/history/HistoryManager.hpp"

#include "../../core/commands/MoveLayerCommand.hpp"

int main() {
  Document doc(512, 512);

  doc.addLayer("Layer A");
  doc.addLayer("Layer B");
  doc.addLayer("Layer C");

  HistoryManager history;

  std::cout << "Before Move:" << std::endl;

  for (const auto &layer : doc.getLayers()) {
    std::cout << layer.getName() << std::endl;
  }

  history.executeCommand(std::make_unique<MoveLayerCommand>(doc, 0, 1));

  std::cout << "\nAfter Move:" << std::endl;

  for (const auto &layer : doc.getLayers()) {
    std::cout << layer.getName() << std::endl;
  }

  history.undo();

  std::cout << "\nAfter Undo:" << std::endl;

  for (const auto &layer : doc.getLayers()) {
    std::cout << layer.getName() << std::endl;
  }

  return 0;
}