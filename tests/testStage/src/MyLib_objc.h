#import <Foundation/Foundation.h>

void MyLibfromCpp();

@interface MyLibAnimal : NSObject

- (NSString *)name;
- (void)setName:newName;

- (int)i;
- (void)seti:(int)newi;
- (void)sayHi;

@property (class, nonatomic, assign, readonly) int userCount;

+ (int)userCount;

@end
