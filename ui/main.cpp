#include "MainWindow.hpp"

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QTimer>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

int main(int argc, char *argv[]) {
#ifdef _WIN32
  HWND console = GetConsoleWindow();
  if (console != nullptr)
    ShowWindow(console, SW_HIDE);
#endif

  QApplication app(argc, argv);

  QPixmap splashPixmap(":/icons/icons/Chithra.png");
  splashPixmap = splashPixmap.scaled(400, 400, Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);

  QSplashScreen splash(splashPixmap);
  splash.setWindowFlag(Qt::FramelessWindowHint);
  splash.show();
  splash.showMessage("Loading Chithra...", Qt::AlignBottom | Qt::AlignCenter,
                     Qt::white);

  app.processEvents();

  MainWindow window;

  QTimer::singleShot(2000, [&]() {
    window.show();
    splash.finish(&window);
  });

  return app.exec();
}