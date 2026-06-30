#include "ToolManager.hpp"

void ToolManager::setCurrentTool(Tool *tool) { currentTool = tool; }

Tool *ToolManager::getCurrentTool() const { return currentTool; }