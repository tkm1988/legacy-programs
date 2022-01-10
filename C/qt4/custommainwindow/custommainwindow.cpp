#include "custommainwindow.hpp"

CustomMainWindow::CustomMainWindow(QWidget * iParent, Qt::WindowFlags iFlags)   // (3)
  : QMainWindow(iParent, iFlags)
{
  setWindowTitle(tr("Custom Main Window"));
  resize(350,250);
  show();
}

CustomMainWindow::~CustomMainWindow()
{

}
