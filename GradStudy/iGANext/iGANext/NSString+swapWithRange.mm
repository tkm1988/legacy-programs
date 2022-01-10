#import "NSString+swapWithRange.h"

@implementation NSMutableString (swapWithRange)
+ (NSString *)swapWithRange:(NSMutableString *)str1 Target:(NSMutableString *)str2 Start:(NSInteger) start End:(NSInteger) end
{
  NSString *topStr = [NSString stringWithString:[str1 substringToIndex:start]];
  NSString *midStr = [NSString stringWithString:[str2 substringWithRange:NSMakeRange(start, (end - start))]];
  NSString *footStr = [NSString stringWithString:[str1 substringFromIndex:end]];
  NSString *Joint = [NSString stringWithString:[[topStr stringByAppendingString:midStr] stringByAppendingString:footStr]];
  return Joint;
}
@end
