//
//  Utils.c
//  MacOS Injector
//
//  Created by Mumphus on 2019-07-27.
//  Copyright © 2019 mumphus. All rights reserved.
//

#import "Utils.h"

//enumerate all running processes
NSMutableArray* PI_enumerateProcesses() {
    //# of procs
    int numberOfProcesses = 0;
    
    //array of pids
    pid_t* pids = NULL;
    
    //processes
    NSMutableArray* processes = nil;
    
    //alloc array
    processes = [NSMutableArray array];
    //get # of procs
    numberOfProcesses = proc_listallpids(NULL, 0);
    if(-1 == numberOfProcesses)
    {
        //bail
        NSLog(@"Couldnt get number of processes");
        goto bail;
    }
    
    //alloc buffer for pids
    pids = calloc(numberOfProcesses, sizeof(pid_t));
    if(nil == pids)
    {
        //bail
        NSLog(@"Couldnt allocate buffer for pids");
        goto bail;
    }
    
    //get list of pids
    numberOfProcesses = proc_listallpids(pids, numberOfProcesses*sizeof(pid_t));
    if(-1 == numberOfProcesses)
    {
        //bail
        NSLog(@"Couldnt get list of pids");
        goto bail;
    }
    
    //iterate over all pids
    // save pid into return array
    for(int i = 0; i < numberOfProcesses; ++i)
    {
        //save each pid
        if(0 != pids[i])
        {
            //save
            [processes addObject:[NSNumber numberWithUnsignedInt:pids[i]]];
        }
    }
    
bail:
    
    //free buffer
    if(NULL != pids)
    {
        //free
        free(pids);
        
        //unset
        pids = NULL;
    }
    
    return processes;
}


NSString* pathFromPid(int pid) {
    //buffer for process path
    char pathBuffer[PROC_PIDPATHINFO_MAXSIZE] = {0};
    
    //status
    int status = -1;
    
    //'management info base' array
    int mib[3] = {0};
    
    //system's size for max args
    int systemMaxArgs = 0;
    
    //process's args
    char* processArgs = NULL;
    
    NSString *returnString;
    
    //size of buffers, etc
    size_t size = 0;
    
    //clear out buffer
    bzero(pathBuffer, PROC_PIDPATHINFO_MAXSIZE);
    
    //1st:
    // attempt to get path via 'proc_pidpath()'
    status = proc_pidpath(pid, pathBuffer, sizeof(pathBuffer));
    if( (0 != status) &&
       (0 != strlen(pathBuffer)) )
    {
        //init path
        returnString = [NSString stringWithUTF8String:pathBuffer];
        
        //all set
        goto bail;
    }
    
    //2nd:
    // try via process's args ('KERN_PROCARGS2')
    
    //init mib
    // want system's size for max args
    mib[0] = CTL_KERN;
    mib[1] = KERN_ARGMAX;
    
    //set size
    size = sizeof(systemMaxArgs);
    
    //get system's size for max args
    if(-1 == sysctl(mib, 2, &systemMaxArgs, &size, NULL, 0))
    {
        //bail
        goto bail;
    }
    
    //alloc space for args
    processArgs = calloc(systemMaxArgs, sizeof(char));
    if(NULL == processArgs)
    {
        //bail
        goto bail;
    }
    
    //init mib
    // want process args
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROCARGS2;
    mib[2] = pid;
    
    //set size
    size = (size_t)systemMaxArgs;
    
    //get process's args
    if(-1 == sysctl(mib, 3, processArgs, &size, NULL, 0))
    {
        //bail
        goto bail;
    }
    
    //sanity check
    // ensure buffer is somewhat sane
    if(size <= sizeof(int))
    {
        //bail
        goto bail;
    }
    
    //extract process name
    // follows # of args (int) and is NULL-terminated
    returnString = [NSString stringWithUTF8String:processArgs + sizeof(int)];
    
bail:
    
    //free process args
    if(NULL != processArgs)
    {
        //free
        free(processArgs);
        
        //unset
        processArgs = NULL;
    }
    
    return returnString;
}
