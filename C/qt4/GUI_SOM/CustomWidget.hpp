#pragma once

#include <QWidget>
#include <boost/numeric/ublas/matrix.hpp>

typedef boost::numeric::ublas::matrix<double> dmat;

class som;
class BatchSOM;
class data;
class QAction;
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QLabel;
class QPushButton;
class QRadioButton;
class QSpinBox;
class QDoubleSpinBox;
class QGroupBox;
class QPainter;
class QTextCodec;

class CustomWidget:public QWidget{
  Q_OBJECT

  public:
  CustomWidget(QWidget *iParent, Qt::WindowFlags iFlag = 0);
  ~CustomWidget();

  private:
  void makeWidgets();

  som *s;
  BatchSOM *bs;
  dmat RefMatrix;

  bool ModeFlag, TypeFlag, StartFlag;
  double init_alpha, current_alpha;
  double init_sigma, current_sigma;
  int init_ltime, current_ltime;

  QPushButton *StartButton;
  QPushButton *ClearButton;
  QPushButton *QuitButton;

  QLabel *FileName;
  QLabel *NowAlpha;
  QLabel *NowSigma;
  QLabel *NowLtime;

  QLineEdit *FilePath;

  QDoubleSpinBox *AlphaSpinBox;
  QDoubleSpinBox *SigmaSpinBox;

  QSpinBox *LtimeSpinBox;
  QSpinBox *WidthSpinBox;
  QSpinBox *HeightSpinBox;

  QRadioButton *ProdMode;
  QRadioButton *NormMode;
  QRadioButton *NormalType;
  QRadioButton *BatchType;

  QGroupBox *InitParamGroupBox;
  QGroupBox *InitTypeGroupBox;
  QGroupBox *InitModeGroupBox;
  QGroupBox *MonitorGroupBox;

  QGridLayout *WidgetLayout;

  QVBoxLayout *ControlLayout;
  QVBoxLayout *ParamGroupBoxLayout;
  QVBoxLayout *TypeGroupBoxLayout;
  QVBoxLayout *ModeGroupBoxLayout;
  QVBoxLayout *MonitorGroupBoxLayout;

  QHBoxLayout *InputDatasetLayout;
  QHBoxLayout *SetupPanelLayout;
  QHBoxLayout *ButtonsLayout;

protected:
  void paintEvent(QPaintEvent *iPaintEvent);

protected slots:
  void doCommandStart();
  void doCommandClear();
  void doCommandSeqential();
  void doCommandBatch();
  void doCommandNorm();
  void doCommandProd();
  void doCommandWSpin(int);
  void doCommandHSpin(int);
};
