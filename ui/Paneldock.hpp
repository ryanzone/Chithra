#pragma once

#include <QDockWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QToolButton>
#include <QWidget>

// ─────────────────────────────────────────────────────────────────────────────
// PanelDockTitleBar
// ─────────────────────────────────────────────────────────────────────────────
class PanelDockTitleBar : public QWidget {
  Q_OBJECT

public:
  explicit PanelDockTitleBar(const QString &title, QWidget *parent = nullptr);

  QToolButton *collapseButton() const { return m_collapseBtn; }

signals:
  void floatToggleRequested();
  void closeRequested();

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private:
  QLabel *m_title = nullptr;

  QToolButton *m_collapseBtn = nullptr;
  QToolButton *m_floatBtn = nullptr;

  bool m_dragging = false;
  QPoint m_dragStart;
};

// ─────────────────────────────────────────────────────────────────────────────
// PanelDock
// ─────────────────────────────────────────────────────────────────────────────
class PanelDock : public QDockWidget {
  Q_OBJECT

public:
  explicit PanelDock(const QString &title, QWidget *parent = nullptr);

  PanelDockTitleBar *panelTitleBar() const { return m_titleBar; }

private:
  void applyFloatingStyle(bool floating);

  PanelDockTitleBar *m_titleBar = nullptr;
};