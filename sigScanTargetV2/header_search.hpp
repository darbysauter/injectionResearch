//
//  header_search.hpp
//  targetV2Injection
//
//  Created by Mumphus on 2019-08-16.
//  Copyright © 2019 mumphus. All rights reserved.
//

#ifndef header_search_hpp
#define header_search_hpp

#include <mach/vm_map.h>
#include <mach/mach_vm.h>
#include <stdio.h>
#include <mach-o/dyld_images.h>
#include <mach-o/dyld.h>

void* findHeader(const char* image_name, mach_vm_size_t* size);

#endif /* header_search_hpp */
