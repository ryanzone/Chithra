#include "MainWindow.hpp"
#include "CanvasWidget.hpp"

#include <QDockWidget>
#include <QLabel>
#include <QListWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Chithra");
  resize(1400, 900);

  //
  // Menubar
  //
  menuBar()->addMenu("File");
  menuBar()->addMenu("Edit");
  menuBar()->addMenu("Image");
  menuBar()->addMenu("Layer");
  menuBar()->addMenu("Select");
  menuBar()->addMenu("View");
  menuBar()->addMenu("Help");

  //
  // Toolbar
  //
  QToolBar *tools = addToolBar("Tools");

  tools->addAction("Move");
  tools->addAction("Brush");
  tools->addAction("Eraser");
  tools->addAction("Fill");
  tools->addAction("Select");

  //
  // Canvas
  //
  canvasWidget = new CanvasWidget(this);
  setCentralWidget(canvasWidget);

  //
  // Layers panel
  //
  auto *layersDock = new QDockWidget("Layers", this);

  auto *layerList = new QListWidget();
  layerList->addItem("Background");

  layersDock->setWidget(layerList);

  addDockWidget(Qt::RightDockWidgetArea, layersDock);

  //
  // Status bar
  //
  statusBar()->showMessage("Ready");
}