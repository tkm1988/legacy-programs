#include "CustomWidget.hpp"
#include "som.hpp"
#include "../../data.h"
#include <climits>
#include <QApplication>
#include <QtGUI>
#include <QAction>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QPainter>
#include <QTextCodec>

CustomWidget::CustomWidget(QWidget *iParent, Qt::WindowFlags iFlags):QWidget(iParent, iFlags){
  makeWidgets();
}

CustomWidget::~CustomWidget(){
}

void CustomWidget::makeWidgets(){
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("EUC-JP"));

  init_alpha = 0.0;
  init_sigma = 0.0;
  init_ltime = 0.0;
  current_alpha = 0.0;
  current_sigma = 0.0;
  current_ltime = 0;

  WidgetLayout		= new QGridLayout();
  ControlLayout         = new QVBoxLayout();
  ParamGroupBoxLayout	= new QVBoxLayout();
  TypeGroupBoxLayout	= new QVBoxLayout();
  ModeGroupBoxLayout	= new QVBoxLayout();
  MonitorGroupBoxLayout = new QVBoxLayout();

  InputDatasetLayout = new QHBoxLayout();
  SetupPanelLayout = new QHBoxLayout();
  ButtonsLayout	   = new QHBoxLayout();

  //PushButton--------------
  StartButton	    = new QPushButton("Start");
  connect(StartButton, SIGNAL(clicked()), this, SLOT(doCommandStart()));
  StartFlag = false;

  ClearButton	    = new QPushButton("Clear");
  connect(ClearButton, SIGNAL(clicked()), this, SLOT(doCommandClear()));

  QuitButton	    = new QPushButton("Quit");
  connect(QuitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

  //Label and TextEdit--------------
  FileName    = new QLabel("ファイル（Path）：");
  NowAlpha    = new QLabel("学習率：" + QString::number(current_alpha));
  NowSigma    = new QLabel("近傍範囲：" + QString::number(current_sigma));
  NowLtime    = new QLabel("学習回数：" + QString::number(current_ltime));

  FilePath = new QLineEdit("point.txt",this);

  //SpinBox--------------
  AlphaSpinBox	= new QDoubleSpinBox();
  AlphaSpinBox->setRange(0.0, 0.99);
  AlphaSpinBox->setSingleStep(0.01);
  AlphaSpinBox->setValue(0.99);
  AlphaSpinBox->setPrefix("学習率：");

  SigmaSpinBox	= new QDoubleSpinBox();
  SigmaSpinBox->setMinimum(0.0);
  SigmaSpinBox->setSingleStep(0.1);
  SigmaSpinBox->setValue(5.0);
  SigmaSpinBox->setPrefix("近傍範囲：");

  LtimeSpinBox	= new QSpinBox();
  LtimeSpinBox->setRange(1, INT_MAX - 1);
  LtimeSpinBox->setSingleStep(1);
  LtimeSpinBox->setValue(100);
  LtimeSpinBox->setPrefix("学習回数：");

  WidthSpinBox	= new QSpinBox();
  WidthSpinBox->setRange(1, INT_MAX - 1);
  WidthSpinBox->setSingleStep(1);
  WidthSpinBox->setValue(10);
  WidthSpinBox->setPrefix("横：");
  WidthSpinBox->setSuffix("個");
  connect(WidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(doCommandWSpin(int))); 

  HeightSpinBox = new QSpinBox();
  HeightSpinBox->setRange(1, INT_MAX - 1);
  HeightSpinBox->setSingleStep(1);
  HeightSpinBox->setValue(10);
  HeightSpinBox->setPrefix("縦：");
  HeightSpinBox->setSuffix("個");
  connect(HeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(doCommandHSpin(int))); 
  
  //RadioButton--------------
  ProdMode		= new QRadioButton("内積型");
  ProdMode->setChecked(true);
  ModeFlag = true;
  connect(ProdMode, SIGNAL(clicked()), this, SLOT(doCommandProd())); 

  NormMode		= new QRadioButton("距離型");
  connect(NormMode, SIGNAL(clicked()), this, SLOT(doCommandNorm())); 

  NormalType		      = new QRadioButton("逐次方式");
  NormalType->setChecked(true);
  TypeFlag = true;
  connect(NormalType, SIGNAL(clicked()), this, SLOT(doCommandSeqential())); 

  BatchType		 = new QRadioButton("バッチ方式");
  connect(BatchType, SIGNAL(clicked()), this, SLOT(doCommandBatch())); 

  //GroupBox--------------
  InitParamGroupBox = new QGroupBox("初期値");
  InitTypeGroupBox  = new QGroupBox("実行タイプ");
  InitModeGroupBox  = new QGroupBox("計算方法");
  MonitorGroupBox   = new QGroupBox("現在");

  //Layout--------------
  ParamGroupBoxLayout->addWidget(WidthSpinBox);
  ParamGroupBoxLayout->addWidget(HeightSpinBox);
  ParamGroupBoxLayout->addWidget(LtimeSpinBox);
  ParamGroupBoxLayout->addWidget(AlphaSpinBox);
  ParamGroupBoxLayout->addWidget(SigmaSpinBox);
  InitParamGroupBox->setLayout(ParamGroupBoxLayout);

  ModeGroupBoxLayout->addWidget(ProdMode);
  ModeGroupBoxLayout->addWidget(NormMode);
  InitModeGroupBox->setLayout(ModeGroupBoxLayout);
  
  TypeGroupBoxLayout->addWidget(NormalType);
  TypeGroupBoxLayout->addWidget(BatchType);
  InitTypeGroupBox->setLayout(TypeGroupBoxLayout);

  MonitorGroupBoxLayout->addWidget(NowLtime);
  MonitorGroupBoxLayout->addWidget(NowAlpha);
  MonitorGroupBoxLayout->addWidget(NowSigma);
  MonitorGroupBox->setLayout(MonitorGroupBoxLayout);
  
  InputDatasetLayout->addWidget(FileName);
  InputDatasetLayout->addWidget(FilePath);
  
  SetupPanelLayout->addWidget(InitParamGroupBox);
  SetupPanelLayout->addWidget(InitModeGroupBox);
  SetupPanelLayout->addWidget(InitTypeGroupBox);
  //SetupPanelLayout->addWidget(MonitorGroupBox);

  ButtonsLayout->setAlignment(Qt::AlignRight);
  ButtonsLayout->addWidget(StartButton);
  ButtonsLayout->addWidget(ClearButton);
  ButtonsLayout->addWidget(QuitButton);

  ControlLayout->addLayout(InputDatasetLayout);
  ControlLayout->addLayout(SetupPanelLayout);
  ControlLayout->addLayout(ButtonsLayout);

  WidgetLayout->addLayout(ControlLayout, 0, 0);
  WidgetLayout->setRowStretch(1, 300);

  setLayout(WidgetLayout);

  RefMatrix.resize((WidthSpinBox->value() * HeightSpinBox->value()), 2);
  for(int y = 0, y_max = HeightSpinBox->value(); y < y_max; ++y){
    for(int x = 0, x_max = WidthSpinBox->value(); x < x_max; ++x){
      RefMatrix(y * x_max + x, 0) = 10*x + (400 - 5*(x_max - 1));
      RefMatrix(y * x_max + x, 1) = 10*y + (505 - 5*(y_max - 1));
    }
  }
}

void CustomWidget::paintEvent(QPaintEvent *iPaintEvent){
  //Window 800 x 700
  QPainter mPainter(this);

  mPainter.setBrush(QBrush("#ffffff"));
  mPainter.drawRect(20, 330, 760, 350);

  mPainter.setBrush(QBrush("#000000"));
  
  for(int i = 0, i_max = static_cast<int>(RefMatrix.size1()); i < i_max; ++i){
    mPainter.drawEllipse(static_cast<int>(RefMatrix(i, 0)), static_cast<int>(RefMatrix(i, 1)), 5, 5);
    if(((i + 1) % WidthSpinBox->value()) > 0){
      mPainter.drawLine(static_cast<int>(RefMatrix(i, 0)), static_cast<int>(RefMatrix(i, 1)), 
			static_cast<int>(RefMatrix(i + 1, 0)), static_cast<int>(RefMatrix(i + 1, 1)));
    }
    if(i < (i_max - WidthSpinBox->value())){
      mPainter.drawLine(static_cast<int>(RefMatrix(i, 0)), static_cast<int>(RefMatrix(i, 1)), 
			static_cast<int>(RefMatrix(i + WidthSpinBox->value(), 0)), static_cast<int>(RefMatrix(i + WidthSpinBox->value(), 1)));
    }
  }

  update();
}

void CustomWidget::doCommandStart(){
  StartFlag = true;
  init_alpha = AlphaSpinBox->value();
  init_sigma = SigmaSpinBox->value();
  init_ltime = LtimeSpinBox->value();
  current_alpha = 0.0;
  current_sigma = 0.0;
  current_ltime = 0;

  dmat Data = data::generator(FilePath->text().toStdString(), 2);

  if(NormalType->isChecked()){
    s = new som(static_cast<int>(WidthSpinBox->value()), static_cast<int>(HeightSpinBox->value()), static_cast<int>(Data.size2()));
    s->setData(&Data);
    s->setLtimeLimit(init_ltime);
    for(int clt_max = init_ltime; current_ltime < clt_max; ++current_ltime){
      RefMatrix.clear();
      if(ProdMode->isChecked()){
	s->ProdLearning(init_ltime, init_alpha, init_sigma, true);
      }else{
	s->NormLearning(init_ltime, init_alpha, init_sigma, true);
      }

      current_alpha = s->getAlpha();
      current_sigma = s->getSigma();
      RefMatrix = s->getRefMatrix();
      
      dvect MinVectX = column(RefMatrix, 0);
      dvect MinVectY = column(RefMatrix, 1);
      double min_x = MinVectX(min_element(MinVectX.begin(), MinVectX.end()).index());
      double min_y = MinVectY(min_element(MinVectY.begin(), MinVectY.end()).index());

      for(int i = 0, i_max = static_cast<int>(RefMatrix.size1()); i < i_max; ++i){
	RefMatrix(i, 0) = (RefMatrix(i, 0) - min_x) * 40 + 20;
	RefMatrix(i, 1) = (RefMatrix(i, 1) - min_y) * 20 + 330;
      }
    }
  }else{
    bs = new BatchSOM(static_cast<int>(WidthSpinBox->value()), static_cast<int>(HeightSpinBox->value()), &Data);
    bs->setLtimeLimit(init_ltime);
    for(int clt_max = init_ltime; current_ltime < clt_max; ++current_ltime){
      if(ProdMode->isChecked()){
	bs->ProdLearning(init_ltime, true);
      }else{
	bs->NormLearning(init_ltime, true);
      }

      RefMatrix = bs->getRefMatrix();
      for(int i = 0, i_max = static_cast<int>(RefMatrix.size1()); i < i_max; ++i){
	RefMatrix(i, 0) = RefMatrix(i, 0) * 1.1;
	RefMatrix(i, 1) = RefMatrix(i, 1) * 1.1;
      }
    }
  }
}
void CustomWidget::doCommandClear(){
  StartFlag = false;
  current_alpha = 0.0;
  current_sigma = 0.0;
  current_ltime = 0;
  AlphaSpinBox->setValue(0.99);
  SigmaSpinBox->setValue(5.0);
  LtimeSpinBox->setValue(100);
  WidthSpinBox->setValue(10);
  HeightSpinBox->setValue(10);
  ProdMode->setChecked(true);
  NormMode->setChecked(false);
  ModeFlag = true;
  NormalType->setChecked(true);
  BatchType->setChecked(false);
  TypeFlag = true;
  FilePath->setText("point.txt");
  NowAlpha->setText("学習率：" + QString::number(current_alpha));
  NowSigma->setText("近傍範囲："+ QString::number(current_sigma));
  NowLtime->setText("学習回数："+ QString::number(current_ltime));

  RefMatrix.resize((WidthSpinBox->value() * HeightSpinBox->value()), 2);
  for(int y = 0, y_max = HeightSpinBox->value(); y < y_max; ++y){
    for(int x = 0, x_max = WidthSpinBox->value(); x < x_max; ++x){
      RefMatrix(y * x_max + x, 0) = 10*x + (400 - 5*(x_max - 1));
      RefMatrix(y * x_max + x, 1) = 10*y + (505 - 5*(y_max - 1));
    }
  }
}

void CustomWidget::doCommandProd(){
  ModeFlag = true;
  NormMode->setChecked(false);
}

void CustomWidget::doCommandNorm(){
  ModeFlag = false;
  ProdMode->setChecked(false);
}

void CustomWidget::doCommandSeqential(){
  TypeFlag = true;
  BatchType->setChecked(false);
}

void CustomWidget::doCommandBatch(){
  TypeFlag = false;
  NormalType->setChecked(false);
}

void CustomWidget::doCommandWSpin(int xl){
  RefMatrix.resize((WidthSpinBox->value() * HeightSpinBox->value()), 2);
  for(int y = 0, y_max = HeightSpinBox->value(); y < y_max; ++y){
    for(int x = 0; x < xl; ++x){
      RefMatrix(y * xl + x, 0) = 10*x + (400 - 5*(xl - 1));
      RefMatrix(y * xl + x, 1) = 10*y + (505 - 5*(y_max - 1));
    }
  }
}

void CustomWidget::doCommandHSpin(int yl){
  RefMatrix.resize((WidthSpinBox->value() * HeightSpinBox->value()), 2);
  for(int y = 0; y < yl; ++y){
    for(int x = 0, x_max = WidthSpinBox->value(); x < x_max; ++x){
      RefMatrix(y * x_max + x, 0) = 10*x + (400 - 5*(x_max - 1));
      RefMatrix(y * x_max + x, 1) = 10*y + (505 - 5*(yl - 1));
    }
  }
}
