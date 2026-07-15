#include <iostream>

#include "../../core/document/Document.hpp"
#include "../../core/managers/LayerManager.hpp"

#include "../../core/tools/BrushTool.hpp"
#include "../../core/tools/ToolContext.hpp"

#include "../../core/project/ProjectSerializer.hpp"

#include "../../core/canvas/Canvas.hpp"
#include "../../core/file/ImageIO.hpp"

int main() {
  Document doc(512, 512);

  LayerManager layerManager(doc);

  layerManager.addLayer("Background");

  ToolContext context{layerManager, doc.getCanvas()};

  BrushTool brush(40, Color(255, 0, 0));

  brush.apply(context, 256, 256);

  ProjectSerializer::save(doc, "test.chi");

  Document loadedDoc(1, 1);

  if (!ProjectSerializer::load(loadedDoc, "test.chi")) {
    std::cout << "Load Failed" << std::endl;

    return 1;
  }

  Canvas canvas;

  Image result = canvas.render(loadedDoc);

  ImageIO::savePNG(result, "loaded.png");

  std::cout << "Save/Load Success" << std::endl;

  return 0;
}