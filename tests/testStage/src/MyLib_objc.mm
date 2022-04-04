#include <Foundation/Foundation.h>
#include "MyLib_objc.h"
#include "MyLib.hpp"
#include <iostream>
#include <memory>

void MyLibfromCpp() {
  fromCpp();
}

@implementation MyLibAnimal

// This is the actual value
std::unique_ptr<Animal> a;

static int _userCount = 0;

-(instancetype)init {
  if (self = [super init]) {
    a = std::unique_ptr<Animal>(new Animal());
    a->i = 10;
    a->name = "Ior";
  }
  return self;
}

+ (int)userCount {
  return _userCount;
}

-(NSString*)name {
  return [NSString stringWithUTF8String: a->name.c_str()];
}

-(void)setName:newName {
  a->name = [newName UTF8String];
}

-(int)i {
  return a->i;
}

-(void)seti:(int)newi {
  a->i = newi;
}

-(void)sayHi {
  NSLog(@"Hello from Objective-C++");
  fromCpp();
}


@end
