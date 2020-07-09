//
//  Player.hpp
//  target V2
//
//  Created by Mumphus on 2019-08-08.
//  Copyright © 2019 mumphus. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Person.hpp"

class Player: public Person {
public:
    Player(const char* n, int a, int h);
    ~Player();
    void talk(const char* str);
    void printAge();
    int hp;
};

#endif /* Player_hpp */
