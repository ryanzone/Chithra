#include "PanelDock.hpp"

#include <QApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QStyle>
#include <QWindow>

// ─────────────────────────────────────────────────────────────────────────────
// Shared style constants
// ─────────────────────────────────────────────────────────────────────────────
static constexpr int kTitleBarHeight = 26;

static const char *kTitleBarStyle = R"(
  PanelDockTitleBar {
    background: #2a2a2a;
    border-bottom: 1px solid #111111;
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
  QToolButton {
    background: transparent;
    border: none;
    color: #888888;
    border-radius: 2px;
  }
  QToolButton:hover {
    background: #3c3c3c;
    color: #cccccc;
  }
  QToolButton:pressed {
    background: #484848;
  }
)";

// ─────────────────────────────────────────────────────────────────────────────
// PanelDockTitleBar
// ─────────────────────────────────────────────────────────────────────────────
PanelDockTitleBar::PanelDockTitleBar(const QString &title, QWidget *parent)
    : QWidget(parent) {
  setFixedHeight(kTitleBarHeight);
  setStyleSheet(kTitleBarStyle);

  auto *layout = new QHBoxLayout(this);
  layout->setContentsMargins(8, 0, 4, 0);
  layout->setSpacing(2);

  m_title = new QLabel(title, this);
  m_title->setStyleSheet(kTitleLabelStyle);

  // Float/dock toggle button (⧉ when docked, ⊟ when floating)
  m_floatBtn = new QToolButton(this);
  m_floatBtn->setText("\u29C9"); // ⧉
  m_floatBtn->setFixedSize(18, 18);
  m_floatBtn->setToolTip("Float / Dock panel");
  m_floatBtn->setStyleSheet(kIconBtnStyle);
  connect(m_floatBtn, &QToolButton::clicked, this,
          &PanelDockTitleBar::floatToggleRequested);

  // Collapse arrow
  m_collapseBtn = new QToolButton(this);
  m_collapseBtn->setCheckable(true);
  m_collapseBtn->setChecked(true);
  m_collapseBtn->setArrowType(Qt::DownArrow);
  m_collapseBtn->setFixedSize(18, 18);
  m_collapseBtn->setToolTip("Collapse / Expand");
  m_collapseBtn->setStyleSheet(kIconBtnStyle);

  layout->addWidget(m_title);
  layout->addStretch();
  layout->addWidget(m_floatBtn);
  layout->addWidget(m_collapseBtn);
}

// Drag-to-move when the dock is floating ─────────────────────────────────────

void PanelDockTitleBar::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    // Only drag if our top-level is actually a floating dock
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
  // Let stylesheet handle the background; nothing extra needed.
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

  // Float/undock toggle via the title bar button
  connect(m_titleBar, &PanelDockTitleBar::floatToggleRequested, this,
          [this]() { setFloating(!isFloating()); });
  connect(this, &QDockWidget::topLevelChanged, this,
          &PanelDock::applyFloatingStyle);
}

void PanelDock::applyFloatingStyle(bool floating) {
  if (floating) {
    // Remove the OS window frame; our title bar is the chrome.
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint |
                   Qt::WindowStaysOnTopHint);

    // Thin border so the panel has a defined edge against the canvas
    setStyleSheet(R"(
      QDockWidget#PanelDock {
        border: 1px solid #111111;
      }
    )");

    // Re-show after flag change (required after setWindowFlags)
    show();
  } else {
    setWindowFlags(Qt::Widget);
    setStyleSheet("");
    show();
  }
}