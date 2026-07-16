#include "MainWindow.hpp"
#include "CanvasWidget.hpp"
#include "LayerItemWidget.hpp"
#include "PanelDock.hpp"
#include <QAction>
#include <QColorDialog>
#include <QComboBox>
#include <QFileDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QImage>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QSlider>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>
#include <algorithm> // For std::copy
#include <cstdint>   // For uint8_t

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
// Global Stylesheet (Depth-focused Dark Theme)
// ═══════════════════════════════════════════════════════════════════════════════
void MainWindow::applyGlobalStyle() {
  setStyleSheet(R"(

*{
    font-family:"Segoe UI","Inter","SF Pro Display",sans-serif;
    color:#E2E8F0;
}

QMainWindow{
    background:#0A0D14;
}


/* ================= MENU ================= */

QMenuBar{
    background:#101520;
    border:none;
    border-bottom:1px solid #1F293D;
    padding:4px 8px;
    font-size:12px;
}

QMenuBar::item{
    background:transparent;
    padding:6px 12px;
    border-radius:4px;
}

QMenuBar::item:selected{
    background: #1E293B;
}

QMenuBar::item:pressed{
    background: #2563EB;
}


QMenu{
    background:#101520;
    border:1px solid #1F293D;
    padding:5px;
}

QMenu::item{
    padding:7px 28px;
    border-radius:4px;
}

QMenu::item:selected{
    background: #1E293B;
}

QMenu::separator{
    background: #1F293D;
    height:1px;
    margin:6px;
}



/* ================= TOOLBAR ================= */

QToolBar{
    background:#101520;
    border:none;
    border-bottom:1px solid #1F293D;
    spacing:8px;
    padding:6px;
}



/* ================= STATUS BAR ================= */

QStatusBar{
    background:#101520;
    border-top:1px solid #1F293D;
    min-height:24px;
}

QStatusBar QLabel{
    color: #94A3B8;
    padding:0px 10px;
    font-size:11px;
}



/* ================= TOOLTIP ================= */

QToolTip{
    background:#101520;
    border:1px solid #1F293D;
    color:#F1F5F9;
    padding:6px;
}



/* ================= SLIDER ================= */

QSlider::groove:horizontal{
    background:  #1F293D;
    height:4px;
    border-radius:2px;
}

QSlider::sub-page:horizontal{
    background: #2563EB;
    border-radius:2px;
}

QSlider::handle:horizontal{
    background: #EAEFF5;
    width:12px;
    height:12px;
    margin:-4px 0;
    border-radius:6px;
}

QSlider::handle:hover{
    background: white;
}



/* ================= COMBOBOX ================= */

QComboBox{
    background:#141B27;
    border:1px solid #1F293D;
    border-radius:4px;
    padding:4px 10px;
    min-height:24px;
}

QComboBox:hover{
    border:1px solid #2563EB;
}

QComboBox::drop-down{
    border:none;
    width:20px;
}

QComboBox QAbstractItemView{
    background:#141B27;
    border:1px solid #1F293D;
    selection-background-color: #2563EB;
}



/* ================= SCROLLBAR ================= */

QScrollBar:vertical{
    background:#0A0D14;
    width:10px;
}

QScrollBar::handle:vertical{
    background:#1F293D;
    border-radius:5px;
}

QScrollBar::handle:vertical:hover{
    background:#2563EB;
}

QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical{
    height:0;
}

QScrollBar::add-page:vertical,
QScrollBar::sub-page:vertical{
    background:none;
}



/* ================= DOCK ================= */

QDockWidget{
    background:#101520;
    border-left:1px solid #1F293D;
}

QDockWidget::title{
    background:#101520;
    padding:8px;
    text-align:left;
    border-bottom:1px solid #1F293D;
}

)");
}

QIcon getTintedIcon(const QString &path, const QColor &color) {
  QIcon icon(path);

  QPixmap pixmap(20, 20);
  pixmap.fill(Qt::transparent);

  QPainter painter(&pixmap);

  icon.paint(&painter, QRect(0, 0, 20, 20), Qt::AlignCenter, QIcon::Normal,
             QIcon::On);

  painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
  painter.fillRect(pixmap.rect(), color);
  painter.end();

  return QIcon(pixmap);
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
  addToolBarBreak(); // Force tool options onto a clean new row
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
  connect(
      layerList, &LayerListWidget::layersReordered, this,
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

  QAction *newAction = new QAction("New", this);
  newAction->setShortcut(QKeySequence::New);
  connect(newAction, &QAction::triggered, this, []() {
    // TODO
  });
  fileMenu->addAction(newAction);

  // ── Open Action ──
  QAction *openAction = new QAction("Open...", this);
  openAction->setShortcut(QKeySequence::Open); // Ctrl+O
  connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
  fileMenu->addAction(openAction);

  // ── Save Action ──
  QAction *saveAction = new QAction("Save As...", this);
  saveAction->setShortcut(QKeySequence::Save); // Ctrl+S
  connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
  fileMenu->addAction(saveAction);

  fileMenu->addSeparator();

  QAction *exitAction = new QAction("Exit", this);
  exitAction->setShortcut(QKeySequence::Quit);
  connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
  fileMenu->addAction(exitAction);

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

  QAction *addLayerAction = new QAction("Add Layer", this);
  addLayerAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_N));
  connect(addLayerAction, &QAction::triggered, this,
          [this]() { canvasWidget->addLayer(); });
  layerMenu->addAction(addLayerAction);

  QAction *deleteLayerAction = new QAction("Delete Layer", this);
  connect(deleteLayerAction, &QAction::triggered, this,
          [this](bool) { deleteLayer(); });
  layerMenu->addAction(deleteLayerAction);

  QAction *renameLayerAction = new QAction("Rename Layer", this);
  connect(renameLayerAction, &QAction::triggered, this,
          [this](bool) { renameLayer(); });
  layerMenu->addAction(renameLayerAction);

  // ── View ──
  menuBar()->addMenu("&View");

  // ── Help ──
  menuBar()->addMenu("&Help");
}

// ═══════════════════════════════════════════════════════════════════════════════
// Tool Button Helper
// ═══════════════════════════════════════════════════════════════════════════════
static const QString kToolBtnStyle = R"(
QToolButton{
    background:transparent;
    border:none;
    border-radius:5px;
    min-width:28px;
    min-height:28px;
    padding:4px;
}

QToolButton:hover{
    background: #1E293B;
}

QToolButton:pressed{
    background: #2563EB;
}

QToolButton[active="true"]{
    background: #2563EB;
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
  bool isBrushLike = (btn == brushBtn || btn == eraserBtn || btn == fillBtn);
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
  toolbar->setFixedHeight(38); // Unified height

  toolbar->setContentsMargins(6, 4, 6, 4);
  toolbar->setMovable(false);
  toolbar->setIconSize(QSize(18, 18));

  auto makeTool = [&](const QString &icon, const QString &tip) {
    auto *btn = new QToolButton();
    btn->setIcon(getTintedIcon(icon, QColor("#FFFFFF")));
    btn->setIconSize(QSize(18, 18));
    btn->setText("");
    btn->setToolTip(tip);
    btn->setStyleSheet(kToolBtnStyle);
    btn->setFixedSize(30, 30);
    toolbar->addWidget(btn);
    return btn;
  };
  brushBtn = makeTool(":/icons/icons/brush.svg", "Brush");
  eraserBtn = makeTool(":/icons/icons/eraser.svg", "Eraser");
  fillBtn = makeTool(":/icons/icons/paint-bucket.svg", "Fill");
  selectBtn = makeTool(":/icons/icons/mouse-pointer-2.svg", "Selection");
  moveBtn = makeTool(":/icons/icons/move.svg", "Move");
  shapeBtn = makeTool(":/icons/icons/square.svg", "Shape");

  // Separator
  toolbar->addSeparator();

  // Color swatch
  colorSwatch = new QWidget();
  colorSwatch->setFixedSize(20, 20);
  colorSwatch->setStyleSheet(
      "background: #000000ff; border: 1px solid #1F293D; border-radius: 3px;");
  colorSwatch->setToolTip("Current Color");
  colorSwatch->setCursor(Qt::PointingHandCursor);
  toolbar->addWidget(colorSwatch);

  auto *colorBtn = new QToolButton();
  colorBtn->setIcon(getTintedIcon(":/icons/icons/palette.svg", Qt::white));
  colorBtn->setIconSize(QSize(18, 18));
  colorBtn->setText("");
  colorBtn->setToolTip("Pick Color");
  colorBtn->setStyleSheet(kToolBtnStyle);
  colorBtn->setFixedSize(30, 30);
  toolbar->addWidget(colorBtn);

  toolbar->addSeparator();

  // Clear buttons
  auto *clearLayerBtn =
      makeTool(":/icons/icons/delete-layer.svg", "Clear Layer");
  auto *clearAllBtn =
      makeTool(":/icons/icons/brush-cleaning.svg", "Clear Canvas");

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
        QString(
            "background: %1; border: 2px solid #A8C0E0; border-radius: 4px;")
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
  optionsBar->setFixedHeight(38); // Match main Toolbar height perfectly
  optionsBar->setStyleSheet(R"(
    QToolBar {
      background: #101520;
      border-bottom: 1px solid #1F293D;
      spacing: 12px;
      padding: 3px 6px;
    }
    QLabel {
      color: #94A3B8;
      font-size: 11px;
    }
  )");

  // ── Brush/Eraser options ──
  brushOptionsWidget = new QWidget();
  brushOptionsWidget->setSizePolicy(QSizePolicy::Maximum,
                                    QSizePolicy::Preferred);
  auto *brushLayout = new QHBoxLayout(brushOptionsWidget);
  brushLayout->setContentsMargins(0, 0, 0, 0);
  brushLayout->setSpacing(6);

  auto *sizeLabel = new QLabel("Size:");
  brushSizeSlider = new QSlider(Qt::Horizontal);
  brushSizeSlider->setRange(1, 100);
  brushSizeSlider->setValue(2);
  brushSizeSlider->setFixedWidth(140);

  brushSizeLabel = new QLabel("10");
  brushSizeLabel->setFixedWidth(18);
  brushSizeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

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
  shapeOptionsWidget->setSizePolicy(QSizePolicy::Maximum,
                                    QSizePolicy::Preferred);
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
      background: #141B27;
      color: #94A3B8;
      border: 1px solid #1F293D;
      border-radius: 3px;
      padding: 2px 8px;
      font-size: 11px;
    }
    QToolButton:hover { background: #1E293B; }
    QToolButton:checked {
      background: #2563EB;
      border-color: #3B82F6;
      color: #ffffff;
    }
  )");

  shapeLayout->addWidget(shapeLabel);
  shapeLayout->addWidget(shapeTypeCombo);
  shapeLayout->addWidget(shapeFilledBtn);

  optionsBar->addWidget(shapeOptionsWidget);
  shapeOptionsWidget->setVisible(false);

  connect(
      shapeTypeCombo, &QComboBox::currentIndexChanged, this, [this](int idx) {
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
    QWidget#LayerPanelRoot { background: #101520; }
  )");

  QVBoxLayout *rootLayout = new QVBoxLayout(panelRoot);
  rootLayout->setContentsMargins(0, 0, 0, 0);
  rootLayout->setSpacing(0);

  QWidget *contentArea = new QWidget();
  contentArea->setObjectName("LayerContentArea");
  contentArea->setStyleSheet(R"(
    QWidget#LayerContentArea { background: #101520; }
  )");

  QVBoxLayout *contentLayout = new QVBoxLayout(contentArea);
  contentLayout->setContentsMargins(0, 0, 0, 0);
  contentLayout->setSpacing(0);

  // ── Layer list (Sunken Panel) ──
  layerList = new LayerListWidget(this);
  layerList->setObjectName("LayerList");
  layerList->setStyleSheet(R"(
    QListWidget#LayerList {
      background: #141B27;
      border: none;
      border-top: 1px solid #1F293D;
      outline: 0;
    }
    QListWidget#LayerList::item {
      background: #101520;
      border-bottom: 1px solid #1F293D;
      padding: 0px;
    }
    QListWidget#LayerList::item:hover {
      background: #1E293B;
    }
    QListWidget#LayerList::item:selected {
      background: #2563EB;
    }
    QListWidget#LayerList::item:selected:hover {
      background:#1D4ED8;
    }
    QScrollBar:vertical {
      background: #0A0D14; width: 8px; margin: 0;
    }
    QScrollBar::handle:vertical {
      background: #1F293D; border-radius: 4px; min-height: 24px;
    }
    QScrollBar::handle:vertical:hover { background: #2563EB; }
    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
    QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }
  )");
  layerList->setMinimumHeight(80);

  // ── Separator ──
  QFrame *separator = new QFrame();
  separator->setFrameShape(QFrame::HLine);
  separator->setFixedHeight(1);
  separator->setStyleSheet("background: #1F293D; border: none;");

  // ── Action bar ──
  QWidget *actionBar = new QWidget();
  actionBar->setObjectName("LayerActionBar");
  actionBar->setFixedHeight(36);
  actionBar->setStyleSheet(R"(
    QWidget#LayerActionBar {
      background: #101520;
      border-top: 1px solid #1F293D;
    }
  )");

  QHBoxLayout *actionLayout = new QHBoxLayout(actionBar);
  actionLayout->setContentsMargins(4, 3, 4, 3);
  actionLayout->setSpacing(2);

  const QString iconBtnStyle = R"(
    QToolButton {
      background: transparent; border: none; color: #94A3B8;
      font-size: 14px; border-radius: 3px; padding: 0px 4px;
      min-width: 22px; min-height: 22px;
    }
    QToolButton:hover { background: #1E293B; color: #ffffff; }
    QToolButton:pressed { background: #2563EB; }
    QToolButton:disabled { color: #1F293D; }
  )";

  addLayerButton = new QPushButton("+");
  addLayerButton->setObjectName("AddLayerBtn");
  addLayerButton->setFixedSize(22, 22);
  addLayerButton->setToolTip("New Layer");
  addLayerButton->setStyleSheet(R"(
    QPushButton#AddLayerBtn {
      background: transparent; border: none; color: #94A3B8;
      font-size: 16px; border-radius: 3px; padding-bottom: 1px;
    }
    QPushButton#AddLayerBtn:hover { background: #1E293B; color: #ffffff; }
    QPushButton#AddLayerBtn:pressed { background: #2563EB; }
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

  // ── Collapse (Preserving custom SVG chevrons) ──
  QToolButton *collapseBtn = layersDock->panelTitleBar()->collapseButton();
  connect(collapseBtn, &QToolButton::toggled, this,
          [contentArea](bool expanded) { contentArea->setVisible(expanded); });

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

  // FIX: Access the document properties to get correct dimensions
  statusSize = new QLabel(QString("%1 × %2")
                              .arg(canvasWidget->getDocument().getWidth())
                              .arg(canvasWidget->getDocument().getHeight()));

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

    item->setSizeHint(QSize(0, 46));
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
  QString text = QInputDialog::getText(
      this, "Rename Layer", "Layer Name:", QLineEdit::Normal, "", &ok);
  if (!ok || text.isEmpty())
    return;

  canvasWidget->renameLayer(row, text.toStdString());
}

void MainWindow::openFile() {
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("Open Image"), "",
      tr("Images (*.png *.jpg *.jpeg *.bmp *.tga);;All Files (*)"));

  if (filePath.isEmpty()) {
    return;
  }

  QImage loadedImage(filePath);
  if (loadedImage.isNull()) {
    QMessageBox::critical(this, tr("Error"), tr("Failed to load image file."));
    return;
  }

  QImage rgbaImage = loadedImage.convertToFormat(QImage::Format_RGBA8888);
  int width = rgbaImage.width();
  int height = rgbaImage.height();

  // FIX: Sync both Document and Canvas dimensions
  auto &doc = canvasWidget->getDocument();
  doc.setWidth(width);
  doc.setHeight(height);

  auto &canvas = canvasWidget->getCanvas();
  canvas.setWidth(width);
  canvas.setHeight(height);

  auto &pixels = canvas.getPixels();
  pixels.resize(width * height * 4);

  const uint8_t *srcBits =
      reinterpret_cast<const uint8_t *>(rgbaImage.constBits());
  std::copy(srcBits, srcBits + (width * height * 4), pixels.begin());

  statusSize->setText(QString("%1 × %2").arg(width).arg(height));

  canvasWidget->update();
}

void MainWindow::saveFile() {
  auto &canvas = canvasWidget->getCanvas();
  int width = canvas.getWidth();
  int height = canvas.getHeight();
  auto &pixels = canvas.getPixels();

  if (pixels.empty() || width <= 0 || height <= 0) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Canvas is empty. Nothing to save."));
    return;
  }

  QString filePath = QFileDialog::getSaveFileName(
      this, tr("Save Image As"), "untitled.png",
      tr("PNG Image (*.png);;JPEG Image (*.jpg *.jpeg);;BMP Image (*.bmp);;All "
         "Files (*)"));

  if (filePath.isEmpty()) {
    return;
  }

  QImage qImage(reinterpret_cast<uchar *>(pixels.data()), width, height,
                QImage::Format_RGBA8888);

  if (!qImage.save(filePath)) {
    QMessageBox::critical(this, tr("Error"), tr("Failed to save image file."));
  }
}