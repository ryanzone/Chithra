#pragma once

#include <QWidget>

class QLabel;
class QToolButton;
class QMouseEvent;
class QContextMenuEvent;

class LayerItemWidget : public QWidget {
  Q_OBJECT

public:
  explicit LayerItemWidget(const QString &name, bool visible,
                           QWidget *parent = nullptr);

  QToolButton *visibilityButton;
  QLabel *nameLabel;

signals:
  void clicked();
  void visibilityToggled();
  void renameRequested();
  void deleteRequested();

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
};