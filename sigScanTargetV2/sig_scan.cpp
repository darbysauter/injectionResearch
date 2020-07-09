//
//  sig_scan.cpp
//  sigScanTargetV2
//
//  Created by Mumphus on 2019-08-16.
//  Copyright © 2019 mumphus. All rights reserved.
//

#include "sig_scan.hpp"

void* sig_scan(void* baseAddr,
               unsigned long long size,
               const unsigned char *sig_str,
               const char *sig_mask,
               size_t sig_len) {
    
    unsigned char* pBasePtr = (unsigned char*)baseAddr;
    unsigned char* pEndPtr = (unsigned char*)baseAddr+size;
    size_t i;
    
    printf("Searghing %p - %p\n",pBasePtr, pEndPtr);
    
    while(pBasePtr < pEndPtr) {
        for(i = 0;i < sig_len;i++) {
            if((sig_mask[i] != '?') && (sig_str[i] != pBasePtr[i]))
                break;
        }
        
        // If 'i' reached the end, we know we have a match!
        if(i == sig_len)
            return (void*)pBasePtr;
        
        pBasePtr++;
    }
    
    return NULL;
}
