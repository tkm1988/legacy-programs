#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char *argv[]){
  QApplication application(argc, argv);
  MainWindow mainWindow;

  return application.exec();
}