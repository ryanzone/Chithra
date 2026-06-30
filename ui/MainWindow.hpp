#pragma once

#include <QMainWindow>
#include <QPushButton>

#include <QDropEvent>
#include <QListWidget>
#include <QMouseEvent>


class CanvasWidget;

class LayerListWidget : public QListWidget {
  Q_OBJECT

public:
  explicit LayerListWidget(QWidget *parent = nullptr);

signals:
  void layersReordered(int srcRow, int destRow);

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void dropEvent(QDropEvent *event) override;
};

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

private slots:
  void refreshLayerList();
  void onLayerSelected(int row);

  void deleteLayer();
  void toggleLayerVisibility();
  void renameLayer();

private:
  CanvasWidget *canvasWidget;
  LayerListWidget *layerList;

  QPushButton *addLayerButton;
};