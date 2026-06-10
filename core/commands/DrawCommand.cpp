#include "DrawCommand.hpp"

DrawCommand::DrawCommand(Image &image, BrushTool &brush, int x, int y)
    : image(image), brush(brush), x(x), y(y) {}

void DrawCommand::execute() {
  previousPixels = image.getPixels();

  brush.draw(image, x, y);
}

void DrawCommand::undo() { image.getPixels() = previousPixels; }