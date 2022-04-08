#include "MyLib.hpp"
#include <iostream>

Animal::Animal() { std::cout << "Animal constructor called" << '\n'; }

Animal::~Animal() { std::cout << "Animal destructor called" << '\n'; }
