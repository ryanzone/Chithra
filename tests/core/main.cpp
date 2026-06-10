#include <iostream>

#include "../../core/document/Document.hpp"
#include "../../core/managers/LayerManager.hpp"

#include "../../core/tools/FillTool.hpp"
#include "../../core/tools/ToolContext.hpp"

#include "../../core/canvas/Canvas.hpp"
#include "../../core/file/ImageIO.hpp"

int main() {
  Document doc(512, 512);

  LayerManager layerManager(doc);

  layerManager.addLayer("Background");

  ToolContext context{layerManager};

  FillTool fill(Color(255, 255, 0));

  fill.apply(context, 0, 0);

  Canvas canvas;

  Image result = canvas.render(doc);

  ImageIO::savePNG(result, "filled.png");

  std::cout << "Fill complete" << std::endl;

  return 0;
}