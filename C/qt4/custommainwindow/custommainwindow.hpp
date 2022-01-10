#pragma once
#include <QMainWindow>

class CustomMainWindow : public QMainWindow{
  Q_OBJECT

  public:
  CustomMainWindow(QWidget * iParent = 0, Qt::WindowFlags iFlags = 0);
  ~CustomMainWindow();
};
