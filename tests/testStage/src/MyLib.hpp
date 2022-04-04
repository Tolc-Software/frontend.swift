#pragma once
#include <string>

class Animal {
public:
  explicit Animal();
  ~Animal();

  std::string name;
  int i;
};

void fromCpp();
