#include <QTextCodec>
#include "MainWindow.hpp"
#include "CustomWidget.hpp"

MainWindow::MainWindow(QWidget * iParent, Qt::WindowFlags iFlags):QMainWindow(iParent, iFlags),mWidget(0){
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("EUC-JP"));

  mWidget = new CustomWidget(this);
  setCentralWidget(mWidget);
  setWindowTitle("SOM�ץ����");
  setFixedSize(800,700);
  show();
}

MainWindow::~MainWindow(){
}
