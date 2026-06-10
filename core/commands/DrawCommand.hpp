#pragma once

#include "Command.hpp"

#include "../image/Image.hpp"
#include "../tools/BrushTool.hpp"

#include <cstdint>
#include <vector>


class DrawCommand : public Command {
private:
  Image &image;
  BrushTool &brush;

  int x;
  int y;

  std::vector<uint8_t> previousPixels;

public:
  DrawCommand(Image &image, BrushTool &brush, int x, int y);

  void execute() override;
  void undo() override;
};
