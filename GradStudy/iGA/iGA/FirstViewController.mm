//
//  FirstViewController.m
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/07/16.
//  Copyright (c) 2412 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//


#import "IgaGene.h"
#import "NSArrayWrapperForUblasNumericalData.h"
#import "kmeans.h"

#if defined (nil)
#undef nil
#endif

#if defined (Nil)
#undef Nil
#endif

#define nil cpp_nil
#define Nil cpp_nil

#import <boost/random.hpp>

#undef nil
#undef Nil
#define nil NULL
#define Nil NULL

#import "FirstViewController.h"

static int const WEAR_MAX_NUM = 20;
static int const IMG_WIDTH = 80;
static int const IMG_HEIGHT = 87;
static int const X_ORIGIN = 312;
static int const Y_ORIGIN = 174;
static unsigned int const SELECTION_MAX = WEAR_MAX_NUM/2;
static int const CLUSTERING_TIMING = 4;
static int const MAP_WIDTH = 15;
static int const MAP_HEIGHT = 15;
static int const LEARNING_TIME = 350;
static double const INIT_ALPHA = 0.9;
static double const INIT_SIGMA = 0.9;
static int const CLUSTER_NUM = 2;
static double const NEIGHBORHOOD_DISTANCE = 0.75;

@implementation FirstViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
  self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
  if (self) {
    self.title = NSLocalizedString(@"Preference Search System", @"Preference Search System");
  }
  SelectionHistory = [NSMutableArray array];
  GeneObjArr = [NSMutableArray array];
  ImgRectArr = [NSMutableArray array];
  PairArr = [[NSMutableArray alloc] initWithObjects:
             @(-1),@(-1),@(-1),@(-1),@(-1),
             @(-1),@(-1),@(-1),@(-1),@(-1),
             @(-1),@(-1),@(-1),@(-1),@(-1),
             @(-1),@(-1),@(-1),@(-1),@(-1), nil];
  selectionNum = 0;
  GenerationNum = 0;
  
  return self;
}
							
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
  [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
  
  boost::mt19937 gen(static_cast<unsigned long>(std::time(0)));
  boost::uniform_real<> BGColorHueDist(0, 1);
  boost::uniform_real<> BGColorSatDist(0.75, 1);
  boost::uniform_real<> BGColorValDist(0.75, 1);
  boost::uniform_smallint<> NeckDist(0, 14);
  boost::uniform_smallint<> SleeveDist(0, 1);
  boost::uniform_smallint<> PatternDist(0, 7);
  boost::uniform_smallint<> PtrColorDist(0, 8);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > BGColorHueRand(gen, BGColorHueDist);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > BGColorSatRand(gen, BGColorSatDist);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > BGColorValRand(gen, BGColorValDist);
  boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > NeckRand(gen, NeckDist);
  boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > SleeveRand(gen, SleeveDist);
  boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > PatternRand(gen, PatternDist);
  boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > PtrColorRand(gen, PtrColorDist);
  
  [NextButton addTarget:self action:@selector(pushNextButton)forControlEvents:UIControlEventTouchDown];
  [self.view addSubview:NextButton];
  
  //Tシャツイメージの初期化
  for (int i = 0; i < WEAR_MAX_NUM; ++i) {
    //座標とサイズ（定数），イメージの属性（乱数）の初期化
    NSMutableArray *GeneType = NULL;
    CGRect imgRect = CGRectMake(X_ORIGIN+IMG_WIDTH*(i%5),
                                Y_ORIGIN+IMG_HEIGHT*(i/5),
                                IMG_WIDTH,
                                IMG_HEIGHT);
    NSNumber *Hue = @(BGColorHueRand());
    NSNumber *Sat = @(BGColorSatRand());
    NSNumber *Val = @(BGColorValRand());
    NSNumber *NeckType = @(NeckRand());
    NSNumber *PatternNum = @(PatternRand());
    NSNumber *PtrColor = @(PtrColorRand());
    NSNumber *SleeveNum = @(SleeveRand());
    
    //襟の形などはすべて「数字.拡張子」という表記
    NSString *NeckTypeStr = [[NSString alloc] initWithFormat:@"%d.png", [NeckType unsignedIntegerValue]];
    NSString *PtrColorStr = [[NSString alloc] initWithFormat:@"%d.png", [PtrColor unsignedIntegerValue]];
    NSString *SleeveTypeStr = [[NSString alloc] initWithString:([SleeveNum integerValue]==0)?@"long/":@"short/"];
    NSString *PatternStr = NULL;
    
    //配列によるイメージオブジェクトの管理
    [ImgRectArr addObject:[NSValue valueWithCGRect:imgRect]];

    switch([PatternNum integerValue]){
      case 0:PatternStr = @"blank";break;
      case 1:PatternStr = @"item/border/";break;
      case 2:PatternStr = @"item/bubble/";break;
      case 3:PatternStr = @"item/dot/";break;
      case 4:PatternStr = @"item/flower/";break;
      case 5:PatternStr = @"item/line/";break;
      case 6:PatternStr = @"item/rainbow/";break;
      case 7:PatternStr = @"item/star/";break;
      default:PatternStr = @"blank";NSLog(@"PatternRand Error");break;
    }
    
    switch(i){
      case  0:
        BaseColor00.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear00 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern00 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor00.frame = imgRect;
        Wear00.frame = imgRect;
        Pattern00.frame = imgRect;
        
        BaseColor00.userInteractionEnabled = YES;
        Wear00.userInteractionEnabled = YES;
        Pattern00.userInteractionEnabled = YES;

        [self.view addSubview:BaseColor00];
        [self.view addSubview:Pattern00];
        [self.view addSubview:Wear00];

        break;
      case  1:
        BaseColor01.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear01 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern01 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor01.frame = imgRect;
        Wear01.frame = imgRect;
        Pattern01.frame = imgRect;
        
        BaseColor01.userInteractionEnabled = YES;
        Wear01.userInteractionEnabled = YES;
        Pattern01.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor01];
        [self.view addSubview:Pattern01];
        [self.view addSubview:Wear01];
        
        break;
      case  2:
        BaseColor02.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear02 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern02 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor02.frame = imgRect;
        Wear02.frame = imgRect;
        Pattern02.frame = imgRect;
        
        BaseColor02.userInteractionEnabled = YES;
        Wear02.userInteractionEnabled = YES;
        Pattern02.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor02];
        [self.view addSubview:Pattern02];
        [self.view addSubview:Wear02];
        
        break;
      case  3:
        BaseColor03.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear03 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern03 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor03.frame = imgRect;
        Wear03.frame = imgRect;
        Pattern03.frame = imgRect;
        
        BaseColor03.userInteractionEnabled = YES;
        Wear03.userInteractionEnabled = YES;
        Pattern03.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor03];
        [self.view addSubview:Pattern03];
        [self.view addSubview:Wear03];
        
        break;
      case  4:
        BaseColor04.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear04 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern04 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor04.frame = imgRect;
        Wear04.frame = imgRect;
        Pattern04.frame = imgRect;
        
        BaseColor04.userInteractionEnabled = YES;
        Wear04.userInteractionEnabled = YES;
        Pattern04.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor04];
        [self.view addSubview:Pattern04];
        [self.view addSubview:Wear04];
        
        break;
      case  5:
        BaseColor10.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear10 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern10 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor10.frame = imgRect;
        Wear10.frame = imgRect;
        Pattern10.frame = imgRect;
        
        BaseColor10.userInteractionEnabled = YES;
        Wear10.userInteractionEnabled = YES;
        Pattern10.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor10];
        [self.view addSubview:Pattern10];
        [self.view addSubview:Wear10];
        
        break;
      case  6:
        BaseColor11.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear11 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern11 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor11.frame = imgRect;
        Wear11.frame = imgRect;
        Pattern11.frame = imgRect;
        
        BaseColor11.userInteractionEnabled = YES;
        Wear11.userInteractionEnabled = YES;
        Pattern11.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor11];
        [self.view addSubview:Pattern11];
        [self.view addSubview:Wear11];
        
        break;
      case  7:
        BaseColor12.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear12 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern12 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor12.frame = imgRect;
        Wear12.frame = imgRect;
        Pattern12.frame = imgRect;
        
        BaseColor12.userInteractionEnabled = YES;
        Wear12.userInteractionEnabled = YES;
        Pattern12.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor12];
        [self.view addSubview:Pattern12];
        [self.view addSubview:Wear12];
        
        break;
      case  8:
        BaseColor13.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear13 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern13 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor13.frame = imgRect;
        Wear13.frame = imgRect;
        Pattern13.frame = imgRect;
        
        BaseColor13.userInteractionEnabled = YES;
        Wear13.userInteractionEnabled = YES;
        Pattern13.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor13];
        [self.view addSubview:Pattern13];
        [self.view addSubview:Wear13];
        
        break;
      case  9:
        BaseColor14.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear14 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern14 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor14.frame = imgRect;
        Wear14.frame = imgRect;
        Pattern14.frame = imgRect;
        
        BaseColor14.userInteractionEnabled = YES;
        Wear14.userInteractionEnabled = YES;
        Pattern14.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor14];
        [self.view addSubview:Pattern14];
        [self.view addSubview:Wear14];
        
        break;
      case 10:
        BaseColor20.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear20 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern20 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor20.frame = imgRect;
        Wear20.frame = imgRect;
        Pattern20.frame = imgRect;
        
        BaseColor20.userInteractionEnabled = YES;
        Wear20.userInteractionEnabled = YES;
        Pattern20.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor20];
        [self.view addSubview:Pattern20];
        [self.view addSubview:Wear20];
        
        break;
      case 11:
        BaseColor21.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear21 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern21 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor21.frame = imgRect;
        Wear21.frame = imgRect;
        Pattern21.frame = imgRect;
        
        BaseColor21.userInteractionEnabled = YES;
        Wear21.userInteractionEnabled = YES;
        Pattern21.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor21];
        [self.view addSubview:Pattern21];
        [self.view addSubview:Wear21];
        
        break;
      case 12:
        BaseColor22.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear22 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern22 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor22.frame = imgRect;
        Wear22.frame = imgRect;
        Pattern22.frame = imgRect;
        
        BaseColor22.userInteractionEnabled = YES;
        Wear22.userInteractionEnabled = YES;
        Pattern22.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor22];
        [self.view addSubview:Pattern22];
        [self.view addSubview:Wear22];
        
        break;
      case 13:
        BaseColor23.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear23 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern23 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor23.frame = imgRect;
        Wear23.frame = imgRect;
        Pattern23.frame = imgRect;
        
        BaseColor23.userInteractionEnabled = YES;
        Wear23.userInteractionEnabled = YES;
        Pattern23.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor23];
        [self.view addSubview:Pattern23];
        [self.view addSubview:Wear23];
        
        break;
      case 14:
        BaseColor24.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear24 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern24 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor24.frame = imgRect;
        Wear24.frame = imgRect;
        Pattern24.frame = imgRect;
        
        BaseColor24.userInteractionEnabled = YES;
        Wear24.userInteractionEnabled = YES;
        Pattern24.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor24];
        [self.view addSubview:Pattern24];
        [self.view addSubview:Wear24];
        
        break;
      case 15:
        BaseColor30.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear30 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern30 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor30.frame = imgRect;
        Wear30.frame = imgRect;
        Pattern30.frame = imgRect;
        
        BaseColor30.userInteractionEnabled = YES;
        Wear30.userInteractionEnabled = YES;
        Pattern30.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor30];
        [self.view addSubview:Pattern30];
        [self.view addSubview:Wear30];
        
        break;
      case 16:
        BaseColor31.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear31 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern31 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor31.frame = imgRect;
        Wear31.frame = imgRect;
        Pattern31.frame = imgRect;
        
        BaseColor31.userInteractionEnabled = YES;
        Wear31.userInteractionEnabled = YES;
        Pattern31.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor31];
        [self.view addSubview:Pattern31];
        [self.view addSubview:Wear31];
        
        break;
      case 17:
        BaseColor32.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear32 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern32 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor32.frame = imgRect;
        Wear32.frame = imgRect;
        Pattern32.frame = imgRect;
        
        BaseColor32.userInteractionEnabled = YES;
        Wear32.userInteractionEnabled = YES;
        Pattern32.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor32];
        [self.view addSubview:Pattern32];
        [self.view addSubview:Wear32];
        
        break;
      case 18:
        BaseColor33.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear33 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern33 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor33.frame = imgRect;
        Wear33.frame = imgRect;
        Pattern33.frame = imgRect;
        
        BaseColor33.userInteractionEnabled = YES;
        Wear33.userInteractionEnabled = YES;
        Pattern33.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor33];
        [self.view addSubview:Pattern33];
        [self.view addSubview:Wear33];
        
        break;
      case 19:
        BaseColor34.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
        Wear34 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
        if(![PatternStr isEqual: @"blank"]){
          Pattern34 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
        }
        
        BaseColor34.frame = imgRect;
        Wear34.frame = imgRect;
        Pattern34.frame = imgRect;
        
        BaseColor34.userInteractionEnabled = YES;
        Wear34.userInteractionEnabled = YES;
        Pattern34.userInteractionEnabled = YES;
        
        [self.view addSubview:BaseColor34];
        [self.view addSubview:Pattern34];
        [self.view addSubview:Wear34];

        break;
      default:NSLog(@"Image Layout Error");break;
    }
    
    //iGAの個体を生成
    GeneType = [NSArray arrayWithObjects:Hue, Sat, Val, NeckType, PatternNum, PtrColor, SleeveNum, nil];
    
    IgaGene *Individual = [[IgaGene alloc] initWithGeneType:GeneType];
    [Individual setFrame:imgRect];
    [Individual setNumber:(unsigned long)i];
    
    [GeneObjArr addObject:Individual];
  }
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
	[super viewWillDisappear:animated];
}

- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
  return YES;
}
#pragma mark - Button
- (IBAction)pushNextButton{
  NSMutableArray *NextGeneArr = [NSMutableArray array];
  
  if(GenerationNum < CLUSTERING_TIMING){
    if (selectionNum < SELECTION_MAX) {
      boost::mt19937 gen(static_cast<unsigned long>(std::time(0)));
      boost::uniform_smallint<> dist(0, ([PairArr count] - 1));
      boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > rand(gen, dist);
      
      while(selectionNum < SELECTION_MAX){
        NSInteger firstTarget = rand();
        
        if([[PairArr objectAtIndex:firstTarget] integerValue] < 0){
          while(TRUE){
            NSInteger secondTarget = rand();
            
            if(firstTarget != secondTarget && [[PairArr objectAtIndex:secondTarget] integerValue] < 0){
              [PairArr replaceObjectAtIndex:firstTarget withObject:[NSNumber numberWithInt:secondTarget]];
              [PairArr replaceObjectAtIndex:secondTarget withObject:[NSNumber numberWithInt:firstTarget]];
              
              selectionNum += 2;
              break;
            }else{
              continue;
            }
          }
        }else{
          continue;
        }
      }
    }
  
    for(int i = 0, iMax = [PairArr count], cnt = 0; i < iMax; ++i){
      if(cnt >= 5 ){
        break;
      }
      if([[PairArr objectAtIndex:i] integerValue] > -1){
        IgaGene *Partner = [GeneObjArr objectAtIndex:[[PairArr objectAtIndex:i] integerValue]];
        NSArray *OffspringsList = [[GeneObjArr objectAtIndex:i] crossover:Partner];

        [NextGeneArr addObject:[GeneObjArr objectAtIndex:i]];
        [NextGeneArr addObject:Partner];
        [SelectionHistory addObject:[GeneObjArr objectAtIndex:i]];
        [SelectionHistory addObject:Partner];
        
        if([[OffspringsList objectAtIndex:0] mutation]){
          NSLog(@"Mutation Occured.[index 0]");
        }
        if([[OffspringsList objectAtIndex:1] mutation]){
          NSLog(@"Mutation Occured.[index 1]");
        }
        
        [NextGeneArr addObject:[OffspringsList objectAtIndex:0]];
        [NextGeneArr addObject:[OffspringsList objectAtIndex:1]];
      
        [PairArr replaceObjectAtIndex:[[PairArr objectAtIndex:i] integerValue] withObject:@(-1)];
        [PairArr replaceObjectAtIndex:i withObject:@(-1)];
        ++cnt;
      }
    }
    GenerationNum += 1;
    [self startupForNextGeneration:(NSMutableArray *)NextGeneArr];
  }else{
    GenerationNum = 0;
    [ClusteringIndicator startAnimating];
    [[NSRunLoop currentRunLoop]runUntilDate:[NSDate dateWithTimeIntervalSinceNow:0.0]];
    NSLog(@"%d", ClusteringIndicator.isAnimating);
    [self ClusteringAndInitializing];
  }
}
#pragma mark - Touches Event
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
  NSLog(@"hogehoge");
  if([self touchesDetector:touches withEvent:event]){
    SelectImgNum = [self selectImageDetector:event];
    
    if(SelectImgNum < 0 || [[PairArr objectAtIndex:(NSUInteger)SelectImgNum] integerValue] >= 0){
      SelectImgNum = -1;
    }
  }
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
  if([self touchesDetector:touches withEvent:event]){
    if(SelectImgNum >= 0){
      [self moveUIImageView:event];
    }
  }
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
  if([self touchesDetector:touches withEvent:event]){
    if(SelectImgNum >= 0){
      [self moveUIImageView:event];
      
      if([self checkOverLap:event]){
        long UnderImgNum = [self selectImageDetector:event];
        NSNumber *UnderImgNSNum = [NSNumber numberWithLong:UnderImgNum];
        
        if([[PairArr objectAtIndex:UnderImgNum] integerValue] < 0 && (selectionNum < SELECTION_MAX)){
          [PairArr replaceObjectAtIndex:(NSUInteger)UnderImgNum withObject:[NSNumber numberWithLong:SelectImgNum]];
          [PairArr replaceObjectAtIndex:(NSUInteger)SelectImgNum withObject:UnderImgNSNum];
          selectionNum += 2;
          
        }else{
          [self resetPosition:SelectImgNum];
        }
      }else{
        [self resetPosition:SelectImgNum];
      }
    }
  }
  SelectImgNum = -1;
}
#pragma mark - Other Method
- (BOOL)touchesDetector:(NSSet *)touches withEvent:(UIEvent *)event{
  if([[event allTouches] count] == 1){
    return YES;
  }else{
    return NO;
  }
}

- (long)selectImageDetector:(UIEvent *)event{
  NSInteger touchX = 0;
  NSInteger touchY = 0;
  [self touchCoord:[event allTouches] touchX:&touchX touchY:&touchY];
  
  for(NSValue *imgRectNSVal in ImgRectArr){
    CGRect imgRect = [imgRectNSVal CGRectValue];

    if((imgRect.origin.x <= touchX) &&
       ((static_cast<float>(imgRect.origin.x)+imgRect.size.width) >= static_cast<float>(touchX)) &&
       (imgRect.origin.y <= touchY) &&
       ((static_cast<float>(imgRect.origin.y)+imgRect.size.height) >= static_cast<float>(touchY))){

      return static_cast<unsigned long>(((static_cast<int>(imgRect.origin.y) - Y_ORIGIN) / IMG_HEIGHT)*5 +
              ((static_cast<int>(imgRect.origin.x) - X_ORIGIN) / IMG_WIDTH));
    }
  }
  
  return -1;
}
- (void)touchCoord:(NSSet *)touches touchX:(NSInteger *)touchX touchY:(NSInteger *)touchY{
  CGPoint touchPoint = CGPointZero;
  for(UITouch *touch in touches){
    touchPoint = [touch locationInView:[self view]];
  }
  *touchX = touchPoint.x;
  *touchY = touchPoint.y;
}
- (void)moveUIImageView:(UIEvent *)event{
  NSInteger touchX = 0;
  NSInteger touchY = 0;
  [self touchCoord:[event allTouches] touchX:&touchX touchY:&touchY];
  
  CGRect imgRect = CGRectMake(touchX,
                              touchY,
                              IMG_WIDTH,
                              IMG_HEIGHT);
  switch(SelectImgNum){
    case 0:
      BaseColor00.frame = imgRect;
      Wear00.frame = imgRect;
      Pattern00.frame = imgRect;
      
      [self.view addSubview:BaseColor00];
      [self.view addSubview:Pattern00];
      [self.view addSubview:Wear00];
      break;
    case 1:
      BaseColor01.frame = imgRect;
      Wear01.frame = imgRect;
      Pattern01.frame = imgRect;
      
      [self.view addSubview:BaseColor01];
      [self.view addSubview:Pattern01];
      [self.view addSubview:Wear01];
      break;
    case 2:
      BaseColor02.frame = imgRect;
      Wear02.frame = imgRect;
      Pattern02.frame = imgRect;
      
      [self.view addSubview:BaseColor02];
      [self.view addSubview:Pattern02];
      [self.view addSubview:Wear02];
      break;
    case 3:
      BaseColor03.frame = imgRect;
      Wear03.frame = imgRect;
      Pattern03.frame = imgRect;
      
      [self.view addSubview:BaseColor03];
      [self.view addSubview:Pattern03];
      [self.view addSubview:Wear03];
      break;
    case 4:
      BaseColor04.frame = imgRect;
      Wear04.frame = imgRect;
      Pattern04.frame = imgRect;
      
      [self.view addSubview:BaseColor04];
      [self.view addSubview:Pattern04];
      [self.view addSubview:Wear04];
      break;
    case 5:
      BaseColor10.frame = imgRect;
      Wear10.frame = imgRect;
      Pattern10.frame = imgRect;
      
      [self.view addSubview:BaseColor10];
      [self.view addSubview:Pattern10];
      [self.view addSubview:Wear10];
      break;
    case 6:
      BaseColor11.frame = imgRect;
      Wear11.frame = imgRect;
      Pattern11.frame = imgRect;
      
      [self.view addSubview:BaseColor11];
      [self.view addSubview:Pattern11];
      [self.view addSubview:Wear11];
      break;
    case 7:
      BaseColor12.frame = imgRect;
      Wear12.frame = imgRect;
      Pattern12.frame = imgRect;
      
      [self.view addSubview:BaseColor12];
      [self.view addSubview:Pattern12];
      [self.view addSubview:Wear12];
      break;
    case 8:
      BaseColor13.frame = imgRect;
      Wear13.frame = imgRect;
      Pattern13.frame = imgRect;
      
      [self.view addSubview:BaseColor13];
      [self.view addSubview:Pattern13];
      [self.view addSubview:Wear13];
      break;
    case 9:
      BaseColor14.frame = imgRect;
      Wear14.frame = imgRect;
      Pattern14.frame = imgRect;
      
      [self.view addSubview:BaseColor14];
      [self.view addSubview:Pattern14];
      [self.view addSubview:Wear14];
      break;
    case 10:
      BaseColor20.frame = imgRect;
      Wear20.frame = imgRect;
      Pattern20.frame = imgRect;
      
      [self.view addSubview:BaseColor20];
      [self.view addSubview:Pattern20];
      [self.view addSubview:Wear20];
      break;
    case 11:
      BaseColor21.frame = imgRect;
      Wear21.frame = imgRect;
      Pattern21.frame = imgRect;
      
      [self.view addSubview:BaseColor21];
      [self.view addSubview:Pattern21];
      [self.view addSubview:Wear21];
      break;
    case 12:
      BaseColor22.frame = imgRect;
      Wear22.frame = imgRect;
      Pattern22.frame = imgRect;
      
      [self.view addSubview:BaseColor22];
      [self.view addSubview:Pattern22];
      [self.view addSubview:Wear22];
      break;
    case 13:
      BaseColor23.frame = imgRect;
      Wear23.frame = imgRect;
      Pattern23.frame = imgRect;
      
      [self.view addSubview:BaseColor23];
      [self.view addSubview:Pattern23];
      [self.view addSubview:Wear23];
      break;
    case 14:
      BaseColor24.frame = imgRect;
      Wear24.frame = imgRect;
      Pattern24.frame = imgRect;
      
      [self.view addSubview:BaseColor24];
      [self.view addSubview:Pattern24];
      [self.view addSubview:Wear24];
      break;
    case 15:
      BaseColor30.frame = imgRect;
      Wear30.frame = imgRect;
      Pattern30.frame = imgRect;
      
      [self.view addSubview:BaseColor30];
      [self.view addSubview:Pattern30];
      [self.view addSubview:Wear30];
      break;
    case 16:
      BaseColor31.frame = imgRect;
      Wear31.frame = imgRect;
      Pattern31.frame = imgRect;
      
      [self.view addSubview:BaseColor31];
      [self.view addSubview:Pattern31];
      [self.view addSubview:Wear31];
      break;
    case 17:
      BaseColor32.frame = imgRect;
      Wear32.frame = imgRect;
      Pattern32.frame = imgRect;
      
      [self.view addSubview:BaseColor32];
      [self.view addSubview:Pattern32];
      [self.view addSubview:Wear32];
      break;
    case 18:
      BaseColor33.frame = imgRect;
      Wear33.frame = imgRect;
      Pattern33.frame = imgRect;
      
      [self.view addSubview:BaseColor33];
      [self.view addSubview:Pattern33];
      [self.view addSubview:Wear33];
      break;
    case 19:
      BaseColor34.frame = imgRect;
      Wear34.frame = imgRect;
      Pattern34.frame = imgRect;
      
      [self.view addSubview:BaseColor34];
      [self.view addSubview:Pattern34];
      [self.view addSubview:Wear34];
      break;
    default:
      break;
  }
}
- (BOOL)checkOverLap:(UIEvent *)event{
  long OverLapImgNum = [self selectImageDetector:event];
  
  if((SelectImgNum != OverLapImgNum) && (OverLapImgNum >= 0)){
    return YES;
  }else{
    return NO;
  }
}
- (void)resetPosition:(NSUInteger) num{
  NSNumber *Hue = [[[GeneObjArr objectAtIndex:num] valueForKey:@"GeneType"] objectAtIndex:0];
  NSNumber *Sat = [[[GeneObjArr objectAtIndex:num] valueForKey:@"GeneType"] objectAtIndex:1];
  NSNumber *Val = [[[GeneObjArr objectAtIndex:num] valueForKey:@"GeneType"] objectAtIndex:2];
  NSNumber *NeckType = [[[GeneObjArr objectAtIndex:num] valueForKey:@"GeneType"] objectAtIndex:3];
  NSNumber *PatternNum = [[[GeneObjArr objectAtIndex:num] valueForKey:@"GeneType"] objectAtIndex:4];
  NSNumber *PtrColor = [[[GeneObjArr objectAtIndex:num] valueForKey:@"GeneType"] objectAtIndex:5];
  NSNumber *SleeveNum = [[[GeneObjArr objectAtIndex:num] valueForKey:@"GeneType"] objectAtIndex:6];
  
  NSString *NeckTypeStr = [[NSString alloc] initWithFormat:@"%d.png", [NeckType unsignedIntegerValue]];
  NSString *PtrColorStr = [[NSString alloc] initWithFormat:@"%d.png", [PtrColor unsignedIntegerValue]];
  NSString *SleeveTypeStr = [[NSString alloc] initWithString:([SleeveNum integerValue]==0)?@"long/":@"short/"];
  NSString *PatternStr = NULL;
  
  switch([PatternNum integerValue]){
    case 0:PatternStr = @"blank";break;
    case 1:PatternStr = @"item/border/";break;
    case 2:PatternStr = @"item/bubble/";break;
    case 3:PatternStr = @"item/dot/";break;
    case 4:PatternStr = @"item/flower/";break;
    case 5:PatternStr = @"item/line/";break;
    case 6:PatternStr = @"item/rainbow/";break;
    case 7:PatternStr = @"item/star/";break;
    default:PatternStr = @"blank";NSLog(@"PatternRand Error");break;
  }
  CGRect numRect = [[ImgRectArr objectAtIndex:(NSUInteger)num] CGRectValue];
  
  switch(num){
    case 0:
      [BaseColor00 removeFromSuperview];
      [Pattern00 removeFromSuperview];
      [Wear00 removeFromSuperview];
      
      BaseColor00.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear00 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern00 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor00.frame = numRect;
      Wear00.frame = numRect;
      Pattern00.frame = numRect;
      
      [self.view addSubview:BaseColor00];
      [self.view addSubview:Pattern00];
      [self.view addSubview:Wear00];
      break;
    case 1:
      [BaseColor01 removeFromSuperview];
      [Pattern01 removeFromSuperview];
      [Wear01 removeFromSuperview];
      
      BaseColor01.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear01 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern01 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor01.frame = numRect;
      Wear01.frame = numRect;
      Pattern01.frame = numRect;
      
      [self.view addSubview:BaseColor01];
      [self.view addSubview:Pattern01];
      [self.view addSubview:Wear01];
      break;
    case 2:
      [BaseColor02 removeFromSuperview];
      [Pattern02 removeFromSuperview];
      [Wear02 removeFromSuperview];
      
      BaseColor02.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear02 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern02 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor02.frame = numRect;
      Wear02.frame = numRect;
      Pattern02.frame = numRect;
      
      [self.view addSubview:BaseColor02];
      [self.view addSubview:Pattern02];
      [self.view addSubview:Wear02];
      break;
    case 3:
      [BaseColor03 removeFromSuperview];
      [Pattern03 removeFromSuperview];
      [Wear03 removeFromSuperview];
      
      BaseColor03.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear03 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern03 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor03.frame = numRect;
      Wear03.frame = numRect;
      Pattern03.frame = numRect;
      
      [self.view addSubview:BaseColor03];
      [self.view addSubview:Pattern03];
      [self.view addSubview:Wear03];
      break;
    case 4:
      [BaseColor04 removeFromSuperview];
      [Pattern04 removeFromSuperview];
      [Wear04 removeFromSuperview];
      
      BaseColor04.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear04 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern04 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }

      BaseColor04.frame = numRect;
      Wear04.frame = numRect;
      Pattern04.frame = numRect;
      
      [self.view addSubview:BaseColor04];
      [self.view addSubview:Pattern04];
      [self.view addSubview:Wear04];
      break;
    case 5:
      [BaseColor10 removeFromSuperview];
      [Pattern10 removeFromSuperview];
      [Wear10 removeFromSuperview];
      
      BaseColor10.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear10 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern10 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor10.frame = numRect;
      Wear10.frame = numRect;
      Pattern10.frame = numRect;
      
      [self.view addSubview:BaseColor10];
      [self.view addSubview:Pattern10];
      [self.view addSubview:Wear10];
      break;
    case 6:
      [BaseColor11 removeFromSuperview];
      [Pattern11 removeFromSuperview];
      [Wear11 removeFromSuperview];
      
      BaseColor11.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear11 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern11 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor11.frame = numRect;
      Wear11.frame = numRect;
      Pattern11.frame = numRect;
      
      [self.view addSubview:BaseColor11];
      [self.view addSubview:Pattern11];
      [self.view addSubview:Wear11];
      break;
    case 7:
      [BaseColor12 removeFromSuperview];
      [Pattern12 removeFromSuperview];
      [Wear12 removeFromSuperview];
      
      BaseColor12.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear12 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern12 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor12.frame = numRect;
      Wear12.frame = numRect;
      Pattern12.frame = numRect;
      
      [self.view addSubview:BaseColor12];
      [self.view addSubview:Pattern12];
      [self.view addSubview:Wear12];
      break;
    case 8:
      [BaseColor13 removeFromSuperview];
      [Pattern13 removeFromSuperview];
      [Wear13 removeFromSuperview];
      
      BaseColor13.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear13 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern13 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor13.frame = numRect;
      Wear13.frame = numRect;
      Pattern13.frame = numRect;
      
      [self.view addSubview:BaseColor13];
      [self.view addSubview:Pattern13];
      [self.view addSubview:Wear13];
      break;
    case 9:
      [BaseColor14 removeFromSuperview];
      [Pattern14 removeFromSuperview];
      [Wear14 removeFromSuperview];
      
      BaseColor14.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear14 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern14 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor14.frame = numRect;
      Wear14.frame = numRect;
      Pattern14.frame = numRect;
      
      [self.view addSubview:BaseColor14];
      [self.view addSubview:Pattern14];
      [self.view addSubview:Wear14];
      break;
    case 10:
      [BaseColor20 removeFromSuperview];
      [Pattern20 removeFromSuperview];
      [Wear20 removeFromSuperview];
      
      BaseColor20.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear20 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern20 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor20.frame = numRect;
      Wear20.frame = numRect;
      Pattern20.frame = numRect;
      
      [self.view addSubview:BaseColor20];
      [self.view addSubview:Pattern20];
      [self.view addSubview:Wear20];
      break;
    case 11:
      [BaseColor21 removeFromSuperview];
      [Pattern21 removeFromSuperview];
      [Wear21 removeFromSuperview];
    
      
      BaseColor21.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear21 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern21 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor21.frame = numRect;
      Wear21.frame = numRect;
      Pattern21.frame = numRect;
      
      [self.view addSubview:BaseColor21];
      [self.view addSubview:Pattern21];
      [self.view addSubview:Wear21];
      break;
    case 12:
      [BaseColor22 removeFromSuperview];
      [Pattern22 removeFromSuperview];
      [Wear22 removeFromSuperview];
      
      BaseColor22.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear22 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern22 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor22.frame = numRect;
      Wear22.frame = numRect;
      Pattern22.frame = numRect;
      
      [self.view addSubview:BaseColor22];
      [self.view addSubview:Pattern22];
      [self.view addSubview:Wear22];
      break;
    case 13:
      [BaseColor23 removeFromSuperview];
      [Pattern23 removeFromSuperview];
      [Wear23 removeFromSuperview];
      
      BaseColor23.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear23 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern23 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor23.frame = numRect;
      Wear23.frame = numRect;
      Pattern23.frame = numRect;
      
      [self.view addSubview:BaseColor23];
      [self.view addSubview:Pattern23];
      [self.view addSubview:Wear23];
      break;
    case 14:
      [BaseColor24 removeFromSuperview];
      [Pattern24 removeFromSuperview];
      [Wear24 removeFromSuperview];
      
      BaseColor24.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear24 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern24 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor24.frame = numRect;
      Wear24.frame = numRect;
      Pattern24.frame = numRect;
      
      [self.view addSubview:BaseColor24];
      [self.view addSubview:Pattern24];
      [self.view addSubview:Wear24];
      break;
    case 15:
      [BaseColor30 removeFromSuperview];
      [Pattern30 removeFromSuperview];
      [Wear30 removeFromSuperview];
      
      BaseColor30.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear30 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern30 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor30.frame = numRect;
      Wear30.frame = numRect;
      Pattern30.frame = numRect;
      
      [self.view addSubview:BaseColor30];
      [self.view addSubview:Pattern30];
      [self.view addSubview:Wear30];
      break;
    case 16:
      [BaseColor31 removeFromSuperview];
      [Pattern31 removeFromSuperview];
      [Wear31 removeFromSuperview];
      
      BaseColor31.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear31 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern31 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor31.frame = numRect;
      Wear31.frame = numRect;
      Pattern31.frame = numRect;
      
      [self.view addSubview:BaseColor31];
      [self.view addSubview:Pattern31];
      [self.view addSubview:Wear31];
      break;
    case 17:
      [BaseColor32 removeFromSuperview];
      [Pattern32 removeFromSuperview];
      [Wear32 removeFromSuperview];
      
      BaseColor32.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear32 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern32 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor32.frame = numRect;
      Wear32.frame = numRect;
      Pattern32.frame = numRect;
      
      [self.view addSubview:BaseColor32];
      [self.view addSubview:Pattern32];
      [self.view addSubview:Wear32];
      break;
    case 18:
      [BaseColor33 removeFromSuperview];
      [Pattern33 removeFromSuperview];
      [Wear33 removeFromSuperview];
      
      BaseColor33.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear33 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern33 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor33.frame = numRect;
      Wear33.frame = numRect;
      Pattern33.frame = numRect;
      
      [self.view addSubview:BaseColor33];
      [self.view addSubview:Pattern33];
      [self.view addSubview:Wear33];
      break;
    case 19:
      [BaseColor34 removeFromSuperview];
      [Pattern34 removeFromSuperview];
      [Wear34 removeFromSuperview];
      
      BaseColor34.backgroundColor = [UIColor colorWithHue:(CGFloat)[Hue floatValue] saturation:(CGFloat)[Sat floatValue] brightness:(CGFloat)[Val floatValue] alpha:1.0];
      Wear34 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[SleeveTypeStr stringByAppendingString:NeckTypeStr]]];
      if(![PatternStr isEqual: @"blank"]){
        Pattern34 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[PatternStr stringByAppendingString:PtrColorStr]]];
      }
      
      BaseColor34.frame = numRect;
      Wear34.frame = numRect;
      Pattern34.frame = numRect;
      
      [self.view addSubview:BaseColor34];
      [self.view addSubview:Pattern34];
      [self.view addSubview:Wear34];
      break;
    default:
      break;
  }
}
- (void)ClusteringAndInitializing{
  boost::mt19937 gen(static_cast<unsigned long>(std::time(0)));
  boost::uniform_real<> BGColorHueDist(0, 1);
  boost::uniform_real<> BGColorSatDist(0, 1);
  boost::uniform_real<> BGColorValDist(0, 1);
  boost::uniform_smallint<> NeckDist(0, 14);
  boost::uniform_smallint<> SleeveDist(0, 1);
  boost::uniform_smallint<> PatternDist(0, 7);
  boost::uniform_smallint<> PtrColorDist(0, 8);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > BGColorHueRand(gen, BGColorHueDist);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > BGColorSatRand(gen, BGColorSatDist);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > BGColorValRand(gen, BGColorValDist);
  boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > NeckRand(gen, NeckDist);
  boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > SleeveRand(gen, SleeveDist);
  boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > PatternRand(gen, PatternDist);
  boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > PtrColorRand(gen, PtrColorDist);

  NSMutableArray *NextGeneArr = [NSMutableArray array];
  NSMutableArray *GeneTypeaArr = [NSMutableArray array];
  dmat inputMat;
  dmat codebookVectorMat;
  som *GeneSOM = new som(MAP_WIDTH, MAP_HEIGHT, [[[GeneObjArr objectAtIndex:0] valueForKey:@"GeneType"] count]);
  
  for(int i = 0, iMax = [SelectionHistory count]; i < iMax; ++i){
    [GeneTypeaArr addObject:[[SelectionHistory objectAtIndex:i] valueForKey:@"GeneType"]];
  }
  
  inputMat = [NSArrayWrapperForUblasNumericalData transrateDoubleMatrix:GeneTypeaArr];
  GeneSOM->setData(&inputMat);
  GeneSOM->NormLearning(LEARNING_TIME, INIT_ALPHA, INIT_SIGMA, false);
  //GeneSOM->ProdLearning(LEARNING_TIME, INIT_ALPHA, INIT_SIGMA, false);
  codebookVectorMat = GeneSOM->getRefMatrix();
  
  std::vector<dvect> CenterPointCPPVector = kmeans(CLUSTER_NUM, &codebookVectorMat, 2);

  for(int i = 0; i < WEAR_MAX_NUM;){
    boost::numeric::ublas::vector<double> randomVector(7);
    NSMutableArray *GeneType = NULL;
    NSNumber *Hue = @(BGColorHueRand());
    NSNumber *Sat = @(BGColorSatRand());
    NSNumber *Val = @(BGColorValRand());
    NSNumber *NeckType = @(NeckRand());
    NSNumber *PatternNum = @(PatternRand());
    NSNumber *PtrColor = @(PtrColorRand());
    NSNumber *SleeveNum = @(SleeveRand());
    
    randomVector(0) = [Hue doubleValue];
    randomVector(1) = [Sat doubleValue];
    randomVector(2) = [Val doubleValue];
    randomVector(3) = [NeckType doubleValue];
    randomVector(4) = [PatternNum doubleValue];
    randomVector(5) = [PtrColor doubleValue];
    randomVector(6) = [SleeveNum doubleValue];
    
    if(norm_2(randomVector - CenterPointCPPVector.at(0)) < NEIGHBORHOOD_DISTANCE || norm_2(randomVector - CenterPointCPPVector.at(1)) < NEIGHBORHOOD_DISTANCE){
      GeneType = [[NSMutableArray alloc] initWithObjects:Hue, Sat, Val, NeckType, PatternNum, PtrColor, SleeveNum, nil];
      IgaGene *renewalIgaGeneObj = [[IgaGene alloc] initWithGeneType:GeneType];
      [NextGeneArr addObject:renewalIgaGeneObj];
      ++i;
    }else{
      continue;
    }
  }
  
  SelectionHistory = [NSMutableArray array];
  [self startupForNextGeneration:NextGeneArr];
}

- (void)startupForNextGeneration:(NSMutableArray *)NextGeneArr{
  int cnt = 0;
  
  GeneObjArr = NextGeneArr;
  
  while(cnt < [GeneObjArr count]){
    [self resetPosition:cnt];
    ++cnt;
  }
  
  PairArr = [[NSMutableArray alloc] initWithObjects:
             @(-1),@(-1),@(-1),@(-1),@(-1),
             @(-1),@(-1),@(-1),@(-1),@(-1),
             @(-1),@(-1),@(-1),@(-1),@(-1),
             @(-1),@(-1),@(-1),@(-1),@(-1), nil];
  
  selectionNum = 0;
  [ClusteringIndicator stopAnimating];
}
@end
