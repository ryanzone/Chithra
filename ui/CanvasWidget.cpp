#include "CanvasWidget.hpp"

#include "../core/image/Image.hpp"
#include "../core/tools/ToolContext.hpp"
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <algorithm>

CanvasWidget::CanvasWidget(QWidget *parent)
    : QWidget(parent), document(800, 600), layerManager(document),
      brushTool(10), eraserTool(10), fillTool(Color(255, 0, 0, 255)),
      moveTool(&selectionTool.getSelection()),
      shapeTool(ShapeType::Rectangle, Color(0, 0, 0, 255), 2, false) {
  toolManager.setCurrentTool(&brushTool);
  setMinimumSize(400, 300);
  setMouseTracking(true);
}

QPoint CanvasWidget::canvasOffset() const {
  int x = (width() - document.getWidth()) / 2;
  int y = (height() - document.getHeight()) / 2;
  return QPoint(x, y);
}

QPoint CanvasWidget::toCanvasCoords(QMouseEvent *event) const {
  QPoint off = canvasOffset();
  int x = static_cast<int>(event->position().x()) - off.x();
  int y = static_cast<int>(event->position().y()) - off.y();
  return QPoint(x, y);
}

void CanvasWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, false);

  // Set the canvas workspace background to #000326
  painter.fillRect(rect(), QColor(0, 3, 38));

  QPoint off = canvasOffset();

  Image rendered = canvas.render(document);

  QImage image(rendered.getPixels().data(), rendered.getWidth(),
               rendered.getHeight(), rendered.getWidth() * 4,
               QImage::Format_RGBA8888);

  painter.drawImage(off.x(), off.y(), image);

  // Selection overlay
  auto &sel = selectionTool.getSelection();
  if (sel.active) {
    painter.setPen(QPen(QColor(0, 120, 212), 1, Qt::DashLine));
    painter.setBrush(QColor(0, 120, 212, 30));
    painter.drawRect(off.x() + sel.startX, off.y() + sel.startY, sel.width(),
                     sel.height());
  }

  // Shape preview overlay
  if (shapeTool.isDragging() && toolManager.getCurrentTool() == &shapeTool) {
    painter.setPen(QPen(QColor(0, 120, 212), 1, Qt::DashLine));
    painter.setBrush(Qt::NoBrush);

    int sx = off.x() + shapeTool.getStartX();
    int sy = off.y() + shapeTool.getStartY();
    int ex = off.x() + shapeTool.getEndX();
    int ey = off.y() + shapeTool.getEndY();

    int left = std::min(sx, ex);
    int top = std::min(sy, ey);
    int w = std::abs(ex - sx);
    int h = std::abs(ey - sy);

    switch (shapeTool.getShapeType()) {
    case ShapeType::Rectangle:
      painter.drawRect(left, top, w, h);
      break;
    case ShapeType::Ellipse:
      painter.drawEllipse(left, top, w, h);
      break;
    case ShapeType::Line:
      painter.drawLine(sx, sy, ex, ey);
      break;
    }
  }
}

Document &CanvasWidget::getDocument() { return document; }
const Document &CanvasWidget::getDocument() const { return document; }

void CanvasWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() != Qt::LeftButton)
    return;

  painting = true;
  QPoint pos = toCanvasCoords(event);
  int x = pos.x();
  int y = pos.y();

  Tool *tool = toolManager.getCurrentTool();
  if (tool) {
    ToolContext context{layerManager, document.getCanvas()};
    tool->onPress(context, x, y);
  }

  lastPoint = pos;
  update();
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
  QPoint pos = toCanvasCoords(event);
  emit cursorMoved(pos.x(), pos.y());

  if (!painting)
    return;

  int x = pos.x();
  int y = pos.y();

  Tool *tool = toolManager.getCurrentTool();
  if (!tool)
    return;

  ToolContext context{layerManager, document.getCanvas()};

  if (tool->needsPreview()) {
    tool->onMove(context, x, y);
  } else {
    // Interpolate for brush/eraser
    int dx = x - lastPoint.x();
    int dy = y - lastPoint.y();
    int steps = std::max(std::abs(dx), std::abs(dy));

    if (steps == 0) {
      tool->onMove(context, x, y);
    } else {
      for (int i = 0; i <= steps; i++) {
        float t = static_cast<float>(i) / steps;
        int px = static_cast<int>(lastPoint.x() + dx * t);
        int py = static_cast<int>(lastPoint.y() + dy * t);
        tool->onMove(context, px, py);
      }
    }
  }

  lastPoint = pos;
  update();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (!painting)
    return;

  painting = false;
  QPoint pos = toCanvasCoords(event);

  Tool *tool = toolManager.getCurrentTool();
  if (tool) {
    ToolContext context{layerManager, document.getCanvas()};
    tool->onRelease(context, pos.x(), pos.y());
  }

  update();
}

void CanvasWidget::setBrushTool() { toolManager.setCurrentTool(&brushTool); }
void CanvasWidget::setEraserTool() { toolManager.setCurrentTool(&eraserTool); }
void CanvasWidget::setFillTool() { toolManager.setCurrentTool(&fillTool); }
void CanvasWidget::setSelectionTool() {
  toolManager.setCurrentTool(&selectionTool);
}
void CanvasWidget::setMoveTool() { toolManager.setCurrentTool(&moveTool); }
void CanvasWidget::setShapeTool() { toolManager.setCurrentTool(&shapeTool); }

void CanvasWidget::addLayer() {
  int count = static_cast<int>(document.getLayers().size());
  layerManager.addLayer("Layer " + std::to_string(count + 1));
  emit layersChanged();
  update();
}

void CanvasWidget::setActiveLayer(int index) {
  layerManager.setActiveLayer(index);
  update();
}

int CanvasWidget::getActiveLayerIndex() const {
  return layerManager.getActiveLayerIndex();
}

void CanvasWidget::selectCanvas() {
  layerManager.clearActiveLayer();
  update();
}

void CanvasWidget::removeLayer(int index) {
  layerManager.removeLayer(index);
  emit layersChanged();
  update();
}

void CanvasWidget::renameLayer(int index, const std::string &name) {
  document.renameLayer(index, name);
  emit layersChanged();
  update();
}

void CanvasWidget::reorderLayers(int srcRow, int destRow) {
  layerManager.reorderLayers(srcRow, destRow);
  emit layersChanged();
  update();
}

void CanvasWidget::toggleLayerVisibility(int index) {
  auto &layers = document.getLayers();
  if (index < 0 || index >= static_cast<int>(layers.size()))
    return;
  layers[index].setVisible(!layers[index].isVisible());
  update();
}

void CanvasWidget::setBrushColor(int r, int g, int b) {
  Color color(r, g, b, 255);
  brushTool.setColor(color);
  fillTool.setColor(color);
  shapeTool.setColor(color);
}

void CanvasWidget::duplicateLayer(int index) {
  document.duplicateLayer(index);
  emit layersChanged();
  update();
}

void CanvasWidget::clearActiveLayer() {
  Layer *layer = layerManager.getActiveLayer();
  if (!layer)
    return;

  auto &pixels = layer->getImage().getPixels();
  std::fill(pixels.begin(), pixels.end(), 0);
  update();
}

void CanvasWidget::clearAll() {
  auto &canvasPixels = document.getCanvas().getPixels();
  for (size_t i = 0; i < canvasPixels.size(); i += 4) {
    canvasPixels[i] = 255;
    canvasPixels[i + 1] = 255;
    canvasPixels[i + 2] = 255;
    canvasPixels[i + 3] = 255;
  }

  for (auto &layer : document.getLayers()) {
    auto &pixels = layer.getImage().getPixels();
    std::fill(pixels.begin(), pixels.end(), 0);
  }

  update();
}

void CanvasWidget::setBrushSize(int size) { brushTool.setBrushSize(size); }
void CanvasWidget::setEraserSize(int size) { eraserTool.setSize(size); }

// Return Image& instead of Canvas& to expose the underlying pixel buffer to
// MainWindow
Image &CanvasWidget::getCanvas() { return document.getCanvas(); }
const Image &CanvasWidget::getCanvas() const { return document.getCanvas(); }