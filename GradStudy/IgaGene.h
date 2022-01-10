#import <Foundation/NSObject.h>
#import "Gene.h"

#if defined (nil)
  #undef nil
#endif

#if defined (Nil)
  #undef Nil
#endif

#define nil cpp_nil
#define Nil cpp_nil

//#import "som.h"

#undef nil
#undef Nil
#define nil NULL
#define Nil NULL

@interface IgaGene : NSObject <Gene>
{
  NSMutableDictionary *GeneType;
//  CGRect frame;
//  unsigned long number;
}
- (id<Gene>)init;
- (id<Gene>)initWithGeneType:(NSMutableArray *)Gene;
- (NSArray *)crossover:(IgaGene *)partner;
//- (double)similarity:(IgaGene *)target featureMap:(som *)smap;
- (BOOL)mutation;
- (NSMutableString *)floatToBitString:(float) floatValue digitNumber:(NSUInteger) dNum;
- (float)bitStringToFloat:(NSString *) bitString;

@property (readwrite) CGRect frame;
@property (readwrite) unsigned long number;

@end
