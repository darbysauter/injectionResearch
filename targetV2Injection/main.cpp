//
//  main.cpp
//  targetV2Injection
//
//  Created by Mumphus on 2019-08-09.
//  Copyright © 2019 mumphus. All rights reserved.
//

/*
 mach_vm_region_info to get base address of image
*/

#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <iostream>
#import <mach/vm_map.h>


typedef void (*talk_t)(void*,const char*);
typedef void (*printAge_t)(void*);

void customTalk(void* thisptr,const char* str){
    std::cout << "Hooked: " << str << std::endl;
}

void customAge(void* thisptr){
    std::cout << "HOOKED\n";
}

__attribute__((constructor))
static void initializer(void) {
    printf("[%s] initializer()\n", __FILE__);
    
    const char* symbol = "_ZTV6Player";
    void* playerVTable = dlsym(RTLD_MAIN_ONLY, symbol); // for some reason handle wont work
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        fprintf(stderr, "Cannot load symbol '%s': %s\n", symbol, dlsym_error);
        exit(EXIT_FAILURE);
    }
    
    uintptr_t realStartOfTable = ((uintptr_t)playerVTable) + 0x10;
    
    printf("addr of vTable: %p\n", (uintptr_t*)realStartOfTable);
    uintptr_t* addrOfTalk = (uintptr_t*)(realStartOfTable + 0x00);
    uintptr_t* addrOfPrintAge = (uintptr_t*)(realStartOfTable + 0x08);
    
    void (*originalFn1)(const char*) = (void (*)(const char*))(*addrOfTalk);
    void (*originalFn2)(void) = (void (*)(void))(*addrOfPrintAge);
    
    
    *addrOfTalk = (uintptr_t)customTalk;
    *addrOfPrintAge = (uintptr_t)customAge;
}

__attribute__((destructor))
static void finalizer(void) {
    printf("[%s] finalizer()\n", __FILE__);
}
