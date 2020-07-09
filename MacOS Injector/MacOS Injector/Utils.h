//
//  Utils.h
//  MacOS Injector
//
//  Created by Mumphus on 2019-07-27.
//  Copyright © 2019 mumphus. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "procInfo.h"

NSMutableArray* PI_enumerateProcesses(void);
NSString* pathFromPid(int pid);
