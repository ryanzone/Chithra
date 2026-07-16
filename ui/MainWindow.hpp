#pragma once

#include <QMainWindow>
#include <QPushButton>

#include <QComboBox>
#include <QDropEvent>
#include <QLabel>
#include <QListWidget>
#include <QMouseEvent>
#include <QSlider>
#include <QToolButton>

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
  void updateStatusBar(int x, int y);
  void openFile();
  void saveFile();

private:
  void setupMenuBar();
  void setupToolBar();
  void setupToolOptionsBar();
  void setupLayersPanel();
  void setupStatusBar();
  void applyGlobalStyle();
  void setActiveTool(QToolButton *btn);

  CanvasWidget *canvasWidget = nullptr;
  LayerListWidget *layerList = nullptr;

  QPushButton *addLayerButton = nullptr;
  QLabel *statusCoords = nullptr;
  QLabel *statusTool = nullptr;
  QLabel *statusSize = nullptr;
  QWidget *colorSwatch = nullptr;

  QSlider *brushSizeSlider = nullptr;
  QLabel *brushSizeLabel = nullptr;
  QComboBox *shapeTypeCombo = nullptr;
  QToolButton *shapeFilledBtn = nullptr;

  QWidget *brushOptionsWidget = nullptr;
  QWidget *shapeOptionsWidget = nullptr;

  // Tool buttons for active state tracking
  QToolButton *activeTool = nullptr;
  QToolButton *brushBtn = nullptr;
  QToolButton *eraserBtn = nullptr;
  QToolButton *fillBtn = nullptr;
  QToolButton *selectBtn = nullptr;
  QToolButton *moveBtn = nullptr;
  QToolButton *shapeBtn = nullptr;
};
