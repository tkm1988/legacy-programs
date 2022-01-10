//
//  PatternSimilarityViewController.h
//  iGA
//
//  Created by 片ノ坂 卓磨 on 13/01/24.
//  Copyright (c) 2013年 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PatternSimilarityViewController : UIViewController
{
  IBOutlet UIImageView *BackgroundImage;
  
  IBOutlet UIImageView *Border;
  IBOutlet UIImageView *Bubble;
  IBOutlet UIImageView *Dot;
  IBOutlet UIImageView *Flower;
  IBOutlet UIImageView *Line;
  IBOutlet UIImageView *Rainbow;
  IBOutlet UIImageView *Star;
  IBOutlet UIImageView *Blank;

  IBOutlet UIImageView *Shape00;
  IBOutlet UIImageView *Shape01;
  IBOutlet UIImageView *Shape02;
  IBOutlet UIImageView *Shape03;
  IBOutlet UIImageView *Shape04;
  IBOutlet UIImageView *Shape05;
  IBOutlet UIImageView *Shape06;
  IBOutlet UIImageView *Shape07;
  
  IBOutlet UIImageView *Color00;
  IBOutlet UIImageView *Color01;
  IBOutlet UIImageView *Color02;
  IBOutlet UIImageView *Color03;
  IBOutlet UIImageView *Color04;
  IBOutlet UIImageView *Color05;
  IBOutlet UIImageView *Color06;
  IBOutlet UIImageView *Color07;

  IBOutlet UINavigationBar *NaviBar;
  IBOutlet UINavigationItem *NaviItem;
  IBOutlet UIBarButtonItem *NextViewButton;
  
  NSDictionary *PatternNameToCoordDict;
  
}
- (void)pushUIBarButton:(UIBarButtonItem *)button;
@end
