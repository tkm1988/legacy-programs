#include <QApplication>
#include <QMainWindow>
#include <QTextCodec>

int main(int argc, char* argv[])
{
      
  QApplication    application(argc, argv);
    
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("EUC-JP"));
    
  QMainWindow     mainWindow;
  mainWindow.setWindowTitle("日本語の表示ができた！");
    
  mainWindow.resize(300,200);
  mainWindow.show();
    
  return application.exec();
}
