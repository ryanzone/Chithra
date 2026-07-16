#include "PanelDock.hpp"

#include <QApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QStyle>
#include <QStyleOption>
#include <QWindow>

// ─────────────────────────────────────────────────────────────────────────────
// Shared style constants
// ─────────────────────────────────────────────────────────────────────────────
static constexpr int kTitleBarHeight = 26;

static const char *kTitleBarStyle = R"(
  PanelDockTitleBar {
    background: #000326;
    border-bottom: 1px solid #000326;
  }
)";

static const char *kTitleLabelStyle = R"(
  QLabel {
    color: #c8c8c8;
    font-size: 11px;
    font-weight: 600;
    background: transparent;
  }
)";

static const char *kIconBtnStyle = R"(
QToolButton{
    background:transparent;
    border:none;
    border-radius:4px;
    padding:2px;
}

QToolButton:hover{
    background: rgba(255, 255, 255, 0.1); 
}

QToolButton:pressed{
    background: rgba(255, 255, 255, 0.2);
}
)";

static QIcon getTintedIcon(const QString &path, const QColor &color) {
  QIcon icon(path);
  QPixmap pixmap = icon.pixmap(QSize(16, 16));

  QPainter painter(&pixmap);
  painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
  painter.fillRect(pixmap.rect(), color);
  painter.end();

  return QIcon(pixmap);
}

// ─────────────────────────────────────────────────────────────────────────────
// PanelDockTitleBar
// ─────────────────────────────────────────────────────────────────────────────
PanelDockTitleBar::PanelDockTitleBar(const QString &title, QWidget *parent)
    : QWidget(parent) {
  setFixedHeight(kTitleBarHeight);
  setStyleSheet(kTitleBarStyle);

  auto *layout = new QHBoxLayout(this);
  layout->setContentsMargins(8, 0, 4, 0);
  layout->setSpacing(4);

  m_title = new QLabel(title, this);
  m_title->setStyleSheet(kTitleLabelStyle);

  // Float button
  m_floatBtn = new QToolButton(this);
  m_floatBtn->setIcon(getTintedIcon(":/icons/icons/dock.svg", Qt::white));
  m_floatBtn->setIconSize(QSize(16, 16));
  m_floatBtn->setFixedSize(22, 22);
  m_floatBtn->setToolTip("Dock Panel");
  m_floatBtn->setStyleSheet(kIconBtnStyle);

  connect(m_floatBtn, &QToolButton::clicked, this,
          &PanelDockTitleBar::floatToggleRequested);

  // Collapse button
  m_collapseBtn = new QToolButton(this);
  m_collapseBtn->setCheckable(true);
  m_collapseBtn->setChecked(true);
  m_collapseBtn->setIcon(
      getTintedIcon(":/icons/icons/chevron-left.svg", Qt::white));
  m_collapseBtn->setIconSize(QSize(16, 16));
  m_collapseBtn->setFixedSize(22, 22);
  m_collapseBtn->setToolTip("Collapse / Expand");
  m_collapseBtn->setStyleSheet(kIconBtnStyle);

  connect(m_collapseBtn, &QToolButton::toggled, this, [this](bool expanded) {
    m_collapseBtn->setIcon(
        getTintedIcon(expanded ? ":/icons/icons/chevron-left.svg"
                               : ":/icons/icons/chevron-right.svg",
                      Qt::white));
  });

  layout->addWidget(m_title);
  layout->addStretch();
  layout->addWidget(m_floatBtn);
  layout->addWidget(m_collapseBtn);
}

// Drag-to-move when the dock is floating ─────────────────────────────────────
void PanelDockTitleBar::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    QWidget *top = window();
    if (top && top->isWindow()) {
      m_dragging = true;
      m_dragStart =
          event->globalPosition().toPoint() - top->frameGeometry().topLeft();
    }
  }
  QWidget::mousePressEvent(event);
}

void PanelDockTitleBar::mouseMoveEvent(QMouseEvent *event) {
  if (m_dragging) {
    QWidget *top = window();
    if (top && top->isWindow()) {
      top->move(event->globalPosition().toPoint() - m_dragStart);
    }
  }
  QWidget::mouseMoveEvent(event);
}

void PanelDockTitleBar::mouseReleaseEvent(QMouseEvent *event) {
  m_dragging = false;
  QWidget::mouseReleaseEvent(event);
}

void PanelDockTitleBar::mouseDoubleClickEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton)
    emit floatToggleRequested();
  QWidget::mouseDoubleClickEvent(event);
}

void PanelDockTitleBar::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// ─────────────────────────────────────────────────────────────────────────────
// PanelDock
// ─────────────────────────────────────────────────────────────────────────────
PanelDock::PanelDock(const QString &title, QWidget *parent)
    : QDockWidget(parent) {
  setObjectName("PanelDock");
  setFeatures(QDockWidget::DockWidgetMovable |
              QDockWidget::DockWidgetFloatable);

  m_titleBar = new PanelDockTitleBar(title, this);
  setTitleBarWidget(m_titleBar);

  connect(m_titleBar, &PanelDockTitleBar::floatToggleRequested, this,
          [this]() { setFloating(!isFloating()); });
  connect(this, &QDockWidget::topLevelChanged, this,
          &PanelDock::applyFloatingStyle);
}

void PanelDock::applyFloatingStyle(bool floating) {
  if (floating) {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint |
                   Qt::WindowStaysOnTopHint);

    setStyleSheet(R"(
      QDockWidget#PanelDock {
        border: 1px solid #000326;
      }
    )");

    show();
  } else {
    setWindowFlags(Qt::Widget);
    setStyleSheet("");
    show();
  }
}