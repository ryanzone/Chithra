#pragma once

#include "../core/canvas/Canvas.hpp"
#include "../core/document/Document.hpp"
#include "../core/managers/LayerManager.hpp"
#include "../core/tools/BrushTool.hpp"
#include "../core/tools/EraserTool.hpp"
#include "../core/tools/FillTool.hpp"
#include "../core/tools/MoveTool.hpp"
#include "../core/tools/SelectionTool.hpp"
#include "../core/tools/ShapeTool.hpp"
#include "../core/tools/ToolManager.hpp"
#include <QMouseEvent>
#include <QPoint>
#include <QWidget>

class CanvasWidget : public QWidget {
  Q_OBJECT

public:
  explicit CanvasWidget(QWidget *parent = nullptr);
  void setBrushTool();
  void setEraserTool();
  void setFillTool();
  void setSelectionTool();
  void setMoveTool();
  void setShapeTool();
  Image &getCanvas();
  const Image &getCanvas() const;

  Document &getDocument();
  const Document &getDocument() const;

  void addLayer();
  void setActiveLayer(int index);
  int getActiveLayerIndex() const;
  void removeLayer(int index);
  void renameLayer(int index, const std::string &name);
  void reorderLayers(int srcRow, int destRow);
  void toggleLayerVisibility(int index);
  void setBrushColor(int r, int g, int b);
  void selectCanvas();
  void duplicateLayer(int index);

  void clearActiveLayer();
  void clearAll();

  void setBrushSize(int size);
  void setEraserSize(int size);

  ShapeTool &getShapeTool() { return shapeTool; }

  int getCanvasWidth() const { return document.getWidth(); }
  int getCanvasHeight() const { return document.getHeight(); }

signals:
  void layersChanged();
  void cursorMoved(int x, int y);

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  QPoint canvasOffset() const;
  QPoint toCanvasCoords(QMouseEvent *event) const;

  Document document;
  Canvas canvas;

  LayerManager layerManager;
  ToolManager toolManager;

  BrushTool brushTool;
  EraserTool eraserTool;
  SelectionTool selectionTool;
  FillTool fillTool;
  MoveTool moveTool;
  ShapeTool shapeTool;

  bool painting = false;
  QPoint lastPoint;
};