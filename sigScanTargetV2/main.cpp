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
#import "header_search.hpp"
#import "sig_scan.hpp"
#import "Utils.hpp"


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
    
    mach_vm_size_t region_size;
    printf("test\n");
    void* addr = findHeader("target V2", &region_size);
    
    const unsigned char* sig = (const unsigned char*)
    "\x55\x48\x89\xe5\x48\x83\xec\x40"
    "\x48\x89\x7d\xf8\x48\x89\x75\xf0"
    "\x89\x55\xec\x89\x4d\xe8\x48\x8b"
    "\x75\xf8\x48\x8b\x7d\xf0\x8b\x55"
    "\xec\x48\x89\x7d\xd0\x48\x89\xf7"
    "\x48\x8b\x45\xd0\x48\x89\x75\xc8"
    "\x48\x89\xc6\xe8\x88\x0d\x00\x00"
    "\x48\x8d\x05\xc1\x32\x00\x00\x48"
    "\x8b\x75\xc8\x48\x89\x06\x8b\x4d"
    "\xe8\x89\x4e\x18\x48\x8b\x3d\x85"
    "\x31\x00\x00\x48\x8d\x35\x02\x2e"
    "\x00\x00\xe8\x2b\x2b\x00\x00\x48"
    "\x89\x45\xc0\xe9\x00\x00\x00\x00"
    "\x48\x83\xc4\x40\x5d\xc3\x89\xd1"
    "\x48\x89\x45\xe0\x89\x4d\xdc\x48"
    "\x8b\x45\xc8\x48\x89\xc7\xe8\xcd"
    "\x0d\x00\x00\x48\x8b\x7d\xe0\xe8"
    "\x98\x2a\x00\x00\x0f\x0b";
    
    const char* mask =
    "xxxxxxxx"
    "xxxxxxxx"
    "xxxxxxxx"
    "xxxxxxxx"
    "xxxxxxxx"
    "xxxxxxxx"
    "xxx?????"
    "???????x"
    "xxxxxxxx"
    "xxxx????"
    "????????"
    "???????x"
    "xxx?????"
    "xxxxxxxx"
    "xxxxxxxx"
    "xxxxxx??"
    "???xxxx?"
    "????xx";
    
    int sigOffset = 0x3b;
    int ripOffset = 0x3f;
    
    printf("Starting sig scan\n");
    void* startOfSig = sig_scan(addr, region_size, sig, mask, strlen(mask));
    if (!startOfSig){
        printf("Could not find signature\n");
    }else{
        printf("Found signature starting at: %p\n",startOfSig);
    }
    printf("test3\n");
    
    void* vtable = getVTableAddr(startOfSig, sigOffset, ripOffset);
    
    printf("VTable Found at: %p\n",vtable);
    
    
    
    uintptr_t realStartOfTable = (uintptr_t)vtable;
    
    
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
