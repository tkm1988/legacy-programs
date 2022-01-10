//
//  FirstViewController.h
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/07/16.
//  Copyright (c) 2012 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface FirstViewController : UIViewController
{
  IBOutlet UIImageView *Wear00;
  IBOutlet UIImageView *Wear01;
  IBOutlet UIImageView *Wear02;
  IBOutlet UIImageView *Wear03;
  IBOutlet UIImageView *Wear04;
  IBOutlet UIImageView *Wear10;
  IBOutlet UIImageView *Wear11;
  IBOutlet UIImageView *Wear12;
  IBOutlet UIImageView *Wear13;
  IBOutlet UIImageView *Wear14;
  IBOutlet UIImageView *Wear20;
  IBOutlet UIImageView *Wear21;
  IBOutlet UIImageView *Wear22;
  IBOutlet UIImageView *Wear23;
  IBOutlet UIImageView *Wear24;
  IBOutlet UIImageView *Wear30;
  IBOutlet UIImageView *Wear31;
  IBOutlet UIImageView *Wear32;
  IBOutlet UIImageView *Wear33;
  IBOutlet UIImageView *Wear34;
  
  IBOutlet UIImageView *BaseColor00;
  IBOutlet UIImageView *BaseColor01;
  IBOutlet UIImageView *BaseColor02;
  IBOutlet UIImageView *BaseColor03;
  IBOutlet UIImageView *BaseColor04;
  IBOutlet UIImageView *BaseColor10;
  IBOutlet UIImageView *BaseColor11;
  IBOutlet UIImageView *BaseColor12;
  IBOutlet UIImageView *BaseColor13;
  IBOutlet UIImageView *BaseColor14;
  IBOutlet UIImageView *BaseColor20;
  IBOutlet UIImageView *BaseColor21;
  IBOutlet UIImageView *BaseColor22;
  IBOutlet UIImageView *BaseColor23;
  IBOutlet UIImageView *BaseColor24;
  IBOutlet UIImageView *BaseColor30;
  IBOutlet UIImageView *BaseColor31;
  IBOutlet UIImageView *BaseColor32;
  IBOutlet UIImageView *BaseColor33;
  IBOutlet UIImageView *BaseColor34;
  
  IBOutlet UIImageView *Pattern00;
  IBOutlet UIImageView *Pattern01;
  IBOutlet UIImageView *Pattern02;
  IBOutlet UIImageView *Pattern03;
  IBOutlet UIImageView *Pattern04;
  IBOutlet UIImageView *Pattern10;
  IBOutlet UIImageView *Pattern11;
  IBOutlet UIImageView *Pattern12;
  IBOutlet UIImageView *Pattern13;
  IBOutlet UIImageView *Pattern14;
  IBOutlet UIImageView *Pattern20;
  IBOutlet UIImageView *Pattern21;
  IBOutlet UIImageView *Pattern22;
  IBOutlet UIImageView *Pattern23;
  IBOutlet UIImageView *Pattern24;
  IBOutlet UIImageView *Pattern30;
  IBOutlet UIImageView *Pattern31;
  IBOutlet UIImageView *Pattern32;
  IBOutlet UIImageView *Pattern33;
  IBOutlet UIImageView *Pattern34;
  
  IBOutlet UIButton *NextButton;
  
  IBOutlet UIActivityIndicatorView *ClusteringIndicator;
  
  NSMutableArray *SelectionHistory;
  NSMutableArray *GeneObjArr;
  NSMutableArray *ImgRectArr;
  NSMutableArray *PairArr;
  unsigned int selectionNum;
  int GenerationNum;
  long SelectImgNum;
}
- (IBAction) pushNextButton;
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (BOOL)touchesDetector:(NSSet *)touches withEvent:(UIEvent *)event;
- (long)selectImageDetector:(UIEvent *)event;
- (void)touchCoord:(NSSet *)touches touchX:(NSInteger *)touchX touchY:(NSInteger *)touchY;
- (void)moveUIImageView:(UIEvent *)event;
- (BOOL)checkOverLap:(UIEvent *)event;
- (void)resetPosition:(NSUInteger) num;
- (void)startupForNextGeneration:(NSMutableArray *)NextGeneArr;
- (void)ClusteringAndInitializing;
@end