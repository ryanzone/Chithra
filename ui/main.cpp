#include "MainWindow.hpp"

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[]) {
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