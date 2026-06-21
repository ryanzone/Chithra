#include "CanvasWidget.hpp"

#include "../core/image/Image.hpp"
#include "../core/tools/ToolContext.hpp"

#include <QImage>
#include <QPainter>

CanvasWidget::CanvasWidget(QWidget *parent)
    : QWidget(parent), document(800, 600), layerManager(document),
      brushTool(10) {

  document.addLayer("Background");

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
}

void CanvasWidget::applyBrush(int x, int y) {
  ToolContext context{layerManager};

  brushTool.apply(context, x, y);
}

void CanvasWidget::mousePressEvent(QMouseEvent *event) {
  painting = true;

  int x = static_cast<int>(event->position().x()) - (width() - 800) / 2;

  int y = static_cast<int>(event->position().y()) - (height() - 600) / 2;

  applyBrush(x, y);

  lastPoint = QPoint(x, y);

  update();
}

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

    applyBrush(px, py);
  }

  lastPoint = QPoint(x, y);

  update();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *) { painting = false; }