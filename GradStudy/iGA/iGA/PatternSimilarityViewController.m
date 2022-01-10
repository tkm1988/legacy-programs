//
//  PatternSimilarityViewController.m
//  iGA
//
//  Created by 片ノ坂 卓磨 on 13/01/24.
//  Copyright (c) 2013年 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//

#import "FirstViewController.h"
#import "PatternSimilarityViewController.h"

static int const X_ORIGIN = 0;
static int const Y_ORIGIN = 39;
static int const OFFSET = 10;
static int const X_CENTER = 512;
static int const Y_CENTER = 384;
static int const IMG_WIDTH = 80;
static int const IMG_HEIGHT = 87;
static int const WEAR_MAX_NUM = 8;

@implementation PatternSimilarityViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
  UIColor *BackgroundColor = [UIColor colorWithHue:0.0 saturation:0.0 brightness:100.0 alpha:1.0];
  UIImageView *ShapeImage = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"short/0.png"]];
  
  //背景画像の挿入（マルチタッチはオフ）
  BackgroundImage = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"background.png"]];
  BackgroundImage.frame = CGRectMake(0,
                                     0,
                                     1024,
                                     768);
  BackgroundImage.userInteractionEnabled = NO;
  [self.view addSubview:BackgroundImage];
  
  //各画像の初期化
  for (int i = 0; i < WEAR_MAX_NUM; ++i) {
    CGRect imgRect;
    
    if (i < 7) {
      imgRect = CGRectMake(X_ORIGIN+(OFFSET+IMG_WIDTH)*(i%4),
                           Y_ORIGIN+(OFFSET+IMG_HEIGHT)*(i/4),
                           IMG_WIDTH,
                           IMG_HEIGHT);
    }else{
      imgRect = CGRectMake(X_CENTER,
                           Y_CENTER,
                           IMG_WIDTH,
                           IMG_WIDTH);
    }

    switch (i) {
      case 0:
        Border = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"item/border/0.png"]];
        Shape00 = ShapeImage;
        Color00.backgroundColor = BackgroundColor;
        
        Color00.frame = imgRect;
        Border.frame = imgRect;
        Shape00.frame = imgRect;
        
        Color00.userInteractionEnabled = YES;
        Border.userInteractionEnabled = YES;
        Shape00.userInteractionEnabled = YES;
        
        [self.view addSubview:Color00];
        [self.view addSubview:Border];
        [self.view addSubview:Shape00];
        break;
      case 1:
        Bubble = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"item/bubble/0.png"]];
        Shape01 = ShapeImage;
        Color01.backgroundColor = BackgroundColor;
        
        Color01.frame = imgRect;
        Bubble.frame = imgRect;
        Shape01.frame = imgRect;
        
        Color01.userInteractionEnabled = YES;
        Bubble.userInteractionEnabled = YES;
        Shape01.userInteractionEnabled = YES;
        
        [self.view addSubview:Color01];
        [self.view addSubview:Bubble];
        [self.view addSubview:Shape01];
        break;
        
      case 2:
        Dot = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"item/dot/0.png"]];
        Shape02 = ShapeImage;
        Color02.backgroundColor = BackgroundColor;
        
        Color02.frame = imgRect;
        Dot.frame = imgRect;
        Shape02.frame = imgRect;
        
        Color02.userInteractionEnabled = YES;
        Dot.userInteractionEnabled = YES;
        Shape02.userInteractionEnabled = YES;
        
        [self.view addSubview:Color02];
        [self.view addSubview:Dot];
        [self.view addSubview:Shape02];
        break;

      case 3:
        Flower = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"item/flower/0.png"]];
        Shape03 = ShapeImage;
        Color03.backgroundColor = BackgroundColor;
        
        Color03.frame = imgRect;
        Flower.frame = imgRect;
        Shape03.frame = imgRect;
        
        Color03.userInteractionEnabled = YES;
        Flower.userInteractionEnabled = YES;
        Shape03.userInteractionEnabled = YES;
      
        [self.view addSubview:Color03];
        [self.view addSubview:Flower];
        [self.view addSubview:Shape03];
        break;

      case 4:
        Line = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"item/line/0.png"]];
        Shape04 = ShapeImage;
        Color04.backgroundColor = BackgroundColor;
        
        Color04.frame = imgRect;
        Line.frame = imgRect;
        Shape04.frame = imgRect;
        
        Color04.userInteractionEnabled = YES;
        Line.userInteractionEnabled = YES;
        Shape04.userInteractionEnabled = YES;
        
        [self.view addSubview:Color04];
        [self.view addSubview:Line];
        [self.view addSubview:Shape04];
        break;

      case 5:
        Rainbow = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"item/rainbow/0.png"]];
        Shape05 = ShapeImage;
        Color05.backgroundColor = BackgroundColor;
        
        Color05.frame = imgRect;
        Rainbow.frame = imgRect;
        Shape05.frame = imgRect;
        
        Color05.userInteractionEnabled = YES;
        Rainbow.userInteractionEnabled = YES;
        Shape05.userInteractionEnabled = YES;
        
        [self.view addSubview:Color05];
        [self.view addSubview:Rainbow];
        [self.view addSubview:Shape05];
        break;

      case 6:
        Star = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"item/star/0.png"]];
        Shape06 = ShapeImage;
        Color06.backgroundColor = BackgroundColor;
        
        Color06.frame = imgRect;
        Star.frame = imgRect;
        Shape06.frame = imgRect;
        
        Color06.userInteractionEnabled = YES;
        Star.userInteractionEnabled = YES;
        Shape06.userInteractionEnabled = YES;
        
        [self.view addSubview:Color06];
        [self.view addSubview:Star];
        [self.view addSubview:Shape06];
        break;
        
      case 7:
        Blank = Nil;
        Shape07 = ShapeImage;
        Color07.backgroundColor = BackgroundColor;
        
        Color07.frame = imgRect;
        Blank.frame = imgRect;
        Shape07.frame = imgRect;
        
        Color07.userInteractionEnabled = YES;
        Blank.userInteractionEnabled = YES;
        Shape07.userInteractionEnabled = YES;
        
        [self.view addSubview:Color07];
        [self.view addSubview:Blank];
        [self.view addSubview:Shape07];
        break;
        
      default:
        NSLog(@"Image Init Error");
        break;
    }
  }
  

  if (![NaviBar initWithFrame:CGRectMake(0, -5, 1024, 44)] ){
    NSLog(@"ERROR : NaviBar init");
  }
  if (![NaviItem initWithTitle:@"Make Map of Similarity between Patterns"]) {
    NSLog(@"ERROR : NaviItem init");
  }
  if (![NextViewButton initWithTitle:@"FIX & GO TO PREF SEARCH" style:UIBarButtonItemStyleDone target:self action:@selector(pushUIBarButton)]) {
    NSLog(@"ERROR : NextViewButton init");
  }
  
  NSArray *naviBarItems =[[NSArray alloc] initWithObjects:NextViewButton, nil];
  //[NaviBar setItems:toolBarItems animated:YES];
  //NaviBar.topItem.title = @"Make Map of Similarity between Patterns";
  NaviBar.topItem.rightBarButtonItems =naviBarItems;
  [self.view addSubview:NaviBar];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)pushUIBarButton:(UIBarButtonItem *)button
{
  FirstViewController *nextViewController = [[FirstViewController alloc] init];
  if (nextViewController) {
    [self.navigationController pushViewController:nextViewController animated:YES];
  }
}
@end
