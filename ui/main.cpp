#include "MainWindow.hpp"

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QThread>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Splash Screen
  QPixmap splashPixmap(":/icons/icons/splash.jpg");
  QSplashScreen splash(splashPixmap);

  splash.show();
  app.processEvents();

  // Keep splash visible for 10 seconds (demo)
  QThread::sleep(4);

  // Main Window
  MainWindow window;
  window.show();

  splash.finish(&window);

  return app.exec();
}