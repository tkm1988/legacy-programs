//
//  iGANextViewController.m
//  iGANext
//
//  Created by 片ノ坂 卓磨 on 13/01/30.
//  Copyright (c) 2013年 片ノ坂 卓磨. All rights reserved.
//

#import <ctime>
#import "iGANextViewController.h"
#import "sharedData.h"
#import "IgaGene.h"
#import "NSDictionaryWrapperForUblasNumericalData.h"
#import "NSMutableArray+initWithBoostDoubleMatrix.h"
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
#import "som.h"

#undef nil
#undef Nil
#define nil NULL
#define Nil NULL

using namespace std;

static int const X_ORIGIN = 292;
static int const Y_ORIGIN = 174;
static int const OFFSET = 10;
static int const IMG_WIDTH = 80;
static int const IMG_HEIGHT = 87;
static int const WEAR_MAX_NUM = 20;
static int const CLUSTERING_TIMING = 4;
static int const MAP_WIDTH = 15;
static int const MAP_HEIGHT = 15;
static int const LEARNING_TIME = 350;
static double const INIT_ALPHA = 0.9;
static double const INIT_SIGMA = 0.9;
static int const CLUSTER_NUM = 2;
//static double const NEIGHBORHOOD_DISTANCE = 25.0;
static double const NEIGHBORHOOD_DISTANCE = 2.3;

@implementation iGANextViewController
{
  sharedData *sData;
}
- (void)iterationLoad
{
  for (id key in sData.nextIndividualsDict) {
    double hue = [[[sData.nextIndividualsDict objectForKey:key] objectForKey:@"Hue"] doubleValue];
    double sat = [[[sData.nextIndividualsDict objectForKey:key] objectForKey:@"Saturation"] doubleValue];
    double val = [[[sData.nextIndividualsDict objectForKey:key] objectForKey:@"Value"] doubleValue];
    int collarNum = [[[sData.nextIndividualsDict objectForKey:key] objectForKey:@"Collar"] integerValue];
    int sleeveNum = [[[sData.nextIndividualsDict objectForKey:key] objectForKey:@"Sleeve"] integerValue];
    int patternNum = [[[sData.nextIndividualsDict objectForKey:key] objectForKey:@"PatternName"] integerValue];
    int patternColorNum = [[[sData.nextIndividualsDict objectForKey:key] objectForKey:@"PatternColor"] integerValue];
    
    NSMutableDictionary *imageViewAttributeDict = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                   [[NSNumber alloc] initWithDouble:hue], @"Hue",
                                                   [[NSNumber alloc] initWithDouble:val], @"Value",
                                                   [[NSNumber alloc] initWithDouble:sat], @"Saturation",
                                                   [[NSNumber alloc] initWithInt:collarNum], @"Collar",
                                                   [[NSNumber alloc] initWithInt:sleeveNum], @"Sleeve",
                                                   [[NSNumber alloc] initWithInt:patternNum], @"PatternName",
                                                   [[NSNumber alloc] initWithInt:patternColorNum], @"PatternColor",
                                                   nil];
    [imageViewAttributesDict setObject:imageViewAttributeDict forKey:key];
    
    CGRect imgRect = CGRectMake(X_ORIGIN+(OFFSET+IMG_WIDTH)*([key integerValue]%5),
                                Y_ORIGIN+(OFFSET+IMG_HEIGHT)*([key integerValue]/5),
                                IMG_WIDTH,
                                IMG_HEIGHT);
    UIImageView *tshirtColorUIImageView;
    UIImageView *tshirtCollarUIImageView;
    UIImageView *tshirtPatternUIImageView;
    UIPanGestureRecognizer *panGesture1 = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(selectUIImageView:)];
    UIPanGestureRecognizer *panGesture2 = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(selectUIImageView:)];
    UIPanGestureRecognizer *panGesture3 = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(selectUIImageView:)];
    
    if (patternNum < 7) {
      tshirtPatternUIImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[[patternPathPrefixStr stringByAppendingString:[patternNameDict objectForKey:[NSNumber numberWithInt:patternNum]]] stringByAppendingString:[patternFileNameArr objectAtIndex:patternColorNum]]]];
    }else{
      tshirtPatternUIImageView = [[UIImageView alloc] init];
      tshirtPatternUIImageView.backgroundColor = [UIColor colorWithHue:hue saturation:sat brightness:val alpha:1.0];
    }
    
    tshirtColorUIImageView = [[UIImageView alloc] init];
    tshirtColorUIImageView.backgroundColor = [UIColor colorWithHue:hue saturation:sat brightness:val alpha:1.0];
    tshirtCollarUIImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[[sleeveTypeNameArr objectAtIndex:sleeveNum] stringByAppendingString:[collarFileNameArr objectAtIndex: collarNum]]]];
    
    tshirtColorUIImageView.frame = imgRect;
    tshirtPatternUIImageView.frame = imgRect;
    tshirtCollarUIImageView.frame = imgRect;
    
    [tshirtColorUIImageView setTag:[key integerValue]];
    [tshirtPatternUIImageView setTag:[key integerValue]];
    [tshirtCollarUIImageView setTag:[key integerValue]];
    
    [tshirtColorUIImageView addGestureRecognizer:panGesture1];
    [tshirtPatternUIImageView addGestureRecognizer:panGesture2];
    [tshirtCollarUIImageView addGestureRecognizer:panGesture3];
    
    tshirtColorUIImageView.userInteractionEnabled = YES;
    tshirtPatternUIImageView.userInteractionEnabled = YES;
    tshirtCollarUIImageView.userInteractionEnabled = YES;
    
    [self.view addSubview:tshirtColorUIImageView];
    [self.view addSubview:tshirtPatternUIImageView];
    [self.view addSubview:tshirtCollarUIImageView];
    
    [tshirtColorDict setObject:tshirtColorUIImageView forKey:key];
    [tshirtCollarDict setObject:tshirtCollarUIImageView forKey:key];
    [tshirtPatternDict setObject:tshirtPatternUIImageView forKey:key];
  }
}

- (void)viewDidLoad
{
  [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
  
  sData = [sharedData singleton];
  tshirtColorDict = [[NSMutableDictionary alloc] init];
  tshirtCollarDict = [[NSMutableDictionary alloc] init];
  tshirtPatternDict = [[NSMutableDictionary alloc] init];
  overlapedPairDict = [[NSMutableDictionary alloc] init];
  imageViewAttributesDict = [[NSMutableDictionary alloc] init];
  patternPathPrefixStr = @"item/";
  
  sleeveTypeNameArr = [[NSArray alloc] initWithObjects:
                       @"short",
                       @"long", nil];
  
  patternFileNameArr = [[NSArray alloc] initWithObjects:
                        @"/0.png",
                        @"/1.png",
                        @"/2.png",
                        @"/3.png",
                        @"/4.png",
                        @"/5.png",
                        @"/6.png",
                        @"/7.png",
                        @"/8.png", nil];
  
  collarFileNameArr = [[NSArray alloc] initWithObjects:
                       @"/0.png",
                       @"/1.png",
                       @"/2.png",
                       @"/3.png",
                       @"/4.png",
                       @"/5.png",
                       @"/6.png",
                       @"/7.png",
                       @"/8.png",
                       @"/9.png",
                       @"/10.png",
                       @"/11.png",
                       @"/12.png",
                       @"/13.png",
                       @"/14.png", nil];
  
  patternNameDict = [NSDictionary dictionaryWithObjectsAndKeys:
                     @"border", @0,
                     @"bubble", @1,
                     @"dot",    @2,
                     @"flower", @3,
                     @"line",   @4,
                     @"rainbow",@5,
                     @"star",   @6,
                     @"blank",  @7, nil];
  
  if (sData.iterationNum.integerValue > 0) {
    [self iterationLoad];
  }else{
    boost::mt19937 gen(static_cast<unsigned long>(time(0)));
    boost::uniform_real<> BGColorHueDist(0, 1);
    boost::uniform_real<> BGColorSatDist(0.75, 1);
    boost::uniform_real<> BGColorValDist(0.75, 1);
    boost::uniform_smallint<> CollarDist(0, 14);
    boost::uniform_smallint<> SleeveDist(0, 1);
    boost::uniform_smallint<> PatternDist(0, 7);
    boost::uniform_smallint<> PtrColorDist(0, 8);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > BGColorHueRand(gen, BGColorHueDist);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > BGColorSatRand(gen, BGColorSatDist);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > BGColorValRand(gen, BGColorValDist);
    boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > CollarRand(gen, CollarDist);
    boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > SleeveRand(gen, SleeveDist);
    boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > PatternRand(gen, PatternDist);
    boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > PtrColorRand(gen, PtrColorDist);
    
    //各画像の初期化
    for (int i = 0; i < WEAR_MAX_NUM; ++i) {
      double hue = BGColorHueRand();
      double sat = BGColorSatRand();
      double val = BGColorValRand();
      int collarNum = CollarRand();
      int sleeveNum = SleeveRand();
      int patternNum = PatternRand();
      int patternColorNum = PtrColorRand();
      NSMutableDictionary *imageViewAttributeDict = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                     [[NSNumber alloc] initWithDouble:hue], @"Hue",
                                                     [[NSNumber alloc] initWithDouble:val], @"Value",
                                                     [[NSNumber alloc] initWithDouble:sat], @"Saturation",
                                                     [[NSNumber alloc] initWithInt:collarNum], @"Collar",
                                                     [[NSNumber alloc] initWithInt:sleeveNum], @"Sleeve",
                                                     [[NSNumber alloc] initWithInt:patternNum], @"PatternName",
                                                     [[NSNumber alloc] initWithInt:patternColorNum], @"PatternColor",
                                                     nil];
      [imageViewAttributesDict setObject:imageViewAttributeDict forKey:[NSNumber numberWithInt:i]];
      
      CGRect imgRect = CGRectMake(X_ORIGIN+(OFFSET+IMG_WIDTH)*(i%5),
                                  Y_ORIGIN+(OFFSET+IMG_HEIGHT)*(i/5),
                                  IMG_WIDTH,
                                  IMG_HEIGHT);
      UIImageView *tshirtColorUIImageView;
      UIImageView *tshirtCollarUIImageView;
      UIImageView *tshirtPatternUIImageView;
      UIPanGestureRecognizer *panGesture1 = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(selectUIImageView:)];
      UIPanGestureRecognizer *panGesture2 = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(selectUIImageView:)];
      UIPanGestureRecognizer *panGesture3 = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(selectUIImageView:)];
      
      if (patternNum < 7) {
        tshirtPatternUIImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[[patternPathPrefixStr stringByAppendingString:[patternNameDict objectForKey:[NSNumber numberWithInt:patternNum]]] stringByAppendingString:[patternFileNameArr objectAtIndex:patternColorNum]]]];
      }else{
        tshirtPatternUIImageView = [[UIImageView alloc] init];
        tshirtPatternUIImageView.backgroundColor = [UIColor colorWithHue:hue saturation:sat brightness:val alpha:1.0];
      }
      
      tshirtColorUIImageView = [[UIImageView alloc] init];
      tshirtColorUIImageView.backgroundColor = [UIColor colorWithHue:hue saturation:sat brightness:val alpha:1.0];
      tshirtCollarUIImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[[sleeveTypeNameArr objectAtIndex:sleeveNum] stringByAppendingString:[collarFileNameArr objectAtIndex: collarNum]]]];
      
      tshirtColorUIImageView.frame = imgRect;
      tshirtPatternUIImageView.frame = imgRect;
      tshirtCollarUIImageView.frame = imgRect;
      
      [tshirtColorUIImageView setTag:i];
      [tshirtPatternUIImageView setTag:i];
      [tshirtCollarUIImageView setTag:i];
      
      [tshirtColorUIImageView addGestureRecognizer:panGesture1];
      [tshirtPatternUIImageView addGestureRecognizer:panGesture2];
      [tshirtCollarUIImageView addGestureRecognizer:panGesture3];
      
      tshirtColorUIImageView.userInteractionEnabled = YES;
      tshirtPatternUIImageView.userInteractionEnabled = YES;
      tshirtCollarUIImageView.userInteractionEnabled = YES;
      
      [self.view addSubview:tshirtColorUIImageView];
      [self.view addSubview:tshirtPatternUIImageView];
      [self.view addSubview:tshirtCollarUIImageView];
      
      [tshirtColorDict setObject:tshirtColorUIImageView forKey:[NSNumber numberWithInt:i]];
      [tshirtCollarDict setObject:tshirtCollarUIImageView forKey:[NSNumber numberWithInt:i]];
      [tshirtPatternDict setObject:tshirtPatternUIImageView forKey:[NSNumber numberWithInt:i]];
    }
  }
}

- (void)didReceiveMemoryWarning
{
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}

#pragma mark prepareSegue
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
  
  sData.nextIndividualsDict = [NSMutableDictionary dictionary];
  NSMutableArray *offspringsPool = [NSMutableArray array];
  
  //選択されていない分を追加
  if ([overlapedPairDict count] < 5) {
    while (TRUE){
      boost::mt19937 gen(static_cast<unsigned long>(time(0)));
      boost::uniform_smallint<> dst(0, WEAR_MAX_NUM - 1);
      boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > rand(gen, dst);
      int chooseKey = rand();
      int targetKey = rand();
      
      if (([overlapedPairDict objectForKey:[NSNumber numberWithInt:chooseKey]] == NULL) &&
          ![[overlapedPairDict allValues] containsObject:[NSNumber numberWithInt:chooseKey]]){
        if (([overlapedPairDict objectForKey:[NSNumber numberWithInt:targetKey]] == NULL) &&
            ![[overlapedPairDict allValues] containsObject:[NSNumber numberWithInt:targetKey]]){
          [overlapedPairDict setObject:[NSNumber numberWithInteger:targetKey]
                                forKey:[NSNumber numberWithInt:chooseKey]];
        }
      }
      
      if ([overlapedPairDict count] >= 5) {
        break;
      }
    }
  }
  
  //選択されている個体同士で交叉
  for (id key in overlapedPairDict) {
    IgaGene *Individual1 = [[IgaGene alloc] init];
    IgaGene *Individual2 = [[IgaGene alloc] init];
    NSArray *Offsprings = [NSArray array];
    NSMutableDictionary *GeneType1 = [NSMutableDictionary dictionary];
    NSMutableDictionary *GeneType2 = [NSMutableDictionary dictionary];
    NSMutableDictionary *Offspring1 = [NSMutableDictionary dictionary];
    NSMutableDictionary *Offspring2 = [NSMutableDictionary dictionary];
    NSMutableDictionary *imageViewAttributeDict = [imageViewAttributesDict objectForKey:[overlapedPairDict objectForKey:key]];
    NSMutableDictionary *overlapedImageViewAttributeDict = [imageViewAttributesDict objectForKey:key];
    
    [sData.nextIndividualsDict setObject:imageViewAttributeDict forKey:[overlapedPairDict objectForKey:key]];
    [sData.nextIndividualsDict setObject:overlapedImageViewAttributeDict forKey:key];
    
    for (id tmpkey in imageViewAttributeDict) {
      [GeneType1 setObject:[imageViewAttributeDict objectForKey:tmpkey] forKey:tmpkey];
      [GeneType2 setObject:[overlapedImageViewAttributeDict objectForKey:tmpkey] forKey:tmpkey];
    }
    
    [sData.individualsArr addObject:imageViewAttributeDict];
    [sData.individualsArr addObject:overlapedImageViewAttributeDict];
    
    [Individual1 setValue:GeneType1 forKey:@"GeneType"];
    [Individual2 setValue:GeneType2 forKey:@"GeneType"];
    
    Offsprings = [Individual1 crossover:Individual2];
    [Offspring1 setDictionary:[[Offsprings objectAtIndex:0] valueForKey:@"GeneType"]];
    [Offspring2 setDictionary:[[Offsprings objectAtIndex:1] valueForKey:@"GeneType"]];
    
    [offspringsPool addObject:Offspring1];
    [offspringsPool addObject:Offspring2];
  }
  
  //選択されていないオブジェクトを消去
  for (int i = 0; i < WEAR_MAX_NUM; ++i) {
    UIImageView *tmp = [[UIImageView alloc] init];
    NSNumber *key = [[NSNumber alloc] initWithInt:i];
    
    [tmp setTag:-1];
    
    if (([overlapedPairDict objectForKey:[NSNumber numberWithInt:i]] == NULL) &&
        ![[overlapedPairDict allValues] containsObject:[NSNumber numberWithInt:i]]){
      if (([overlapedPairDict objectForKey:[NSNumber numberWithInt:i]] == NULL) &&
          ![[overlapedPairDict allValues] containsObject:[NSNumber numberWithInt:i]]){
        [tshirtPatternDict setObject:tmp forKey:key];
      }
    }
  }
  
  //子個体を消去したところに追加
  if (offspringsPool.count > 0) {
    for (id key in tshirtPatternDict) {
      if ([[tshirtPatternDict objectForKey:key] tag] == -1) {
        [sData.nextIndividualsDict setObject:[offspringsPool objectAtIndex:0] forKey:key];
        [offspringsPool removeObjectAtIndex:0];
        
        if (offspringsPool.count == 0) {
          break;
        }
      }
    }
  }
  
  if ((([sData.iterationNum integerValue] % CLUSTERING_TIMING) == 0) && ([sData.iterationNum integerValue] > 0)){
    //if (TRUE) {
    dmat inputMat;
    dmat codebookVectorMat;
    som *GeneSOM;
    
    NSDate *now = [NSDate date];
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd-HH:mm:ss"];
    NSString *nowString = [formatter stringFromDate:now];
    NSString *homeDir = NSHomeDirectory();
    NSString *docDir = [homeDir stringByAppendingPathComponent:@"Documents"];
    NSString *selectedHistoryFilePath = NULL;
    NSString *similarityMapFilePath = [docDir stringByAppendingPathComponent:[[NSString alloc] initWithFormat:@"%@SimilarityMap.plist", nowString]];
    NSString *SOMMapFilePath = [docDir stringByAppendingPathComponent:[[NSString alloc] initWithFormat:@"%@SOMMap.plist", nowString]];
    NSMutableArray *codebookVectorMatArr = NULL;
    NSMutableDictionary *patternToCoordNSStringDict = [[NSMutableDictionary alloc] init];
    
    boost::mt19937 gen(static_cast<unsigned long>(time(0)));
    boost::uniform_real<> BGColorHueDist(0, 1);
    boost::uniform_real<> BGColorSatDist(0, 1);
    boost::uniform_real<> BGColorValDist(0, 1);
    boost::uniform_smallint<> CollarDist(0, 14);
    boost::uniform_smallint<> SleeveDist(0, 1);
    boost::uniform_smallint<> PatternDist(0, 7);
    boost::uniform_smallint<> PtrColorDist(0, 8);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > BGColorHueRand(gen, BGColorHueDist);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > BGColorSatRand(gen, BGColorSatDist);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > BGColorValRand(gen, BGColorValDist);
    boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > CollarRand(gen, CollarDist);
    boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > SleeveRand(gen, SleeveDist);
    boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > PatternRand(gen, PatternDist);
    boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > PtrColorRand(gen, PtrColorDist);
    
    for (id obj in sData.individualsArr) {
      switch ([[obj objectForKey:@"PatternName"] integerValue]) {
        case 0:
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"border"] CGPointValue].x] forKey:@"x"];
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"border"] CGPointValue].y] forKey:@"y"];
          break;
        case 1:
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"bubble"] CGPointValue].x] forKey:@"x"];
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"bubble"] CGPointValue].y] forKey:@"y"];
          break;
        case 2:
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"dot"] CGPointValue].x] forKey:@"x"];
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"dot"] CGPointValue].y] forKey:@"y"];
          break;
        case 3:
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"flower"] CGPointValue].x] forKey:@"x"];
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"flower"] CGPointValue].y] forKey:@"y"];
          break;
        case 4:
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"line"] CGPointValue].x] forKey:@"x"];
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"line"] CGPointValue].y] forKey:@"y"];
          break;
        case 5:
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"rainbow"] CGPointValue].x] forKey:@"x"];
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"rainbow"] CGPointValue].y] forKey:@"y"];
          break;
        case 6:
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"star"] CGPointValue].x] forKey:@"x"];
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"star"] CGPointValue].y] forKey:@"y"];
          break;
        case 7:
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"blank"] CGPointValue].x] forKey:@"x"];
          [obj setObject:[NSNumber numberWithFloat:[[sData.patternToCoordDict objectForKey:@"blank"] CGPointValue].y] forKey:@"y"];
          break;
        default:
          break;
      }
      [obj removeObjectForKey:@"PatternName"];
    }
    
    GeneSOM = new som(MAP_WIDTH, MAP_HEIGHT, [[sData.individualsArr objectAtIndex:0] count]);
    inputMat = [NSDictionaryWrapperForUblasNumericalData transrateDoubleMatrix:sData.individualsArr];
    GeneSOM->setData(&inputMat);
    GeneSOM->NormLearning(LEARNING_TIME, INIT_ALPHA, INIT_SIGMA, false);
    codebookVectorMat = GeneSOM->getRefMatrix();
    
    std::vector<dvect> CenterPointCPPVector = kmeans(CLUSTER_NUM, &codebookVectorMat, 2);
    for (int i = 0; i < WEAR_MAX_NUM;) {
      boost::numeric::ublas::vector<double> randomVector(8);
      NSInteger patternNum = PatternRand();
      randomVector(0) = BGColorHueRand();
      randomVector(1) = BGColorValRand();
      randomVector(2) = BGColorSatRand();
      randomVector(3) = CollarRand();
      randomVector(4) = SleeveRand();
      randomVector(5) = (double)[[sData.patternToCoordDict objectForKey:[patternNameDict objectForKey:[NSNumber numberWithInteger:patternNum]]] CGPointValue].x;
      randomVector(6) = (double)[[sData.patternToCoordDict objectForKey:[patternNameDict objectForKey:[NSNumber numberWithInteger:patternNum]]] CGPointValue].y;
      randomVector(7) = PtrColorRand();
      
      //NSLog(@"Distance (%lf, %lf)", norm_2(randomVector - CenterPointCPPVector.at(0)), norm_2(randomVector - CenterPointCPPVector.at(1)));
      //NSLog(@"Distance (%lf, %lf)", norm_2(GeneSOM->getCoord(randomVector) - GeneSOM->getCoord(CenterPointCPPVector.at(0))), norm_2(GeneSOM->getCoord(randomVector) - GeneSOM->getCoord(CenterPointCPPVector.at(1))));
      //if(norm_2(randomVector - CenterPointCPPVector.at(0)) < NEIGHBORHOOD_DISTANCE || norm_2(randomVector - CenterPointCPPVector.at(1)) < NEIGHBORHOOD_DISTANCE){
      if(norm_2(GeneSOM->getCoord(randomVector) - GeneSOM->getCoord(CenterPointCPPVector.at(0))) < NEIGHBORHOOD_DISTANCE ||
         norm_2(GeneSOM->getCoord(randomVector) - GeneSOM->getCoord(CenterPointCPPVector.at(1))) < NEIGHBORHOOD_DISTANCE){
        NSMutableDictionary *GeneType = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                         [[NSNumber alloc] initWithDouble:randomVector(0)], @"Hue",
                                         [[NSNumber alloc] initWithDouble:randomVector(1)], @"Value",
                                         [[NSNumber alloc] initWithDouble:randomVector(2)], @"Saturation",
                                         [[NSNumber alloc] initWithInt:(int)randomVector(3)], @"Collar",
                                         [[NSNumber alloc] initWithInt:(int)randomVector(4)], @"Sleeve",
                                         [[NSNumber alloc] initWithInt:(int)patternNum], @"PatternName",
                                         [[NSNumber alloc] initWithInt:(int)randomVector(7)], @"PatternColor",
                                         nil];
        [sData.nextIndividualsDict setObject:GeneType forKey:[NSNumber numberWithInt:i]];
        NSLog(@"Set %d", i);
        ++i;
      }else{
        continue;
      }
    }
    
    codebookVectorMatArr = [[NSMutableArray alloc] initWithBoostArray:&codebookVectorMat];
    for (id Obj in sData.individualsArr) {
      selectedHistoryFilePath = [docDir stringByAppendingPathComponent:[[NSString alloc] initWithFormat:@"%@SelectedHistory%d.plist", nowString, [sData.individualsArr indexOfObject:Obj]]];
      if ([Obj writeToFile:selectedHistoryFilePath atomically:YES]) {
      }
    }

    for (id Key in sData.patternToCoordDict) {
      [patternToCoordNSStringDict setObject:NSStringFromCGPoint([[sData.patternToCoordDict objectForKey:Key] CGPointValue]) forKey:Key];
    }
    if ([patternToCoordNSStringDict writeToFile:similarityMapFilePath atomically:YES]) {
    }

    if ([codebookVectorMatArr writeToFile:SOMMapFilePath atomically:YES]) {
    }
    sData.individualsArr = [[NSMutableArray alloc] init];
  }else{
    
  }
  sData.iterationNum = [NSNumber numberWithInteger:(sData.iterationNum.unsignedIntegerValue)+1];
}
#pragma mark panGesture when touched UIImageView

-(void)selectUIImageView:(UIPanGestureRecognizer *)sender
{
  NSInteger selectedImageViewTag = [[sender view] tag];
  
  if (![[overlapedPairDict allValues] containsObject:[NSNumber numberWithInteger:selectedImageViewTag]]) {
    UIImageView *colorImage = [tshirtColorDict objectForKey:[NSNumber numberWithInteger:selectedImageViewTag]];
    UIImageView *collarImage = [tshirtCollarDict objectForKey:[NSNumber numberWithInteger:selectedImageViewTag]];
    UIImageView *patternImage = [tshirtPatternDict objectForKey:[NSNumber numberWithInteger:selectedImageViewTag]];
    
    CGPoint diffCoord = [sender translationInView:self.view];
    CGPoint movedCoord = CGPointMake([sender view].center.x + diffCoord.x
                                     ,[sender view].center.y + diffCoord.y);
    patternImage.center = movedCoord;
    colorImage.center = movedCoord;
    collarImage.center = movedCoord;
    
    [tshirtPatternDict setObject:patternImage forKey:[NSNumber numberWithInteger:selectedImageViewTag]];
    [tshirtColorDict setObject:colorImage forKey:[NSNumber numberWithInteger:selectedImageViewTag]];
    [tshirtCollarDict setObject:collarImage forKey:[NSNumber numberWithInteger:selectedImageViewTag]];
    
    for (id key in tshirtPatternDict) {
      UIImageView *imageView = [tshirtPatternDict objectForKey:key];
      CGRect selectedViewCGRect = [sender view].frame;
      CGPoint imageCoord = CGPointMake(imageView.frame.origin.x,
                                       imageView.frame.origin.y);
      
      if ([[sender view] tag] != imageView.tag) {
        if (
            (((imageCoord.x <= (selectedViewCGRect.origin.x + IMG_WIDTH))&&((imageCoord.x + IMG_WIDTH) >= (selectedViewCGRect.origin.x + IMG_WIDTH))) &&
             ((imageCoord.y <= (selectedViewCGRect.origin.y + IMG_HEIGHT)) && ((imageCoord.y + IMG_HEIGHT) >= (selectedViewCGRect.origin.y + IMG_HEIGHT)))) ||
            
            (((imageCoord.x <= (selectedViewCGRect.origin.x))&&((imageCoord.x + IMG_WIDTH) >= selectedViewCGRect.origin.x)) &&
             ((imageCoord.y <= (selectedViewCGRect.origin.y + IMG_HEIGHT)) && ((imageCoord.y + IMG_HEIGHT) >= (selectedViewCGRect.origin.y + IMG_HEIGHT)))) ||
            
            (((imageCoord.x <= (selectedViewCGRect.origin.x + IMG_WIDTH))&&((imageCoord.x + IMG_WIDTH) >= (selectedViewCGRect.origin.x + IMG_WIDTH))) &&
             ((imageCoord.y <= (selectedViewCGRect.origin.y)) && ((imageCoord.y + IMG_HEIGHT) >= (selectedViewCGRect.origin.y)))) ||
            
            (((imageCoord.x <= (selectedViewCGRect.origin.x))&&((imageCoord.x + IMG_WIDTH) >= selectedViewCGRect.origin.x)) &&
             ((imageCoord.y <= (selectedViewCGRect.origin.y)) && ((imageCoord.y + IMG_HEIGHT) >= (selectedViewCGRect.origin.y))))
            ){
          if ([overlapedPairDict objectForKey:[NSNumber numberWithInteger:[imageView tag]]] == NULL) {
            [overlapedPairDict setObject:[NSNumber numberWithInteger:[imageView tag]]
                                  forKey:[[NSNumber alloc] initWithInt:selectedImageViewTag]];
          }
        }
      }
    }
    
    [sender setTranslation:CGPointZero inView:self.view];
  }
}
@end
