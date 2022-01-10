#include <QApplication>
#include "custommainwindow.hpp"

int main(int argc, char *argv[])
{
  QApplication application(argc, argv);
  CustomMainWindow mainWindow;
    
  return application.exec();
}
