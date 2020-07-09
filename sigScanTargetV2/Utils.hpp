//
//  Utils.hpp
//  sigScanTargetV2
//
//  Created by Mumphus on 2019-08-16.
//  Copyright © 2019 mumphus. All rights reserved.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
#include <stdint.h>

void* getVTableAddr(void* addr, int VTOffset, int RIPOffset);

#endif /* Utils_hpp */
