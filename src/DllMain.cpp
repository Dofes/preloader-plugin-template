#include "memory_utils/hook.h"
#include <iostream>


#include <Windows.h>
#include <stdio.h>

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        printf("Hello from DLL!\n");
    } else if (fdwReason == DLL_PROCESS_DETACH && !lpReserved) {
        printf("Goodbye from DLL!\n");
    }
    return TRUE;
}

class DedicatedServer {};

LP_AUTO_TYPED_INSTANCE_HOOK(
    DedicatedServer_init,
    memory::HookPriority::Normal,
    DedicatedServer,
    "??0DedicatedServer@@QEAA@XZ",
    void,
) {
    std::cout << "Server started!" << std::endl;
    origin();
}
