#include "CanvasWidget.hpp"

#include "../core/image/Image.hpp"
#include "../core/tools/ToolContext.hpp"
#include <QDebug>
#include <QImage>
#include <QPainter>

// Canvas size: needs to have different canvas sizes
// when the user selects a preset size
CanvasWidget::CanvasWidget(QWidget *parent)
    : QWidget(parent), document(800, 600), layerManager(document),
      brushTool(10), eraserTool(10), fillTool(Color(255, 0, 0, 255)),
      moveTool(&selectionTool.getSelection()) {
  toolManager.setCurrentTool(&brushTool);

  setMinimumSize(800, 600);
}

void CanvasWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);

  painter.fillRect(rect(), QColor(35, 35, 35));

  Image rendered = canvas.render(document);

  QImage image(rendered.getPixels().data(), rendered.getWidth(),
               rendered.getHeight(), rendered.getWidth() * 4,
               QImage::Format_RGBA8888);

  int x = (width() - image.width()) / 2;
  int y = (height() - image.height()) / 2;

  painter.drawImage(x, y, image);
  auto &sel = selectionTool.getSelection();

  if (sel.active) {
    painter.setPen(QPen(Qt::blue, 1, Qt::DashLine));

    painter.drawRect(x + sel.startX, y + sel.startY, sel.width(), sel.height());
  }
}
Document &CanvasWidget::getDocument() { return document; }

const Document &CanvasWidget::getDocument() const { return document; }

// apply tools its in the tool manager (existing tools as of now: erase, brush)
void CanvasWidget::applyTool(int x, int y) {
  Tool *tool = toolManager.getCurrentTool();

  if (!tool)
    return;

  ToolContext context{layerManager, document.getCanvas()};

  tool->apply(context, x, y);
}

void CanvasWidget::setFillTool() {
  qDebug() << "Fill Tool";
  toolManager.setCurrentTool(&fillTool);
}

void CanvasWidget::setSelectionTool() {
  qDebug() << "Selection Tool";
  toolManager.setCurrentTool(&selectionTool);
}
void CanvasWidget::setMoveTool() {
  qDebug() << "Move Tool";
  toolManager.setCurrentTool(&moveTool);
}
// adding layers
void CanvasWidget::addLayer() {
  int count = document.getLayers().size();
  layerManager.addLayer("Layer " + std::to_string(count + 1));

  qDebug() << "Layers:" << document.getLayers().size();

  emit layersChanged();

  update();
}

// mouse press works well in 800x600 as of now, needs to have dynamic sizing
void CanvasWidget::mousePressEvent(QMouseEvent *event) {
  painting = true;

  int x = static_cast<int>(event->position().x()) - (width() - 800) / 2;

  int y = static_cast<int>(event->position().y()) - (height() - 600) / 2;

  applyTool(x, y);

  lastPoint = QPoint(x, y);

  update();
}

// mouse move works well in 800x600 as of now, needs to have dynamic sizing
void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
  if (!painting)
    return;

  int x = static_cast<int>(event->position().x()) - (width() - 800) / 2;

  int y = static_cast<int>(event->position().y()) - (height() - 600) / 2;

  int dx = x - lastPoint.x();
  int dy = y - lastPoint.y();

  int steps = std::max(std::abs(dx), std::abs(dy));

  if (steps == 0)
    return;

  for (int i = 0; i <= steps; i++) {
    float t = static_cast<float>(i) / steps;

    int px = static_cast<int>(lastPoint.x() + dx * t);

    int py = static_cast<int>(lastPoint.y() + dy * t);

    applyTool(px, py);
  }

  lastPoint = QPoint(x, y);

  update();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *) { painting = false; }

void CanvasWidget::setBrushTool() { toolManager.setCurrentTool(&brushTool); }

void CanvasWidget::setEraserTool() { toolManager.setCurrentTool(&eraserTool); }

void CanvasWidget::setActiveLayer(int index) {
  layerManager.setActiveLayer(index);
  update();
}

int CanvasWidget::getActiveLayerIndex() const {
  return static_cast<int>(layerManager.getActiveLayerIndex());
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
  int activeIndex = layerManager.getActiveLayerIndex();
  if (activeIndex == srcRow) {
    layerManager.setActiveLayer(destRow);
  } else {
    if (srcRow < activeIndex && destRow >= activeIndex) {
      layerManager.setActiveLayer(activeIndex - 1);
    } else if (srcRow > activeIndex && destRow <= activeIndex) {
      layerManager.setActiveLayer(activeIndex + 1);
    }
  }

  layerManager.reorderLayers(srcRow, destRow);
  emit layersChanged();
  update();
}
void CanvasWidget::toggleLayerVisibility(int index) {
  auto &layers = document.getLayers();

  if (index < 0 || index >= layers.size())
    return;

  layers[index].setVisible(!layers[index].isVisible());

  update();
}

void CanvasWidget::setBrushColor(int r, int g, int b) {
  Color color(r, g, b, 255);

  brushTool.setColor(color);
  fillTool.setColor(color);
}
void CanvasWidget::duplicateLayer(int index) {
  document.duplicateLayer(index);

  emit layersChanged();

  update();
}