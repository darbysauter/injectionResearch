//
//  injection.h
//  MacOS Injector
//
//  Created by Mumphus on 2019-07-28.
//  Copyright © 2019 mumphus. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <mach/mach.h>
#import "procInfo.h"
#import <dlfcn.h>
#import <stdio.h>
#import <unistd.h>
#import <sys/types.h>
#import <mach/error.h>
#import <errno.h>
#import <stdlib.h>
#import <sys/sysctl.h>
#import <sys/mman.h>

#import <sys/stat.h>
#import <pthread.h>
#import <mach/mach_vm.h>


int inject(int PID, const char* path);
