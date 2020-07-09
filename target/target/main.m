//
//  main.m
//  target
//
//  Created by Darby Sauter on 2019-07-10.
//  Copyright © 2019 Darby Sauter. All rights reserved.
//

#import <Foundation/Foundation.h>

void talk(char* msg){
    printf("%s\n", msg);
}

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        while (1){
            printf("%s\n", "TICK");
            sleep(5);
        }
    }
    return 0;
}
