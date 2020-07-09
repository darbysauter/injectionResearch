//
//  injection.m
//  MacOS Injector
//
//  Created by Mumphus on 2019-07-28.
//  Copyright © 2019 mumphus. All rights reserved.
//

#import "injection.h"

#define STACK_SIZE 65536
#define CODE_SIZE 128


char injectedCode[] =
// "\xCC"                            // int3

"\x55"                            // push       rbp
"\x48\x89\xE5"                    // mov        rbp, rsp
"\x48\x83\xEC\x10"                // sub        rsp, 0x10
"\x48\x8D\x7D\xF8"                // lea        rdi, qword [rbp+var_8]
"\x31\xC0"                        // xor        eax, eax
"\x89\xC1"                        // mov        ecx, eax
"\x48\x8D\x15\x21\x00\x00\x00"    // lea        rdx, qword ptr [rip + 0x21]
"\x48\x89\xCE"                    // mov        rsi, rcx
"\x48\xB8"                        // movabs     rax, pthread_create_from_mach_thread
"PTHRDCRT"
"\xFF\xD0"                        // call       rax
"\x89\x45\xF4"                    // mov        dword [rbp+var_C], eax
"\x48\x83\xC4\x10"                // add        rsp, 0x10
"\x5D"                            // pop        rbp
"\x48\xc7\xc0\x13\x0d\x00\x00"    // mov        rax, 0xD13
"\xEB\xFE"                        // jmp        0x0
"\xC3"                            // ret

"\x55"                            // push       rbp
"\x48\x89\xE5"                    // mov        rbp, rsp
"\x48\x83\xEC\x10"                // sub        rsp, 0x10
"\xBE\x01\x00\x00\x00"            // mov        esi, 0x1
"\x48\x89\x7D\xF8"                // mov        qword [rbp+var_8], rdi
"\x48\x8D\x3D\x1D\x00\x00\x00"    // lea        rdi, qword ptr [rip + 0x2c]
"\x48\xB8"                        // movabs     rax, dlopen
"DLOPEN__"
"\xFF\xD0"                        // call       rax
"\x31\xF6"                        // xor        esi, esi
"\x89\xF7"                        // mov        edi, esi
"\x48\x89\x45\xF0"                // mov        qword [rbp+var_10], rax
"\x48\x89\xF8"                    // mov        rax, rdi
"\x48\x83\xC4\x10"                // add        rsp, 0x10
"\x5D"                            // pop        rbp
"\xC3"                            // ret

"LIBLIBLIBLIB"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";


int inject(int PID, const char* path){
    
    kern_return_t kern_return = 0;
    mach_port_t task = 0;
    
    printf("\n[+] Targeted Process: %d\n", PID);
    
    
    /**
     * First, check we have the library. Otherwise, we won't be able to inject..
     */
    struct stat buf;
    int rc = stat(path, &buf);
    if (rc != 0) {
        fprintf(stderr, "Unable to open library file %s (%s) - Cannot inject\n", path, strerror(errno));
        //return (-9);
    }
    
    
    kern_return = task_for_pid( mach_task_self(), PID, &task);
    if( kern_return != KERN_SUCCESS )
    {
        printf( "task_for_pid failed: %s\n", mach_error_string( kern_return ) );
        return 1;
    }
    
    printf("[+] Got task: %d\n", task);
    
    
    /**
     * From here on, it's pretty much straightforward -
     * Allocate stack and code. We don't really care *where* they get allocated. Just that they get allocated.
     * So, first, stack:
     */
    mach_vm_address_t remoteStack64 = (vm_address_t)NULL;
    mach_vm_address_t remoteCode64 = (vm_address_t)NULL;
    kern_return = mach_vm_allocate(task, &remoteStack64, STACK_SIZE, VM_FLAGS_ANYWHERE);
    if (kern_return != KERN_SUCCESS) {
        fprintf(stderr, "Unable to allocate memory for remote stack in thread: Error %s\n", mach_error_string(kern_return));
        return (-2);
    }
    else {
        fprintf(stderr, "[+] Allocated remote stack 0x%llx\n", remoteStack64);
    }
    /**
     * Then we allocate the memory for the thread
     */
    remoteCode64 = (vm_address_t)NULL;
    kern_return = mach_vm_allocate(task, &remoteCode64, CODE_SIZE, VM_FLAGS_ANYWHERE);
    if (kern_return != KERN_SUCCESS) {
        fprintf(stderr, "Unable to allocate memory for remote code in thread: Error %s\n", mach_error_string(kern_return));
        return (-2);
    }
    
    /**
     * Patch code before injecting: That is, insert correct function addresses (and lib name) into placeholders
     *
     * Since we use the same shared library cache as our victim, meaning we can use memory addresses from
     * OUR address space when we inject..
     */
    
    int i = 0;
    char *possiblePatchLocation = (injectedCode);
    for (i = 0; i < 0x100; i++) {
        // Patching is crude, but works.
        //
        extern void *_pthread_set_self;
        possiblePatchLocation++;
        
        uint64_t addrOfPthreadCreate = (uint64_t)dlsym(RTLD_DEFAULT, "pthread_create_from_mach_thread");
        uint64_t addrOfPthreadExit = (uint64_t)dlsym(RTLD_DEFAULT, "pthread_exit");
        uint64_t addrOfDlopen = (uint64_t)dlopen;
        
        if (memcmp(possiblePatchLocation, "PTHRDCRT", 8) == 0) {
            printf("[+] pthread_create_from_mach_thread 0x%llx\n", addrOfPthreadCreate);
            memcpy(possiblePatchLocation, &addrOfPthreadCreate, 8);
        }
        
        if (memcmp(possiblePatchLocation, "DLOPEN__", 6) == 0) {
            printf("[+] Found dlopen 0x%llx\n", addrOfDlopen);
            memcpy(possiblePatchLocation, &addrOfDlopen, sizeof(uint64_t));
        }
        
        if (memcmp(possiblePatchLocation, "LIBLIBLIB", 9) == 0) {
            strcpy(possiblePatchLocation, path);
        }
    }
    
    /**
     * Write the (now patched) code
     */
    kern_return = mach_vm_write(task,                 // Task port
                                remoteCode64,               // Virtual Address (Destination)
                                (vm_address_t)injectedCode, // Source
                                sizeof(injectedCode));                      // Length of the source
    
    if (kern_return != KERN_SUCCESS) {
        fprintf(stderr, "Unable to write remote thread memory: Error %s\n", mach_error_string(kern_return));
        return (-3);
    }
    
    /*
     * Mark code as executable - This also requires a workaround on iOS, btw.
     */
    kern_return = vm_protect(task, remoteCode64, sizeof(injectedCode), FALSE, VM_PROT_READ | VM_PROT_EXECUTE);
    
    /*
     * Mark stack as writable  - not really necessary
     */
    kern_return = vm_protect(task, remoteStack64, STACK_SIZE, TRUE, VM_PROT_READ | VM_PROT_WRITE);
    if (kern_return != KERN_SUCCESS) {
        fprintf(stderr, "Unable to set memory permissions for remote thread: Error %s\n", mach_error_string(kern_return));
        return (-4);
    }
    
    /*
     * Create thread - This is obviously hardware specific.
     */
    x86_thread_state64_t remoteThreadState64;
    
    thread_act_t remoteThread;
    
    memset(&remoteThreadState64, '\0', sizeof(remoteThreadState64));
    
    remoteStack64 += (STACK_SIZE / 2); // this is the real stack
    //remoteStack64 -= 8;  // need alignment of 16
    
    const char *p = (const char *)remoteCode64;
    
    remoteThreadState64.__rip = (u_int64_t)(vm_address_t)remoteCode64;
    
    // set remote Stack Pointer
    remoteThreadState64.__rsp = (u_int64_t)remoteStack64;
    remoteThreadState64.__rbp = (u_int64_t)remoteStack64;
    
    printf("[+] Remote Stack 64  0x%llx, Remote code is %p\n", remoteStack64, p);
    
    /*
     * create thread and launch it in one go
     */
    kern_return = thread_create_running(task, x86_THREAD_STATE64,
                                        (thread_state_t)&remoteThreadState64, x86_THREAD_STATE64_COUNT, &remoteThread);
    if (kern_return != KERN_SUCCESS) {
        fprintf(stderr, "Unable to create remote thread: error %s", mach_error_string(kern_return));
        return (-3);
    }
    
    // Wait for mach thread to finish
    mach_msg_type_number_t thread_state_count = x86_THREAD_STATE64_COUNT;
    for (;;) {
        kern_return = thread_get_state(remoteThread, x86_THREAD_STATE64, (thread_state_t)&remoteThreadState64, &thread_state_count);
        if (kern_return != KERN_SUCCESS) {
            fprintf(stderr, "Error getting stub thread state: error %s", mach_error_string(kern_return));
            break;
        }
        
        if (remoteThreadState64.__rax == 0xD13) {
            printf("[+] Stub thread finished\n");
            kern_return = thread_terminate(remoteThread);
            if (kern_return != KERN_SUCCESS) {
                fprintf(stderr, "Error terminating stub thread: error %s", mach_error_string(kern_return));
            }
            break;
        }
    }
    
    return 0;
}
