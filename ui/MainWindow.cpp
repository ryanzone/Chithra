#include "MainWindow.hpp"
#include "CanvasWidget.hpp"
#include "LayerItemWidget.hpp"
#include "PanelDock.hpp"
#include <QAction>
#include <QColorDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

//
// LayerListWidget Implementation
//
LayerListWidget::LayerListWidget(QWidget *parent) : QListWidget(parent) {
  setDragEnabled(true);
  setAcceptDrops(true);
  setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::DragDrop);
}

void LayerListWidget::mousePressEvent(QMouseEvent *event) {
  QModelIndex index = indexAt(event->position().toPoint());
  if (!index.isValid()) {
    clearSelection();
    setCurrentRow(-1);
  }
  QListWidget::mousePressEvent(event);
}

void LayerListWidget::dropEvent(QDropEvent *event) {
  int srcRow = currentRow();

  if (srcRow < 0) {
    event->ignore();
    return;
  }

  QModelIndex index = indexAt(event->position().toPoint());

  int destRow;

  if (index.isValid()) {
    destRow = index.row();

    if (dropIndicatorPosition() == QAbstractItemView::BelowItem)
      destRow++;

    if (srcRow < destRow)
      destRow--;
  } else {
    destRow = count() - 1;
  }

  emit layersReordered(srcRow, destRow);

  event->accept();

  return; // IMPORTANT
}

//
// MainWindow Implementation
//
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

  QAction *moveAction = tools->addAction("Move");
  QAction *brushAction = tools->addAction("Brush");
  QAction *eraserAction = tools->addAction("Eraser");
  QAction *fillAction = tools->addAction("Fill");
  QAction *selectAction = tools->addAction("Select");
  QAction *colorAction = tools->addAction("Color");

  //
  // Canvas
  //
  canvasWidget = new CanvasWidget(this);
  setCentralWidget(canvasWidget);

  connect(brushAction, &QAction::triggered, canvasWidget,
          &CanvasWidget::setBrushTool);

  connect(eraserAction, &QAction::triggered, canvasWidget,
          &CanvasWidget::setEraserTool);
  connect(fillAction, &QAction::triggered, canvasWidget,
          &CanvasWidget::setFillTool);

  connect(moveAction, &QAction::triggered, canvasWidget,
          &CanvasWidget::setMoveTool);

  connect(selectAction, &QAction::triggered, canvasWidget,
          &CanvasWidget::setSelectionTool);
  connect(colorAction, &QAction::triggered, this, [this]() {
    QColor color = QColorDialog::getColor(Qt::black, this);
    if (!color.isValid())
      return;
    canvasWidget->setBrushColor(color.red(), color.green(), color.blue());
  });

  //
  // Layers Panel
  //
  // ── Palette ────────────────────────────────────────────────────────────────
  // bg0  #1e1e1e   dock / outermost shell
  // bg1  #252526   panel body
  // bg2  #2a2a2a   header
  // bg3  #3c3c3c   hover / separator
  // acc  #4a90d9   selection highlight (muted blue, Krita-like)
  // txt0 #c8c8c8   primary text
  // txt1 #888888   secondary / icon tint
  // ──────────────────────────────────────────────────────────────────────────

  auto *layersDock = new PanelDock("Layers", this);

  // ── Root panel widget ──────────────────────────────────────────────────────
  QWidget *panelRoot = new QWidget();
  panelRoot->setObjectName("LayerPanelRoot");
  panelRoot->setStyleSheet(R"(
    QWidget#LayerPanelRoot {
      background: #1e1e1e;
    }
  )");

  QVBoxLayout *rootLayout = new QVBoxLayout(panelRoot);
  rootLayout->setContentsMargins(0, 0, 0, 0);
  rootLayout->setSpacing(0);

  // ── Collapsible content area ───────────────────────────────────────────────
  QWidget *contentArea = new QWidget();
  contentArea->setObjectName("LayerContentArea");
  contentArea->setStyleSheet(R"(
    QWidget#LayerContentArea {
      background: #252526;
    }
  )");

  QVBoxLayout *contentLayout = new QVBoxLayout(contentArea);
  contentLayout->setContentsMargins(0, 0, 0, 0);
  contentLayout->setSpacing(0);

  // ── Layer list ─────────────────────────────────────────────────────────────
  layerList = new LayerListWidget(this);
  layerList->setObjectName("LayerList");
  layerList->setStyleSheet(R"(
    QListWidget#LayerList {
      background: #1e1e1e;
      border: none;
      border-top: 1px solid #111111;
      outline: 0;
    }
    QListWidget#LayerList::item {
      background: #252526;
      border-bottom: 1px solid #1a1a1a;
      padding: 0px;
    }
    QListWidget#LayerList::item:hover {
      background: #2e2e2e;
    }
    QListWidget#LayerList::item:selected {
      background: #1f3a5c;
      border-left: 2px solid #4a90d9;
    }
    QListWidget#LayerList::item:selected:hover {
      background: #24456e;
    }
    QScrollBar:vertical {
      background: #1e1e1e;
      width: 8px;
      margin: 0;
    }
    QScrollBar::handle:vertical {
      background: #3c3c3c;
      border-radius: 4px;
      min-height: 24px;
    }
    QScrollBar::handle:vertical:hover {
      background: #505050;
    }
    QScrollBar::add-line:vertical,
    QScrollBar::sub-line:vertical {
      height: 0;
    }
    QScrollBar::add-page:vertical,
    QScrollBar::sub-page:vertical {
      background: none;
    }
  )");
  layerList->setMinimumHeight(80);

  // ── Separator ──────────────────────────────────────────────────────────────
  QFrame *separator = new QFrame();
  separator->setFrameShape(QFrame::HLine);
  separator->setFixedHeight(1);
  separator->setStyleSheet("background: #111111; border: none;");

  // ── Bottom action bar ──────────────────────────────────────────────────────
  QWidget *actionBar = new QWidget();
  actionBar->setObjectName("LayerActionBar");
  actionBar->setFixedHeight(28);
  actionBar->setStyleSheet(R"(
    QWidget#LayerActionBar {
      background: #252526;
      border-top: 1px solid #111111;
    }
  )");

  QHBoxLayout *actionLayout = new QHBoxLayout(actionBar);
  actionLayout->setContentsMargins(4, 3, 4, 3);
  actionLayout->setSpacing(2);

  const QString iconBtnStyle = R"(
    QToolButton {
      background: transparent;
      border: none;
      color: #aaaaaa;
      font-size: 14px;
      border-radius: 3px;
      padding: 0px 4px;
      min-width: 22px;
      min-height: 22px;
    }
    QToolButton:hover {
      background: #3a3a3a;
      color: #dddddd;
    }
    QToolButton:pressed {
      background: #444444;
    }
    QToolButton:disabled {
      color: #555555;
    }
  )";

  addLayerButton = new QPushButton("+");
  addLayerButton->setObjectName("AddLayerBtn");
  addLayerButton->setFixedSize(22, 22);
  addLayerButton->setToolTip("New Layer");
  addLayerButton->setStyleSheet(R"(
    QPushButton#AddLayerBtn {
      background: transparent;
      border: none;
      color: #aaaaaa;
      font-size: 16px;
      border-radius: 3px;
      padding-bottom: 1px;
    }
    QPushButton#AddLayerBtn:hover {
      background: #3a3a3a;
      color: #dddddd;
    }
    QPushButton#AddLayerBtn:pressed {
      background: #444444;
    }
  )");

  QToolButton *deleteLayerBtn = new QToolButton();
  deleteLayerBtn->setObjectName("DeleteLayerBtn");
  deleteLayerBtn->setText("\u232B");
  deleteLayerBtn->setFixedSize(22, 22);
  deleteLayerBtn->setToolTip("Delete Layer");
  deleteLayerBtn->setStyleSheet(iconBtnStyle);

  QToolButton *duplicateLayerBtn = new QToolButton();
  duplicateLayerBtn->setObjectName("DuplicateLayerBtn");
  duplicateLayerBtn->setText("\u29C9");
  duplicateLayerBtn->setFixedSize(22, 22);
  duplicateLayerBtn->setToolTip("Duplicate Layer");
  duplicateLayerBtn->setStyleSheet(iconBtnStyle);

  actionLayout->addWidget(addLayerButton);
  actionLayout->addWidget(duplicateLayerBtn);
  actionLayout->addStretch();
  actionLayout->addWidget(deleteLayerBtn);

  contentLayout->addWidget(layerList);
  contentLayout->addWidget(separator);
  contentLayout->addWidget(actionBar);

  rootLayout->addWidget(contentArea);

  layersDock->setWidget(panelRoot);
  addDockWidget(Qt::RightDockWidgetArea, layersDock);

  // ── Collapse / expand ──────────────────────────────────────────────────────
  QToolButton *collapseBtn = layersDock->panelTitleBar()->collapseButton();

  connect(collapseBtn, &QToolButton::toggled, this,
          [collapseBtn, contentArea](bool expanded) {
            contentArea->setVisible(expanded);
            collapseBtn->setArrowType(expanded ? Qt::DownArrow
                                               : Qt::RightArrow);
          });

  // ── Action bar connections ─────────────────────────────────────────────────
  connect(deleteLayerBtn, &QToolButton::clicked, this,
          &MainWindow::deleteLayer);
  connect(duplicateLayerBtn, &QToolButton::clicked, this, [this]() {
    int row = layerList->currentRow();

    if (row < 0)
      return;

    canvasWidget->duplicateLayer(row);
  });
  //
  // Connections (unchanged from original)
  //
  connect(addLayerButton, &QPushButton::clicked, canvasWidget,
          &CanvasWidget::addLayer);

  connect(canvasWidget, &CanvasWidget::layersChanged, this,
          &MainWindow::refreshLayerList);

  connect(layerList, &QListWidget::currentRowChanged, this,
          &MainWindow::onLayerSelected);

  connect(layerList, &QListWidget::itemDoubleClicked, this,
          [this]() { renameLayer(); });

  connect(layerList, &LayerListWidget::layersReordered, this,
          [this](int srcRow, int destRow) {
            canvasWidget->reorderLayers(srcRow, destRow);
          });

  // Initial population
  refreshLayerList();

  //
  // Status bar
  //
  statusBar()->showMessage("Ready");
}

void MainWindow::refreshLayerList() {
  layerList->blockSignals(true);
  layerList->clear();

  const auto &layers = canvasWidget->getDocument().getLayers();

  for (int i = 0; i < layers.size(); i++) {
    QListWidgetItem *item = new QListWidgetItem(layerList);
    LayerItemWidget *widget = new LayerItemWidget(
        QString::fromStdString(layers[i].getName()), layers[i].isVisible());

    item->setSizeHint(QSize(0, 32));
    layerList->addItem(item);
    layerList->setItemWidget(item, widget);

    connect(widget, &LayerItemWidget::renameRequested, this, [this, item]() {
      int index = layerList->row(item);

      if (index < 0)
        return;

      bool ok;

      QString text = QInputDialog::getText(
          this, "Rename Layer", "Layer Name:", QLineEdit::Normal, "", &ok);

      if (ok && !text.isEmpty())
        canvasWidget->renameLayer(index, text.toStdString());
    });

    connect(widget, &LayerItemWidget::deleteRequested, this, [this, item]() {
      int index = layerList->row(item);

      qDebug() << "Delete requested:" << index;

      if (index >= 0)
        canvasWidget->removeLayer(index);
    });

    connect(widget, &LayerItemWidget::clicked, this, [this, item]() {
      int index = layerList->row(item);
      if (index >= 0) {
        layerList->setCurrentRow(index);
      }
    });

    connect(widget, &LayerItemWidget::visibilityToggled, this, [this, item]() {
      int index = layerList->row(item);

      if (index >= 0) {
        canvasWidget->toggleLayerVisibility(index);
        refreshLayerList();
      }
    });
  }

  int activeIndex = canvasWidget->getActiveLayerIndex();
  layerList->setCurrentRow(activeIndex);
  layerList->blockSignals(false);
}

void MainWindow::onLayerSelected(int row) {
  if (row < 0) {
    canvasWidget->selectCanvas();
  } else {
    canvasWidget->setActiveLayer(row);
  }
}

void MainWindow::deleteLayer() {
  int row = layerList->currentRow();
  if (row < 0)
    return;
  canvasWidget->removeLayer(row);
}

void MainWindow::toggleLayerVisibility() {
  int row = layerList->currentRow();
  if (row < 0)
    return;
  canvasWidget->toggleLayerVisibility(row);
}

void MainWindow::renameLayer() {
  int row = layerList->currentRow();
  if (row < 0)
    return;

  bool ok;
  QString text = QInputDialog::getText(
      this, "Rename Layer", "Layer Name:", QLineEdit::Normal, "", &ok);

  if (!ok || text.isEmpty())
    return;

  canvasWidget->renameLayer(row, text.toStdString());
}