#include "MainWindow.hpp"

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
<<<<<<< HEAD
#include <QThread>
=======
#include <QTimer>
>>>>>>> c51d724 (feat: implement move and selection tools with UI icons and core integration)

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

<<<<<<< HEAD
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
=======
  // Splash image
  QPixmap splashPixmap(":/icons/icons/Chithra.png");

  // Scale if needed
  splashPixmap = splashPixmap.scaled(400, 400, Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);

  QSplashScreen splash(splashPixmap);
  splash.setWindowFlag(Qt::FramelessWindowHint);
  splash.show();

  splash.showMessage("Loading Chithra...", Qt::AlignBottom | Qt::AlignCenter,
                     Qt::white);

  app.processEvents();

  MainWindow *window = new MainWindow;

  // Show main window after 2 seconds
  QTimer::singleShot(2000, [&]() {
    window->show();
    splash.finish(window);
  });
>>>>>>> c51d724 (feat: implement move and selection tools with UI icons and core integration)

  splash.finish(&window);

  return app.exec();
}