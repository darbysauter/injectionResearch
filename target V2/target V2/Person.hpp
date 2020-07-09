//
//  Person.hpp
//  target V2
//
//  Created by Mumphus on 2019-08-06.
//  Copyright © 2019 mumphus. All rights reserved.
//

#ifndef Person_hpp
#define Person_hpp

#include <stdio.h>
#include <iostream>

class Person {
public:
    Person(const char* n, int a);
    ~Person();
    virtual void talk(const char* str);
    virtual void printAge();
    int age;
private:
protected:
    const char* name;
};


#endif /* Person_hpp */
