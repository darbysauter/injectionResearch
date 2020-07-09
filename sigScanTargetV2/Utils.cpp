//
//  Utils.cpp
//  sigScanTargetV2
//
//  Created by Mumphus on 2019-08-16.
//  Copyright © 2019 mumphus. All rights reserved.
//

#include "Utils.hpp"


void* getVTableAddr(void* addr, int VTOffset, int RIPOffset){
    uint8_t* a = (uint8_t*)((uintptr_t)addr+VTOffset);
    uintptr_t RIP = (uintptr_t)addr + RIPOffset;
    
    int off = (a[0] << 0) + (a[1] << 8) + (a[2] << 16) + (a[3] << 24);
    
    void* vtAddr = (void*)(RIP + (uintptr_t)off);
    
    printf("Using RIP{%p} + Offset{0x%x}",(void*)RIP, off);
//    printf("vtable: %p, baseSigAddr: %p, offsetInSig: %d, RIP: %p",vtAddr, addr, offset)
    
    return vtAddr;
}
