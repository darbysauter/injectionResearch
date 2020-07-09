//
//  sig_scan.hpp
//  sigScanTargetV2
//
//  Created by Mumphus on 2019-08-16.
//  Copyright © 2019 mumphus. All rights reserved.
//

#ifndef sig_scan_hpp
#define sig_scan_hpp

#include <stdio.h>
void* sig_scan(void* baseAddr,
               unsigned long long size,
               const unsigned char *sig_str,
               const char *sig_mask,
               size_t sig_len);

#endif /* sig_scan_hpp */
