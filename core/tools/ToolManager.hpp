#pragma once

#include "Tool.hpp"

class ToolManager {
private:
  Tool *currentTool = nullptr;

public:
  void setCurrentTool(Tool *tool);

  Tool *getCurrentTool() const;
};