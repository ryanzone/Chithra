#pragma once

#include "../core/canvas/Canvas.hpp"
#include "../core/document/Document.hpp"
#include "../core/managers/LayerManager.hpp"
#include "../core/tools/BrushTool.hpp"
#include <QMouseEvent>
#include <QPoint>
#include <QWidget>

class CanvasWidget : public QWidget {
  Q_OBJECT

public:
  explicit CanvasWidget(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  void applyBrush(int x, int y);

  Document document;
  Canvas canvas;

  LayerManager layerManager;
  BrushTool brushTool;

  bool painting = false;

  QPoint lastPoint;
};