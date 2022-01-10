#include "CustomWidget.hpp"

#include <ctime>
#include <vector>
#include <boost/random.hpp>
#include <QApplication>
#include <QtGUI>
#include <QAction>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPainter>
#include <QGraphicsItem>
#include <QColor>
#include <QFont>
#include <QFontDatabase>
#include <QStringList>

using namespace std;
using namespace boost;

IndividualImage::IndividualImage(indPoint *Point, QColor *BGColor, QColor *TxtColor, QFont *Font)
{
  IndividualImage::Point    = Point;
  IndividualImage::BGColor  = BGColor;
  IndividualImage::TxtColor = TxtColor;
  IndividualImage::Font	    = Font;
}
IndividualImage::~IndividualImage()
{
}
indPoint *IndividualImage::getPoint()
{
  return Point;
}
QColor *IndividualImage::getBGColor()
{
  return BGColor;
}
QColor *IndividualImage::getTxtColor()
{
  return TxtColor;
}
QFont *IndividualImage::getFont()
{
  return Font;
}

CustomWidget::CustomWidget(QWidget *iParent, Qt::WindowFlags iFlags):QWidget(iParent, iFlags)
{
  makeWidgets();
}

CustomWidget::~CustomWidget(){}

void CustomWidget::makeWidgets()
{
  CrossoverButton   = new QPushButton("CROSSOVER");
  ClearButton	    = new QPushButton("Clear");
  QuitButton	    = new QPushButton("Quit");
  ModeCheckBox	    = new QCheckBox("Use the Original Method");
  EvalButtonsLayout = new QGridLayout();
  ButtonsLayout	    = new QHBoxLayout();
  WidgetLayout	    = new QVBoxLayout();

  QFontDatabase Database;
  QStringList	FontList = Database.families(QFontDatabase::Japanese);
  mt19937       generator_for_qcolor(static_cast<unsigned long>(time(0)));
  mt19937       generator_for_font(static_cast<unsigned long>(time(0)));
  uniform_smallint<> distribution_for_qcolor(0, 255);
  uniform_smallint<> distribution_for_font(0, FontList.size());
  variate_generator<mt19937&, uniform_smallint<> > rand_for_qcolor(generator_for_qcolor,
								   distribution_for_qcolor);
  variate_generator<mt19937&, uniform_smallint<> > rand_for_font(generator_for_font,
								 distribution_for_font);
  
  for(int col = 0; col < IND_WIDTH; ++col)
  {
    for(int row = 0; row < IND_HEIGHT; ++row)
    {
      QDoubleSpinBox    *work     = new QDoubleSpinBox();
      indPoint		 Point	  = {col*100, row*50, col+100, row+50};
      QColor		*BGColor  = new QColor(static_cast<int>(rand_for_qcolor()),
					       static_cast<int>(rand_for_qcolor()),
					       static_cast<int>(rand_for_qcolor()));
      QColor		*TxtColor = new QColor(static_cast<int>(rand_for_qcolor()),
					       static_cast<int>(rand_for_qcolor()),
					       static_cast<int>(rand_for_qcolor()));
      QFont		 TextFont(FontList.at(static_cast<int>(rand_for_font())), 11);     
      IndividualImage	*Ind	  = new IndividualImage(&Point, 
							BGColor, 
							TxtColor,
							&TextFont);
      VectIndImg.push_back(Ind);

      connect(work, SIGNAL(valueChanged(int)), this, SLOT(doEval()));
      work->setRange(0, 10);
      work->setSingleStep(1);
      work->setValue(0);
      work->setPrefix("Eval:");
      work->setFixedWidth(85);
      VectDSpin.push_back(work);
      
      EvalButtonsLayout->addWidget(work, row, col, Qt::AlignBottom);
    }
  }
  
  connect(CrossoverButton, SIGNAL(clicked()), this, SLOT(doCrossover()));
  connect(ClearButton, SIGNAL(clicked()), this, SLOT(doClear()));
  connect(QuitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

  ButtonsLayout->setAlignment(Qt::AlignRight);
  ButtonsLayout->addWidget(ModeCheckBox);
  ButtonsLayout->addWidget(CrossoverButton);
  ButtonsLayout->addWidget(ClearButton);
  ButtonsLayout->addWidget(QuitButton);
  WidgetLayout->addLayout(EvalButtonsLayout);
  WidgetLayout->addLayout(ButtonsLayout);
  setLayout(WidgetLayout);
}

void CustomWidget::paintEvent(QPaintEvent *iPaintEvent)
{
  QPainter mPainter(this);

  update();
}

void CustomWidget::doCrossover()
{
  
}

void CustomWidget::doClear()
{
  
}

void CustomWidget::doEval(){
  
}
