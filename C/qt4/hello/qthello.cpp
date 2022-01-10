/****************************************************************
 **
 ** Qt tutorial 1
 **
 ****************************************************************/
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]){
  QApplication application(argc, argv);
  QMainWindow mainWindow;
    
  mainWindow.setWindowTitle("Hello World !");
  mainWindow.resize(300,200);
  mainWindow.show();
    
  return application.exec();
}
