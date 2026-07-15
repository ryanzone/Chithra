#pragma once

#include "ToolContext.hpp"

class Tool {
public:
  virtual ~Tool() = default;

  virtual void apply(ToolContext &context, int x, int y) = 0;

  virtual void onPress(ToolContext &context, int x, int y) {
    apply(context, x, y);
  }

  virtual void onMove(ToolContext &context, int x, int y) {
    apply(context, x, y);
  }

  virtual void onRelease(ToolContext &context, int x, int y) {
    (void)context;
    (void)x;
    (void)y;
  }

  virtual bool needsPreview() const { return false; }
};