#include "memory.h"
#include <iostream>
#include <unordered_map>

#include <pl/SymbolProvider.h>

#define INRANGE(x, a, b) (x >= a && x <= b)
#define GET_BYTE(x) (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS(x)                                                            \
  (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa)              \
                                    : (INRANGE(x, '0', '9') ? x - '0' : 0))

namespace memory {

std::unordered_map<std::string, FuncPtr> &getSignatureCache() {
  static std::unordered_map<std::string, FuncPtr> cache;
  return cache;
}

uintptr_t FindSig(const char *szSignature) {
  std::cout << szSignature << std::endl;
  const char *pattern = szSignature;
  uintptr_t firstMatch = 0;
  static const uintptr_t rangeStart = (uintptr_t)GetModuleHandleA(NULL);
  static MODULEINFO miModInfo;
  static bool init = false;
  if (!init) {
    init = true;
    GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo,
                         sizeof(MODULEINFO));
  }
  static const uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;

  BYTE patByte = GET_BYTE(pattern);
  const char *oldPat = pattern;

  for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++) {
    if (!*pattern)
      return firstMatch;

    while (*(PBYTE)pattern == ' ')
      pattern++;

    if (!*pattern)
      return firstMatch;

    if (oldPat != pattern) {
      oldPat = pattern;
      if (*(PBYTE)pattern != '\?')
        patByte = GET_BYTE(pattern);
    }

    if (*(PBYTE)pattern == '\?' || *(BYTE *)pCur == patByte) {
      if (!firstMatch)
        firstMatch = pCur;

      if (!pattern[2] || !pattern[1])
        return firstMatch;
      pattern += 2;
    } else {
      pattern = szSignature;
      firstMatch = 0;
    }
  }
  return 0;
}

FuncPtr resolveSymbol(char const* symbol) { return pl::symbol_provider::pl_resolve_symbol(symbol); }

FuncPtr resolveSignature(const char *signature) {
  auto sigCache = getSignatureCache();
  if (sigCache.contains(signature)) {
    return sigCache[signature];
  }
  sigCache[signature] = reinterpret_cast<FuncPtr>(FindSig(signature));
  std::cout << sigCache[signature] << std::endl;
  return sigCache[signature];
}
} // namespace memory