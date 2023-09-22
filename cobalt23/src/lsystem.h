#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#define W

#elif defined(LUA_USE_POSIX) || defined(__unix__) || defined(__unix) || \
    defined(__APPLE__)
#include <sys/utsname.h>
#define U
#else
#define D
#endif

static const char* getCPUName() {
#ifdef W
  char cpuName[256];
  DWORD bufferSize = sizeof(cpuName);
  DWORD type = REG_SZ;
  HKEY hKey;

  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                   "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0,
                   KEY_READ, &hKey) == ERROR_SUCCESS) {
    if (RegQueryValueEx(hKey, "ProcessorNameString", NULL, &type,
                        (LPBYTE)cpuName, &bufferSize) == ERROR_SUCCESS) {
      RegCloseKey(hKey);
      return cpuName;
    }
    RegCloseKey(hKey);
  }

  return "Unknown Windows CPU";
#elif defined(U)
  struct utsname buf;
  if (uname(&buf) == 0) {
    return buf.machine;
  }
  return "Unknown Unix CPU";
#else
  return "Unknown CPU";
#endif
}

static const char* getGPUName() {
#ifdef W
  char gpuName[256];
  DWORD bufferSize = sizeof(gpuName);
  HDEVINFO deviceInfoSet =
      SetupDiGetClassDevs(&GUID_DEVCLASS_DISPLAY, NULL, NULL, DIGCF_PRESENT);
  if (deviceInfoSet != INVALID_HANDLE_VALUE) {
    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (DWORD i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData);
         i++) {
      DWORD dataType;
      if (SetupDiGetDeviceRegistryProperty(
              deviceInfoSet, &deviceInfoData, SPDRP_HARDWAREID, &dataType,
              (BYTE*)gpuName, bufferSize, &bufferSize)) {
        if (strstr(gpuName, "VEN_") && strstr(gpuName, "DEV_")) {
          char* vendorID = strstr(gpuName, "VEN_") + 4;
          char* deviceID = strstr(gpuName, "DEV_") + 4;
          *strstr(vendorID, "&") = '\0';
          *strstr(deviceID, "&") = '\0';
          sprintf(gpuName, "%s %s", vendorID, deviceID);
          SetupDiDestroyDeviceInfoList(deviceInfoSet);
          return gpuName;
        }
      }
    }
    SetupDiDestroyDeviceInfoList(deviceInfoSet);
  }
  return "Unknown Windows GPU";
#elif defined(U)
  return "Unknown Unix GPU";
#else
  return "Unknown GPU";
#endif
}
#ifdef __cplusplus
}
#endif
