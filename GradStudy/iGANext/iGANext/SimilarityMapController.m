//
//  SimilarityMapController.m
//  iGANext
//
//  Created by 片ノ坂 卓磨 on 13/01/30.
//  Copyright (c) 2013年 片ノ坂 卓磨. All rights reserved.
//

#import "SimilarityMapController.h"
#import "iGANextViewController.h"
#import "sharedData.h"

static int const X_ORIGIN = 0;
static int const Y_ORIGIN = 39;
static int const OFFSET = 10;
static int const X_CENTER = 512;
static int const Y_CENTER = 352;
static int const IMG_WIDTH = 80;
static int const IMG_HEIGHT = 87;
static int const WEAR_MAX_NUM = 8;

@implementation SimilarityMapController{
  sharedData *sData;
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
  self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
  if (self) {

  }
  return self;
}

- (void)viewDidLoad
{
  sData = [sharedData singleton];
  tshirtColorDict = [[NSMutableDictionary alloc] init];
  tshirtCollarDict = [[NSMutableDictionary alloc] init];
  tshirtPatternDict = [[NSMutableDictionary alloc] init];
  
  NSArray *patternPathArr = [[NSArray alloc] initWithObjects:
                             @"item/border/5.png",
                             @"item/bubble/5.png",
                             @"item/dot/5.png",
                             @"item/flower/5.png",
                             @"item/line/5.png",
                             @"item/rainbow/5.png",
                             @"item/star/5.png", nil];
  NSArray *patternNameArr = [[NSArray alloc] initWithObjects:
                             @"border",
                             @"bubble",
                             @"dot",
                             @"flower",
                             @"line",
                             @"rainbow",
                             @"star",
                             @"blank",nil];
  NSString *collarStr = @"short/5.png";
  
  /*  NSArray *collarArr = [[NSArray alloc] initWithObjects:@"short/5.png",
   @"short/5.png",
   @"short/5.png",
   @"short/5.png",
   @"short/5.png",
   @"short/5.png",
   @"short/5.png",nil];
   */
  
  [super viewDidLoad];
	// Do any additional setup after loading the view.
  
  self.navigationController.delegate = self;
  
  //各画像の初期化
  for (int i = 0; i < WEAR_MAX_NUM; ++i) {
    CGRect imgRect;
    UIImageView *tshirtColorUIImageView;
    UIImageView *tshirtCollarUIImageView;
    UIImageView *tshirtPatternUIImageView;
    UIPanGestureRecognizer *panGesture1 = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(selectUIImageView:)];
    UIPanGestureRecognizer *panGesture2 = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(selectUIImageView:)];
    UIPanGestureRecognizer *panGesture3 = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(selectUIImageView:)];
    
    if (i < 7) {
      imgRect = CGRectMake(X_ORIGIN+(OFFSET+IMG_WIDTH)*(i%4),
                           Y_ORIGIN+(OFFSET+IMG_HEIGHT)*(i/4),
                           IMG_WIDTH,
                           IMG_HEIGHT);
      tshirtPatternUIImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[patternPathArr objectAtIndex:i]]];
    }else{
      imgRect = CGRectMake(X_CENTER - IMG_WIDTH * 0.5,
                           Y_CENTER - IMG_HEIGHT * 0.5,
                           IMG_WIDTH,
                           IMG_WIDTH);
      tshirtPatternUIImageView = [[UIImageView alloc] init];
      tshirtPatternUIImageView.backgroundColor = [UIColor colorWithHue:0.0 saturation:0.0 brightness:100.0 alpha:1.0];
    }
    
    tshirtColorUIImageView = [[UIImageView alloc] init];
    tshirtColorUIImageView.backgroundColor = [UIColor colorWithHue:0.0 saturation:0.0 brightness:100.0 alpha:1.0];
    tshirtCollarUIImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:collarStr]];
    
    tshirtColorUIImageView.frame = imgRect;
    tshirtPatternUIImageView.frame = imgRect;
    tshirtCollarUIImageView.frame = imgRect;
    
    [tshirtColorUIImageView setAccessibilityIdentifier:[patternNameArr objectAtIndex:i]];
    [tshirtPatternUIImageView setAccessibilityIdentifier:[patternNameArr objectAtIndex:i]];
    [tshirtCollarUIImageView setAccessibilityIdentifier:[patternNameArr objectAtIndex:i]];
    
    [tshirtColorUIImageView addGestureRecognizer:panGesture1];
    [tshirtPatternUIImageView addGestureRecognizer:panGesture2];
    [tshirtCollarUIImageView addGestureRecognizer:panGesture3];
    
    tshirtColorUIImageView.userInteractionEnabled = YES;
    tshirtPatternUIImageView.userInteractionEnabled = YES;
    tshirtCollarUIImageView.userInteractionEnabled = YES;
    
    [self.view addSubview:tshirtColorUIImageView];
    [self.view addSubview:tshirtPatternUIImageView];
    [self.view addSubview:tshirtCollarUIImageView];
    
    [tshirtColorDict setObject:tshirtColorUIImageView forKey:[patternNameArr objectAtIndex:i]];
    [tshirtCollarDict setObject:tshirtCollarUIImageView forKey:[patternNameArr objectAtIndex:i]];
    [tshirtPatternDict setObject:tshirtPatternUIImageView forKey:[patternNameArr objectAtIndex:i]];
  }
  
}

- (void)didReceiveMemoryWarning
{
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}

#pragma mark UINavigationViewContorllerDeligate
-(void) navigationController:(UINavigationController *)navigationController didShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
}

- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
}
#pragma mark prepareSegue
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
  for (NSString *Key in tshirtPatternDict) {
    [sData.patternToCoordDict setObject:[NSValue valueWithCGPoint:[[tshirtPatternDict objectForKey:Key] center]] forKey:Key];
  }
}
#pragma mark panGesture when touched UIImageView
-(void)selectUIImageView:(UIPanGestureRecognizer *)sender
{
  NSString *selectedImageViewID = [[sender view] accessibilityIdentifier];
  
  UIImageView *colorImage = [tshirtColorDict objectForKey:selectedImageViewID];
  UIImageView *collarImage = [tshirtCollarDict objectForKey:selectedImageViewID];
  UIImageView *patternImage = [tshirtPatternDict objectForKey:selectedImageViewID];
  
  CGPoint diffCoord = [sender translationInView:self.view];
  CGPoint movedCoord = CGPointMake([sender view].center.x + diffCoord.x
                                   ,[sender view].center.y + diffCoord.y);
  patternImage.center = movedCoord;
  colorImage.center = movedCoord;
  collarImage.center = movedCoord;
  
  [tshirtPatternDict setObject:patternImage forKey:selectedImageViewID];
  [tshirtColorDict setObject:colorImage forKey:selectedImageViewID];
  [tshirtCollarDict setObject:collarImage forKey:selectedImageViewID];
  
  [sender setTranslation:CGPointZero inView:self.view];
}
@end
