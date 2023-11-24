#include "patch.h"

std::vector<uint16_t> StringToBytes(const std::string &str) {
  std::vector<uint16_t> bytes;
  std::stringstream ss(str);
  std::string byteStr;

  while (ss >> byteStr) {
    if (byteStr == ET("??")) {
      bytes.push_back(0xFFFF);
    } else {
      bytes.push_back(std::stoi(byteStr, nullptr, 16));
    }
  }

  return bytes;
}

void memory::WriteEx(uintptr_t fovAddr, const std::vector<uint16_t> &bytes,
                     HANDLE hProcess = 0) {
  DWORD oldprotect;
  for (uint16_t i = 0; i < bytes.size(); ++i) {
    if (bytes[i] != 0xFFFF) {
      VirtualProtect((LPVOID)fovAddr, 1, PAGE_EXECUTE_READWRITE, &oldprotect);
      WriteProcessMemory(hProcess, (LPVOID)fovAddr, &bytes[i], 1, nullptr);
      VirtualProtect((LPVOID)fovAddr, 1, oldprotect, &oldprotect);
    }
    fovAddr++;
  }
}

void memory::WriteEx(memory::FuncPtr fovAddr,
                     const std::vector<uint16_t> &bytes, HANDLE hProcess = 0) {
  uintptr_t address = reinterpret_cast<uintptr_t>(fovAddr);
  memory::WriteEx(address, bytes, hProcess);
}

void memory::WriteEx(uintptr_t fovAddr, const std::string &byteStr,
                     HANDLE hProcess = 0) {
  std::vector<uint16_t> bytes = StringToBytes(byteStr);
  memory::WriteEx(fovAddr, bytes, hProcess);
}

void memory::WriteEx(memory::FuncPtr fovAddr, const std::string &byteStr,
                     HANDLE hProcess = 0) {
  uintptr_t address = reinterpret_cast<uintptr_t>(fovAddr);
  memory::WriteEx(address, byteStr, hProcess);
}