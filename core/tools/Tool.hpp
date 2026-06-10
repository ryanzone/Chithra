#pragma once

#include "ToolContext.hpp"

class Tool {
public:
  virtual ~Tool() = default;

  virtual void apply(ToolContext &context, int x, int y) = 0;
};