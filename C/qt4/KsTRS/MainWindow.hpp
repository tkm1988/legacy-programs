#pragma once

#include<QMainWindow>
class CustomWidget;

class MainWindow:public QMainWindow{
  Q_OBJECT
  
 public:
  MainWindow(QWidget *iParent=0, Qt::WindowFlags iFlags=0);
  ~MainWindow();

 private:
  CustomWidget *mWidget;
};
