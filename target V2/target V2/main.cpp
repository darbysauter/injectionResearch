//
//  main.cpp
//  target V2
//
//  Created by Mumphus on 2019-08-06.
//  Copyright © 2019 mumphus. All rights reserved.
//

#include <iostream>
#include "Player.hpp"
#include "CustomArray.hpp"

bool mainLoop();

typedef void (*talk_t)(void*, const char*);
typedef void (*printAge_t)(void*);

int main(int argc, const char * argv[]) {
    std::cout << "Welcome to Target V2\n\n";
    std::cout << "Enter a Command:\n";
    while (mainLoop()){}
    return 0;
}

bool mainLoop(){
    static CustomArray<Player> players = CustomArray<Player>(10);
    static int age = 20;
    
    std::string input;
    
    std::cout << "> ";
    std::cin >> input;

    
    if (input == "exit"){
        std::cout << "EXITING.\n";
        return false;
    }
    
    else if (input == "talk"){
        std::cout << "HI\n";
    }
    
    else if (input == "create"){
        Player* p = new Player("Bill",age++,100);
        players.add(p);
    }
    
    else if (input == "0t"){
        (*players.get(0)).talk("Hello I am in spot 0");
    }
    
    else if (input == "1t"){
        (*players.get(1)).talk("Hello I am in spot 1");
    }
    
    else if (input == "0a"){
        (*players.get(0)).printAge();
    }
    
    else if (input == "1a"){
        (*players.get(1)).printAge();
    }
    
    else{
        std::cout << "Invalid Input\n";
    }
    
    return true;
}
