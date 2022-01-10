#import <Foundation/NSString.h>

@interface NSMutableString (swapWithRange)
+ (NSString *)swapWithRange:(NSMutableString *)str1 Target:(NSMutableString *)str2 Start:(NSInteger)start End:(NSInteger)end;
@end
