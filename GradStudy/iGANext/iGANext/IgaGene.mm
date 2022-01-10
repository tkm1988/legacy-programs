#import <ctime>
#import "IgaGene.h"
#import "NSString+swapWithRange.h"

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

static const int DIGIT_NUMBER = 7;
@implementation IgaGene
- (id<Gene>)init
{
  self = [super init];
  return self;
}

- (id<Gene>)initWithGeneType:(NSMutableDictionary *)Gene
{
  self = [super init];
  
  if(self != nil){
    NSMutableDictionary *wp = self->GeneType;
    self->GeneType = Gene;
    Gene = wp;
  }

  return self;
}

- (NSArray *)crossover:(IgaGene *) partner
{
  id Offspring1 = [[IgaGene alloc] init];
  id Offspring2 = [[IgaGene alloc] init];
  NSMutableDictionary *GeneType1 = [[NSMutableDictionary alloc] init];
  NSMutableDictionary *GeneType2 = [[NSMutableDictionary alloc] init];
  NSNumber *valueForOffspring1 = [[NSNumber alloc] init];
  NSNumber *valueForOffspring2 = [[NSNumber alloc] init];

  for (NSString *key in [self valueForKey:@"GeneType"]) {
    if ([key isEqual: @"Hue"] || [key isEqual: @"Collar"]) {
      int selfVal = [[[self valueForKey:@"GeneType"] objectForKey:key] floatValue]*100;
      int targetVal = [[[partner valueForKey:@"GeneType"] objectForKey:key] floatValue]*100;
      int start = std::min(selfVal, targetVal);
      int end = std::max(selfVal, targetVal);
      boost::mt19937 gen(static_cast<unsigned long>(std::time(0)));
      boost::uniform_smallint<> dst(start, end);
      boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > rand(gen, dst);
      valueForOffspring1 = [[NSNumber alloc] initWithFloat:rand()/100.0];
      valueForOffspring2 = [[NSNumber alloc] initWithFloat:rand()/100.0];
      
    }else if ([key isEqual: @"Saturation"] || [key isEqual: @"Value"]){
      NSInteger start = 0;
      NSInteger end = 0;
      
      NSMutableString *selfGeneStr = [self floatToBitString:[[[self valueForKey:@"GeneType"] objectForKey:key] floatValue] digitNumber:DIGIT_NUMBER];
      NSMutableString *partnerGeneStr = [self floatToBitString:[[[partner valueForKey:@"GeneType"] objectForKey:key] floatValue] digitNumber:DIGIT_NUMBER];
      
      boost::mt19937 gen(static_cast<unsigned long>(std::time(0)));
      boost::uniform_smallint<> dst1(0, ((int)[selfGeneStr length] - 1));
      boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > rand1(gen, dst1);
      start = rand1();
      
      boost::uniform_smallint<> dst2(start, ((int)[partnerGeneStr length]));
      boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > rand2(gen, dst2);
      end = rand2();
      
      valueForOffspring1 = [NSNumber numberWithFloat:
                                      [self bitStringToFloat:
                                       [NSMutableString swapWithRange:selfGeneStr
                                                               Target:partnerGeneStr
                                                                Start:start
                                                                  End:end]
                                       ]/100.0
                                      ];
      valueForOffspring2 = [NSNumber numberWithFloat:
                                      [self bitStringToFloat:
                                       [NSMutableString swapWithRange:partnerGeneStr
                                                               Target:selfGeneStr
                                                                Start:start
                                                                  End:end]
                                       ]/100.0
                                      ];
      
    }else{
      valueForOffspring1 = [[self valueForKey:@"GeneType"] objectForKey:key];
      valueForOffspring2 = [[partner valueForKey:@"GeneType"] objectForKey:key];
    }
    [GeneType1 setObject:valueForOffspring1 forKey:key];
    [GeneType2 setObject:valueForOffspring2 forKey:key];
  }
  
  [Offspring1 setValue:GeneType1 forKey:@"GeneType"];
  [Offspring2 setValue:GeneType2 forKey:@"GeneType"];

  return [[NSArray alloc] initWithObjects:Offspring1, Offspring2, nil];
}
//- (double)similarity:(IgaGene *)target featureMap:(som *)smap
//{
//  double sim = 0.0;
//
//  return sim;
//}
- (BOOL)mutation
{
  BOOL Mutation = NO;
  boost::mt19937 gen(static_cast<unsigned long>(std::time(0)));
  boost::uniform_real<> dst(0, 1);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > occurrence(gen, dst);

  if (occurrence() < 0.1) {
    boost::uniform_smallint<> dst(0, (unsigned int)[[self valueForKey:@"GeneType"] count]);
    boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > chooseIndex(gen, dst);
    NSArray *attributeArr = [[NSArray alloc] initWithObjects:
                             @"Hue",
                             @"Saturation",
                             @"Value",
                             @"Collar",
                             @"PatternName",
                             @"PatternColor",
                             @"Sleeve",nil];
    NSString *key = [attributeArr objectAtIndex:chooseIndex()];
    
    if ([key isEqual: @"Hue"]) {
      boost::uniform_smallint<> dst(0, 359);
    }else if ([key isEqual: @"Saturation"] || [key isEqual: @"Value"]){
      boost::uniform_smallint<> dst(0, 100);
    }else if ([key isEqual: @"Collar"]){
      boost::uniform_smallint<> dst(0, 14);
    }else if ([key isEqual: @"PatternName"]){
      boost::uniform_smallint<> dst(0, 7);
    }else if ([key isEqual: @"PatternColor"]){
      
    }else{
      if([[self valueForKey:@"GeneType"] objectForKey:key] == 0){
        [[self valueForKey:@"GeneType"] setObject:@1 forKey:key];
      }else{
        [[self valueForKey:@"GeneType"] setObject:@0 forKey:key];
      }
    }
    
    if ([key isEqual: @"Hue"]) {
      boost::uniform_smallint<> dst(0, 359);
    }else if ([key isEqual: @"Saturation"] || [key isEqual: @"Value"]){
      boost::uniform_smallint<> dst(0, 100);
    }else if ([key isEqual: @"Collar"]){
      boost::uniform_smallint<> dst(0, 14);
    }else if ([key isEqual: @"PatternName"]){
      boost::uniform_smallint<> dst(0, 7);
    }else if ([key isEqual: @"PatternColor"]){
      boost::uniform_smallint<> dst(0, 8);
    }else{
      if([[self valueForKey:@"GeneType"] objectForKey:key] == 0){
        [[self valueForKey:@"GeneType"] setObject:@1 forKey:key];
      }else{
        [[self valueForKey:@"GeneType"] setObject:@0 forKey:key];
      }
    }
    
    if ([key isEqual: @"Sleeve"]) {
     //None
    }else{
      boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > mutationResult(gen, dst);
      [[self valueForKey:@"GeneType"] setObject:[NSNumber numberWithInt:mutationResult()] forKey:key];
    }
    Mutation = YES;
  }else{
    /*
      NONE
    */
  }

  return Mutation;
}
- (NSMutableString *)floatToBitString:(float) floatValue digitNumber:(NSUInteger)dNum{
  NSMutableString *result = [[NSMutableString alloc] init];
  
  for (NSInteger intValue = (NSInteger)(floatValue * 100.0); intValue > 0; intValue/=2) {
    ((intValue % 2) == 0)?[result appendString:@"0"]:[result appendString:@"1"];
  }
  
  if([result length] < dNum){
    for (int i = dNum - [result length]; i > 0; --i){
      [result insertString:@"0" atIndex:0];
    }
  }
  
  return result;
}
- (float)bitStringToFloat:(NSString *)bitString{
  float result = 0.0;
  
  for (int i = [bitString length] - 1, j = 0; i >= 0; --i, ++j) {
    if([[bitString substringWithRange:NSMakeRange(i, 1)] compare:@"1"] == NSOrderedSame){
      result += pow(2.0, j);
    }
  }
  
  return result;
}
@end
