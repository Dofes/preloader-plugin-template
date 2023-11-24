#include "memory.h"
#include <sstream>
#include <string>

namespace memory {
void WriteEx(uintptr_t fovAddr, const std::vector<uint16_t> &bytes,
             HANDLE hProcess);
void WriteEx(memory::FuncPtr fovAddr, const std::vector<uint16_t> &bytes,
             HANDLE hProcess);
void WriteEx(uintptr_t fovAddr, const std::string &byteStr, HANDLE hProcess);
void WriteEx(memory::FuncPtr fovAddr, const std::string &byteStr,
             HANDLE hProcess);
} // namespace memory