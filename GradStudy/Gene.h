#import <Foundation/NSArray.h>

@protocol Gene
@required
- (id)crossover:(id)partner;
@optional
- (id)compair:(id)target;
- (BOOL)mutation;
@end
