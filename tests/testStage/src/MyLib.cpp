#include "MyLib.hpp"
#include <iostream>

void fromCpp() { std::cout << "Hello from C++" << '\n'; }

Animal::Animal() { std::cout << "Animal constructor called" << '\n'; }

Animal::~Animal() { std::cout << "Animal destructor called" << '\n'; }
