//
//  Person.cpp
//  target V2
//
//  Created by Mumphus on 2019-08-06.
//  Copyright © 2019 mumphus. All rights reserved.
//

#include "Person.hpp"


Person::Person(const char* n, int a){
    std::cout << "Constructed Person\n";
    name = n;
    age = a;
}

Person::~Person(){
    std::cout << "Destructed Person\n";
}

void Person::talk(const char* str){
    std::cout << "Base Class(talk): " << str << std::endl;
}

void Person::printAge(){
    std::cout << "Base Class(age): " << age << std::endl;
}
