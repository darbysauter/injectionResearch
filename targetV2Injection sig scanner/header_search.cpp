//
//  header_search.cpp
//  targetV2Injection
//
//  Created by Mumphus on 2019-08-16.
//  Copyright © 2019 mumphus. All rights reserved.
//

#include "header_search.hpp"

void* findHeader(const char* image_name, mach_vm_size_t* size){
    int num_images = _dyld_image_count();
    printf("%d images\n", num_images);
    mach_vm_address_t addr;
    
    for (int i = 0; i<=num_images; i++){
        if (i == num_images){
            return NULL;
        }
        const char* path = _dyld_get_image_name(i);
        if (strstr(path, image_name)){
            addr = (mach_vm_address_t) _dyld_get_image_header(i);
            printf("Found %s\n", image_name);
            break;
        }
    }
    
    
    kern_return_t kr;
    mach_msg_type_number_t infoCnt = sizeof (vm_region_basic_info_data_64_t);
    mach_port_t object_name;
    vm_region_basic_info_data_t info;
    
    kr = mach_vm_region(mach_task_self(),
                        &addr,
                        size,
                        VM_REGION_BASIC_INFO,
                        (vm_region_info_t)&info,
                        &infoCnt,
                        &object_name);
    
    if (kr != KERN_SUCCESS){
        printf("mach_vm_region return error:\n %s\n", mach_error_string(kr));
        return NULL;
    }
    
    printf("addr: %p size: %llu\n", (void*)addr, *size);
    return (void*)addr;
}
