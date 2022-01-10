#include "MainWindow.hpp"
#include "CustomWidget.hpp"

MainWindow::MainWindow(QWidget * iParent, Qt::WindowFlags iFlags):QMainWindow(iParent, iFlags),mWidget(0){

  mWidget = new CustomWidget(this);
  setCentralWidget(mWidget);
  setWindowTitle("Keynotes Theme Represent System");
  setFixedSize(1100,700);
  show();
}

MainWindow::~MainWindow(){
}
