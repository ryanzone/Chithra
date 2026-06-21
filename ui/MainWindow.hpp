#pragma once

#include <QMainWindow>

class CanvasWidget;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

private:
  CanvasWidget *canvasWidget;
};