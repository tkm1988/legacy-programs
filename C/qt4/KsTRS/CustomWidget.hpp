
#pragma once

#include <QWidget>

class ctime;
class vector;
class QAction;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QCheckBox;
class QDoubleSpinBox;
class QPainter;
class QGraphicsItem;
class QColor;
class QFont;
class QFontDatabase;
class QStringList;

typedef struct IndividualPoint
{
  double x_start;
  double y_start;
  double x_end;
  double y_end;
} indPoint;

class IndividualImage
{
public:
  IndividualImage(indPoint *, QColor *, QColor *, QFont *);
  ~IndividualImage();
  indPoint	*getPoint();
  QColor	*getBGColor();
  QColor	*getTxtColor();
  QFont		*getFont();
private:
  indPoint	*Point;
  QColor	*BGColor;
  QColor	*TxtColor;
  QFont		*Font;
};  

class CustomWidget:public QWidget
{
  Q_OBJECT

  public:
  CustomWidget(QWidget *iParent, Qt::WindowFlags iFlag = 0);
  ~CustomWidget();

  private:
  void				 makeWidgets();
  static const int		 IND_WIDTH  = 10;
  static const int		 IND_HEIGHT = 3;
  std::vector<QDoubleSpinBox *>	 VectDSpin;
  std::vector<IndividualImage *> VectIndImg;

  QPushButton			*CrossoverButton;
  QPushButton			*ClearButton;
  QPushButton			*QuitButton;
  QCheckBox			*ModeCheckBox;
  QGridLayout			*EvalButtonsLayout;
  QHBoxLayout			*ButtonsLayout;
  QVBoxLayout			*WidgetLayout;

protected:
  void paintEvent(QPaintEvent *iPaintEvent);
					   
protected slots:
  void doCrossover();
  void doClear();
  void doEval();
};
