# injectionResearch
Researching Injection on Mac binaries

# sigScanTargetV2 targetV2Injection sig scanner
This would get injected into the target process and scan for the signature of the target VTABLE to hook

# target V2
The program that was being targeted for hooking

# targetV2injection
Used hardcoded offsets to find the VTABLE and hook functions

# injector
A commandline injector that takes a PID and dylib

# MacOS injector
A GUI based injector, but I realized this only can hook into certain applications due to it not being able to be ran as sudo(maybe there is a way to do that but never looked it up)