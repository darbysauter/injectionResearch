//
//  main.c
//  testInjection
//
//  Created by Darby Sauter on 2019-07-18.
//  Copyright © 2019 Darby Sauter. All rights reserved.
//

#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>


__attribute__((constructor))
static void initializer(void) {
    uint64_t addrOfTalk = (uint64_t)dlsym(RTLD_MAIN_ONLY, "talk");
    if (!addrOfTalk){
        printf("%s\n","Could not find symbol in main");
        return;
    }
    void (*fn)(char*) = (void(*)(char*))addrOfTalk;
    printf("found fn talk at 0x%llx\n", addrOfTalk);
    char* inject =  "-------------------------------------\n"
                    "| H     H      A      CCCCCC  K   K |\n"
                    "| H     H     A A     C       K  K  |\n"
                    "| H     H    A   A    C       K K   |\n"
                    "| HHHHHHH   AAAAAAA   C       KK    |\n"
                    "| H     H   A     A   C       K K   |\n"
                    "| H     H  A       A  C       K  K  |\n"
                    "| H     H  A       A  CCCCCC  K   K |\n"
                    "-------------------------------------";
    fn(inject);
}

// Finalizer.
__attribute__((destructor))
static void finalizer(void) {
    printf("[%s] finalizer()\n", __FILE__);
}
