//
//  Player.cpp
//  target V2
//
//  Created by Mumphus on 2019-08-08.
//  Copyright © 2019 mumphus. All rights reserved.
//

#include "Player.hpp"

Player::Player(const char* n, int a, int h): Person(n,a){
    hp = h;
    std::cout << "Constructed Player\n";
}

Player::~Player(){
    std::cout << "Destructed Player\n";
}

void Player::talk(const char* str){
    std::cout << "Player Class(talk): " << str << std::endl;
}

void Player::printAge(){
    std::cout << "Player Class(printAge): " << age << std::endl;
}

