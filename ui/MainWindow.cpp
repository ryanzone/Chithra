#include "MainWindow.hpp"
#include "CanvasWidget.hpp"
#include "LayerItemWidget.hpp"
#include "PanelDock.hpp"
#include <QAction>
#include <QColorDialog>
#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QSlider>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

// ═══════════════════════════════════════════════════════════════════════════════
// LayerListWidget
// ═══════════════════════════════════════════════════════════════════════════════
LayerListWidget::LayerListWidget(QWidget *parent) : QListWidget(parent) {
  setDragEnabled(true);
  setAcceptDrops(true);
  setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::InternalMove);
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
}

// ═══════════════════════════════════════════════════════════════════════════════
// Global Stylesheet
// ═══════════════════════════════════════════════════════════════════════════════
void MainWindow::applyGlobalStyle() {
  setStyleSheet(R"(
    * {
      font-family: "Segoe UI", "SF Pro Text", "Helvetica Neue", sans-serif;
    }

    QMainWindow {
      background: #1b1b1b;
    }

    QMenuBar {
      background: #252526;
      color: #cccccc;
      border-bottom: 1px solid #1a1a1a;
      padding: 2px 0;
      font-size: 12px;
    }
    QMenuBar::item {
      padding: 4px 10px;
      border-radius: 3px;
    }
    QMenuBar::item:selected {
      background: #3a3a3a;
    }
    QMenuBar::item:pressed {
      background: #094771;
    }

    QMenu {
      background: #252526;
      color: #cccccc;
      border: 1px solid #333333;
      padding: 4px 0;
      font-size: 12px;
    }
    QMenu::item {
      padding: 5px 28px 5px 20px;
    }
    QMenu::item:selected {
      background: #094771;
    }
    QMenu::separator {
      height: 1px;
      background: #333333;
      margin: 4px 8px;
    }
    QMenu::item:disabled {
      color: #555555;
    }

    QToolBar {
      background: #252526;
      border: none;
      border-bottom: 1px solid #1a1a1a;
      spacing: 2px;
      padding: 3px 6px;
    }

    QStatusBar {
      background: #007acc;
      color: #ffffff;
      font-size: 11px;
      border: none;
      min-height: 22px;
    }
    QStatusBar::item {
      border: none;
    }
    QStatusBar QLabel {
      color: #ffffff;
      padding: 0 8px;
      font-size: 11px;
    }

    QToolTip {
      background: #333333;
      color: #cccccc;
      border: 1px solid #555555;
      padding: 4px 8px;
      font-size: 11px;
    }

    QSlider::groove:horizontal {
      height: 4px;
      background: #3a3a3a;
      border-radius: 2px;
    }
    QSlider::handle:horizontal {
      background: #0078d4;
      width: 14px;
      height: 14px;
      margin: -5px 0;
      border-radius: 7px;
    }
    QSlider::handle:horizontal:hover {
      background: #1a8fff;
    }
    QSlider::sub-page:horizontal {
      background: #0078d4;
      border-radius: 2px;
    }

    QComboBox {
      background: #333333;
      color: #cccccc;
      border: 1px solid #444444;
      border-radius: 3px;
      padding: 3px 8px;
      min-height: 20px;
      font-size: 11px;
    }
    QComboBox:hover {
      border-color: #0078d4;
    }
    QComboBox::drop-down {
      border: none;
      width: 18px;
    }
    QComboBox::down-arrow {
      image: none;
      border-left: 4px solid transparent;
      border-right: 4px solid transparent;
      border-top: 5px solid #888888;
      margin-right: 6px;
    }
    QComboBox QAbstractItemView {
      background: #252526;
      color: #cccccc;
      border: 1px solid #444444;
      selection-background-color: #094771;
    }
  )");
}

// ═══════════════════════════════════════════════════════════════════════════════
// MainWindow Constructor
// ═══════════════════════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Chithra");
  setWindowIcon(QIcon(":/icons/icons/chitra.svg"));
  resize(1400, 900);

  applyGlobalStyle();

  canvasWidget = new CanvasWidget(this);
  setCentralWidget(canvasWidget);

  setupMenuBar();
  setupToolBar();
  setupToolOptionsBar();
  setupLayersPanel();
  setupStatusBar();

  // Connections
  connect(canvasWidget, &CanvasWidget::layersChanged, this,
          &MainWindow::refreshLayerList);
  connect(layerList, &QListWidget::currentRowChanged, this,
          &MainWindow::onLayerSelected);
  connect(layerList, &QListWidget::itemDoubleClicked, this,
          [this]() { renameLayer(); });
  connect(layerList, &LayerListWidget::layersReordered, this,
          [this](int src, int dest) { canvasWidget->reorderLayers(src, dest); });
  connect(canvasWidget, &CanvasWidget::cursorMoved, this,
          &MainWindow::updateStatusBar);

  refreshLayerList();
}

// ═══════════════════════════════════════════════════════════════════════════════
// Menu Bar
// ═══════════════════════════════════════════════════════════════════════════════
void MainWindow::setupMenuBar() {
  // ── File ──
  QMenu *fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction("New", this, []() {}, QKeySequence::New);
  fileMenu->addSeparator();
  fileMenu->addAction("Exit", this, &QMainWindow::close, QKeySequence::Quit);

  // ── Edit ──
  QMenu *editMenu = menuBar()->addMenu("&Edit");
  editMenu->addAction("Clear Layer", this,
                       [this]() { canvasWidget->clearActiveLayer(); });
  editMenu->addAction("Clear All", this,
                       [this]() { canvasWidget->clearAll(); });

  // ── Image ──
  menuBar()->addMenu("&Image");

  // ── Layer ──
  QMenu *layerMenu = menuBar()->addMenu("&Layer");
  layerMenu->addAction("Add Layer", this, [this]() { canvasWidget->addLayer(); },
                        QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_N));
  layerMenu->addAction("Delete Layer", this, &MainWindow::deleteLayer);
  layerMenu->addAction("Rename Layer", this, &MainWindow::renameLayer);

  // ── View ──
  menuBar()->addMenu("&View");

  // ── Help ──
  menuBar()->addMenu("&Help");
}

// ═══════════════════════════════════════════════════════════════════════════════
// Tool Button Helper
// ═══════════════════════════════════════════════════════════════════════════════
static const QString kToolBtnStyle = R"(
  QToolButton {
    background: transparent;
    border: 1px solid transparent;
    border-radius: 4px;
    color: #aaaaaa;
    font-size: 15px;
    min-width: 32px;
    min-height: 32px;
    padding: 2px;
  }
  QToolButton:hover {
    background: #3a3a3a;
    border-color: #4a4a4a;
    color: #dddddd;
  }
  QToolButton:pressed {
    background: #444444;
  }
  QToolButton[active="true"] {
    background: #094771;
    border-color: #0078d4;
    color: #ffffff;
  }
)";

void MainWindow::setActiveTool(QToolButton *btn) {
  if (activeTool) {
    activeTool->setProperty("active", false);
    activeTool->style()->unpolish(activeTool);
    activeTool->style()->polish(activeTool);
  }
  activeTool = btn;
  if (activeTool) {
    activeTool->setProperty("active", true);
    activeTool->style()->unpolish(activeTool);
    activeTool->style()->polish(activeTool);
  }

  // Show/hide tool options
  bool isBrushLike =
      (btn == brushBtn || btn == eraserBtn || btn == fillBtn);
  bool isShape = (btn == shapeBtn);
  
  if (brushOptionsWidget)
    brushOptionsWidget->setVisible(isBrushLike);
  if (shapeOptionsWidget)
    shapeOptionsWidget->setVisible(isShape);

  // Update status bar tool name
  if (btn && statusTool)
    statusTool->setText(btn->toolTip());
}

// ═══════════════════════════════════════════════════════════════════════════════
// Tool Bar
// ═══════════════════════════════════════════════════════════════════════════════
void MainWindow::setupToolBar() {
  QToolBar *toolbar = addToolBar("Tools");
  toolbar->setMovable(false);
  toolbar->setIconSize(QSize(20, 20));

  auto makeTool = [&](const QString &icon, const QString &tip) {
    auto *btn = new QToolButton();
    btn->setText(icon);
    btn->setToolTip(tip);
    btn->setStyleSheet(kToolBtnStyle);
    btn->setFixedSize(34, 34);
    toolbar->addWidget(btn);
    return btn;
  };

  brushBtn = makeTool("\U0001F58C", "Brush");
  eraserBtn = makeTool("\U0001F9F9", "Eraser");
  fillBtn = makeTool("\U0001FAA3", "Fill");
  selectBtn = makeTool("\u2B1C", "Select");
  moveBtn = makeTool("\u2725", "Move");
  shapeBtn = makeTool("\u25AD", "Shape");

  // Separator
  toolbar->addSeparator();

  // Color swatch
  colorSwatch = new QWidget();
  colorSwatch->setFixedSize(28, 28);
  colorSwatch->setStyleSheet(
      "background: #ff0000; border: 2px solid #555555; border-radius: 4px;");
  colorSwatch->setToolTip("Current Color");
  colorSwatch->setCursor(Qt::PointingHandCursor);
  toolbar->addWidget(colorSwatch);

  auto *colorBtn = new QToolButton();
  colorBtn->setText("\U0001F3A8");
  colorBtn->setToolTip("Pick Color");
  colorBtn->setStyleSheet(kToolBtnStyle);
  colorBtn->setFixedSize(34, 34);
  toolbar->addWidget(colorBtn);

  toolbar->addSeparator();

  // Clear buttons
  auto *clearLayerBtn = makeTool("\u2395", "Clear Layer");
  auto *clearAllBtn = makeTool("\u239A", "Clear All");

  // ── Connections ──
  connect(brushBtn, &QToolButton::clicked, this, [this]() {
    canvasWidget->setBrushTool();
    setActiveTool(brushBtn);
  });
  connect(eraserBtn, &QToolButton::clicked, this, [this]() {
    canvasWidget->setEraserTool();
    setActiveTool(eraserBtn);
  });
  connect(fillBtn, &QToolButton::clicked, this, [this]() {
    canvasWidget->setFillTool();
    setActiveTool(fillBtn);
  });
  connect(selectBtn, &QToolButton::clicked, this, [this]() {
    canvasWidget->setSelectionTool();
    setActiveTool(selectBtn);
  });
  connect(moveBtn, &QToolButton::clicked, this, [this]() {
    canvasWidget->setMoveTool();
    setActiveTool(moveBtn);
  });
  connect(shapeBtn, &QToolButton::clicked, this, [this]() {
    canvasWidget->setShapeTool();
    setActiveTool(shapeBtn);
  });

  connect(colorBtn, &QToolButton::clicked, this, [this]() {
    QColor color = QColorDialog::getColor(Qt::black, this, "Select Color");
    if (!color.isValid())
      return;
    canvasWidget->setBrushColor(color.red(), color.green(), color.blue());
    colorSwatch->setStyleSheet(
        QString("background: %1; border: 2px solid #555555; border-radius: 4px;")
            .arg(color.name()));
  });

  connect(clearLayerBtn, &QToolButton::clicked, this,
          [this]() { canvasWidget->clearActiveLayer(); });
  connect(clearAllBtn, &QToolButton::clicked, this,
          [this]() { canvasWidget->clearAll(); });

  // Set default active
  setActiveTool(brushBtn);
}

// ═══════════════════════════════════════════════════════════════════════════════
// Tool Options Bar
// ═══════════════════════════════════════════════════════════════════════════════
void MainWindow::setupToolOptionsBar() {
  QToolBar *optionsBar = addToolBar("Options");
  optionsBar->setMovable(false);
  optionsBar->setFixedHeight(32);
  optionsBar->setStyleSheet(R"(
    QToolBar {
      background: #2d2d2d;
      border-bottom: 1px solid #1a1a1a;
      spacing: 6px;
      padding: 2px 8px;
    }
    QLabel {
      color: #aaaaaa;
      font-size: 11px;
    }
  )");

  // ── Brush/Eraser options ──
  brushOptionsWidget = new QWidget();
  auto *brushLayout = new QHBoxLayout(brushOptionsWidget);
  brushLayout->setContentsMargins(0, 0, 0, 0);
  brushLayout->setSpacing(6);

  auto *sizeLabel = new QLabel("Size:");
  brushSizeSlider = new QSlider(Qt::Horizontal);
  brushSizeSlider->setRange(1, 100);
  brushSizeSlider->setValue(10);
  brushSizeSlider->setFixedWidth(120);

  brushSizeLabel = new QLabel("10");
  brushSizeLabel->setFixedWidth(24);
  brushSizeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

  brushLayout->addWidget(sizeLabel);
  brushLayout->addWidget(brushSizeSlider);
  brushLayout->addWidget(brushSizeLabel);

  optionsBar->addWidget(brushOptionsWidget);

  connect(brushSizeSlider, &QSlider::valueChanged, this, [this](int val) {
    brushSizeLabel->setText(QString::number(val));
    canvasWidget->setBrushSize(val);
    canvasWidget->setEraserSize(val);
  });

  // ── Shape options ──
  shapeOptionsWidget = new QWidget();
  auto *shapeLayout = new QHBoxLayout(shapeOptionsWidget);
  shapeLayout->setContentsMargins(0, 0, 0, 0);
  shapeLayout->setSpacing(6);

  auto *shapeLabel = new QLabel("Shape:");
  shapeTypeCombo = new QComboBox();
  shapeTypeCombo->addItem("Rectangle");
  shapeTypeCombo->addItem("Ellipse");
  shapeTypeCombo->addItem("Line");
  shapeTypeCombo->setFixedWidth(100);

  shapeFilledBtn = new QToolButton();
  shapeFilledBtn->setText("Filled");
  shapeFilledBtn->setCheckable(true);
  shapeFilledBtn->setStyleSheet(R"(
    QToolButton {
      background: #333333;
      color: #aaaaaa;
      border: 1px solid #444444;
      border-radius: 3px;
      padding: 2px 8px;
      font-size: 11px;
    }
    QToolButton:hover { background: #3a3a3a; }
    QToolButton:checked {
      background: #094771;
      border-color: #0078d4;
      color: #ffffff;
    }
  )");

  shapeLayout->addWidget(shapeLabel);
  shapeLayout->addWidget(shapeTypeCombo);
  shapeLayout->addWidget(shapeFilledBtn);

  optionsBar->addWidget(shapeOptionsWidget);
  shapeOptionsWidget->setVisible(false);

  connect(shapeTypeCombo, &QComboBox::currentIndexChanged, this, [this](int idx) {
    canvasWidget->getShapeTool().setShapeType(static_cast<ShapeType>(idx));
  });
  connect(shapeFilledBtn, &QToolButton::toggled, this, [this](bool checked) {
    canvasWidget->getShapeTool().setFilled(checked);
  });
}

// ═══════════════════════════════════════════════════════════════════════════════
// Layers Panel
// ═══════════════════════════════════════════════════════════════════════════════
void MainWindow::setupLayersPanel() {
  auto *layersDock = new PanelDock("Layers", this);

  QWidget *panelRoot = new QWidget();
  panelRoot->setObjectName("LayerPanelRoot");
  panelRoot->setStyleSheet(R"(
    QWidget#LayerPanelRoot { background: #1e1e1e; }
  )");

  QVBoxLayout *rootLayout = new QVBoxLayout(panelRoot);
  rootLayout->setContentsMargins(0, 0, 0, 0);
  rootLayout->setSpacing(0);

  QWidget *contentArea = new QWidget();
  contentArea->setObjectName("LayerContentArea");
  contentArea->setStyleSheet(R"(
    QWidget#LayerContentArea { background: #252526; }
  )");

  QVBoxLayout *contentLayout = new QVBoxLayout(contentArea);
  contentLayout->setContentsMargins(0, 0, 0, 0);
  contentLayout->setSpacing(0);

  // ── Layer list ──
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
      border-left: 2px solid #0078d4;
    }
    QListWidget#LayerList::item:selected:hover {
      background: #24456e;
    }
    QScrollBar:vertical {
      background: #1e1e1e; width: 8px; margin: 0;
    }
    QScrollBar::handle:vertical {
      background: #3c3c3c; border-radius: 4px; min-height: 24px;
    }
    QScrollBar::handle:vertical:hover { background: #505050; }
    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
    QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }
  )");
  layerList->setMinimumHeight(80);

  // ── Separator ──
  QFrame *separator = new QFrame();
  separator->setFrameShape(QFrame::HLine);
  separator->setFixedHeight(1);
  separator->setStyleSheet("background: #111111; border: none;");

  // ── Action bar ──
  QWidget *actionBar = new QWidget();
  actionBar->setObjectName("LayerActionBar");
  actionBar->setFixedHeight(30);
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
      background: transparent; border: none; color: #aaaaaa;
      font-size: 14px; border-radius: 3px; padding: 0px 4px;
      min-width: 22px; min-height: 22px;
    }
    QToolButton:hover { background: #3a3a3a; color: #dddddd; }
    QToolButton:pressed { background: #444444; }
    QToolButton:disabled { color: #555555; }
  )";

  addLayerButton = new QPushButton("+");
  addLayerButton->setObjectName("AddLayerBtn");
  addLayerButton->setFixedSize(22, 22);
  addLayerButton->setToolTip("New Layer");
  addLayerButton->setStyleSheet(R"(
    QPushButton#AddLayerBtn {
      background: transparent; border: none; color: #aaaaaa;
      font-size: 16px; border-radius: 3px; padding-bottom: 1px;
    }
    QPushButton#AddLayerBtn:hover { background: #3a3a3a; color: #dddddd; }
    QPushButton#AddLayerBtn:pressed { background: #444444; }
  )");

  auto *deleteLayerBtn = new QToolButton();
  deleteLayerBtn->setObjectName("DeleteLayerBtn");
  deleteLayerBtn->setText("\u232B");
  deleteLayerBtn->setFixedSize(22, 22);
  deleteLayerBtn->setToolTip("Delete Layer");
  deleteLayerBtn->setStyleSheet(iconBtnStyle);

  auto *duplicateLayerBtn = new QToolButton();
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

  // ── Collapse ──
  QToolButton *collapseBtn = layersDock->panelTitleBar()->collapseButton();
  connect(collapseBtn, &QToolButton::toggled, this,
          [collapseBtn, contentArea](bool expanded) {
            contentArea->setVisible(expanded);
            collapseBtn->setArrowType(expanded ? Qt::DownArrow : Qt::RightArrow);
          });

  // ── Action connections ──
  connect(deleteLayerBtn, &QToolButton::clicked, this,
          &MainWindow::deleteLayer);
  connect(duplicateLayerBtn, &QToolButton::clicked, this, [this]() {
    int row = layerList->currentRow();
    if (row >= 0)
      canvasWidget->duplicateLayer(row);
  });
  connect(addLayerButton, &QPushButton::clicked, canvasWidget,
          &CanvasWidget::addLayer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// Status Bar
// ═══════════════════════════════════════════════════════════════════════════════
void MainWindow::setupStatusBar() {
  statusCoords = new QLabel("X: 0  Y: 0");
  statusTool = new QLabel("Brush");
  statusSize = new QLabel(
      QString("%1 × %2")
          .arg(canvasWidget->getCanvasWidth())
          .arg(canvasWidget->getCanvasHeight()));

  statusBar()->addWidget(statusTool);
  statusBar()->addWidget(statusCoords);
  statusBar()->addPermanentWidget(statusSize);
}

void MainWindow::updateStatusBar(int x, int y) {
  statusCoords->setText(QString("X: %1  Y: %2").arg(x).arg(y));
}

// ═══════════════════════════════════════════════════════════════════════════════
// Layer List Refresh
// ═══════════════════════════════════════════════════════════════════════════════
void MainWindow::refreshLayerList() {
  layerList->blockSignals(true);
  layerList->clear();

  const auto &layers = canvasWidget->getDocument().getLayers();

  for (int i = 0; i < static_cast<int>(layers.size()); i++) {
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
      QString text = QInputDialog::getText(this, "Rename Layer", "Layer Name:",
                                           QLineEdit::Normal, "", &ok);
      if (ok && !text.isEmpty())
        canvasWidget->renameLayer(index, text.toStdString());
    });

    connect(widget, &LayerItemWidget::deleteRequested, this, [this, item]() {
      int index = layerList->row(item);
      if (index >= 0)
        canvasWidget->removeLayer(index);
    });

    connect(widget, &LayerItemWidget::clicked, this, [this, item]() {
      int index = layerList->row(item);
      if (index >= 0)
        layerList->setCurrentRow(index);
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

// ═══════════════════════════════════════════════════════════════════════════════
// Slots
// ═══════════════════════════════════════════════════════════════════════════════
void MainWindow::onLayerSelected(int row) {
  if (row < 0)
    canvasWidget->selectCanvas();
  else
    canvasWidget->setActiveLayer(row);
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
  QString text = QInputDialog::getText(this, "Rename Layer", "Layer Name:",
                                       QLineEdit::Normal, "", &ok);
  if (!ok || text.isEmpty())
    return;

  canvasWidget->renameLayer(row, text.toStdString());
}