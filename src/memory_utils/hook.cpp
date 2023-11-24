#include "hook.h"
#include <pl/Hook.h>
#include <Windows.h>


namespace memory {

  FuncPtr resolveIdentifier(char const* identifier) {
    auto p = resolveSymbol(identifier);
    return p != nullptr ? p : resolveSignature(identifier);
}

int hook(FuncPtr target, FuncPtr detour, FuncPtr* originalFunc, HookPriority priority) {
    return pl::hook::pl_hook(target, detour, originalFunc, static_cast<pl::hook::Priority>(priority));
}

bool unhook(FuncPtr target, FuncPtr detour) { return pl::hook::pl_unhook(target, detour); }

} // namespace memory
