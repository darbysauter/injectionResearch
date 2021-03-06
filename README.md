# Resources
process scanning library
https://github.com/objective-see/ProcInfo for scanning processes

Injection method
http://newosxbook.com/src.jl?tree=listings&file=inject.c

Fixed method
https://knight.sc/malware/2019/03/15/code-injection-on-macos.html

# What this is?
I was curious about hooking/patching when injecting code into another process.
I wrote an injector for windows, but use MacOS more so I decided to conduct my learning on MacOS.

Began with target, which had a function talk(char*) which would just print a string, when injecting
the library into this process it will just call this function using the address found by dlsym

After that I wrote a C++ program that would have VTables

# injectionResearch
Researching Injection on Mac binaries

# sigScanTargetV2
This would get injected into the target process and scan for the signature of the target VTABLE to hook

# target V2
The program that was being targeted for hooking

# targetV2injection
Used hardcoded offsets to find the VTABLE and hook functions

# injector
A commandline injector that takes a PID and dylib

# MacOS injector
A GUI based injector, but I realized this only can hook into certain applications due to it not being able to be ran as sudo(maybe there is a way to do that but never looked it up)

# Running
To run build all of the binaries.

For target:
sudo ./injector libtestInjection.dylib

For target V2:
sudo ./injector libtargetV2Injection.dylib
sudo ./injector libsigScanTargetV2.dylib

# Note
Still works on Big Sur
