#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <vector>
#include <string>
#include <wchar.h>
#include "structs.h"

template < typename T >
  void WPM(pid_t pid, void * address,
    const T & value) {
    long data = value;
    ptrace(PTRACE_POKEDATA, pid, address, data);
  }

void print_ctrl() {
  std::cout << "[Numpad 0]  Enable god mode\n";
  std::cout << "[Numpad 1]  Enable infinite ammo\n";
  std::cout << "[Numpad 2]  Enable infinite armor\n";
  std::cout << "[Numpad 3]  Teleport using X, Y and Z co-ordinates\r\n";
}

void god_mode(unsigned long health_address, pid_t pid) {
  int newHealth = 999999;
  WPM(pid, health_address, newHealth);

  std::cout << "\nEnabled god mode.\r\n";
  sleep(3);
  std::cout << "\033[2J\033[1;1H";
  print_ctrl();
}

void infinite_ammo(unsigned long pweap_address, unsigned long pReserve_address,
  unsigned long sweap_address, unsigned long sReserve_address, pid_t pid) {
  int newAmmo = 99999;
  WPM(pid, pweap_address, newAmmo);
  WPM(pid, pReserve_address, newAmmo);

  WPM(pid, sweap_address, newAmmo);
  WPM(pid, sReserve_address, newAmmo);

  std::cout << "\nInfinite ammo enabled\r\n";
  sleep(3);
  std::cout << "\033[2J\033[1;1H";
  print_ctrl();
}

void infinite_armor(unsigned long armor_address, pid_t pid) {
  int newarmor = 99999;
  WPM(pid, armor_address, newarmor);
  std::cout << "\nEnabled infinite armor." << std::endl;
  sleep(3);
  std::cout << "\033[2J\033[1;1H";
  print_ctrl();
}

void teleport(unsigned long position_address, pid_t pid) {
  std::cout << "\033[2J\033[1;1H";
  std::cout << "Enter the co-ordinates separated by spaces you'd like to teleport to." << std::endl;
  Position pos;
  std::cin >> pos.x >> pos.y >> pos.z;

  WPM(pid, position_address, pos);
  std::cout << "\nTeleported to location." << std::endl;
  sleep(3);
  std::cout << "\033[2J\033[1;1H";
  print_ctrl();
}

pid_t GetProcId(const char * processName) {
  pid_t procid = 0;
  DIR * dir;
  struct dirent * ent;
  char * endptr;
  char buf[512];

  if (!(dir = opendir("/proc"))) {
    perror("can't open /proc");
    return procid;
  }

  while ((ent = readdir(dir)) != NULL) {
    long lpid = strtol(ent -> d_name, & endptr, 10);
    if ( * endptr != '\0') {
      continue;
    }
    snprintf(buf, sizeof(buf), "/proc/%ld/cmdline", lpid);
    FILE * fp = fopen(buf, "r");
    if (fp) {
      if (fgets(buf, sizeof(buf), fp) != NULL) {
        if (strstr(buf, processName) != NULL) {
          procid = lpid;
          break;
        }
      }
      fclose(fp);
    }
  }
  closedir(dir);
  return procid;
}

int main() {
  pid_t procid;
  procid = GetProcId("ac_client.exe");
  if (!procid) {
    std::cout << "AssaultCube process not found." << std::endl;
    return 0;
  }

  std::cout << "AssaultCube process found with ID " << procid << std::endl;
  std::cout << "Waiting for game to start..." << std::endl;

  int status;
  waitpid(procid, & status, 0);

  std::cout << "Game started! Press a key to begin the trainer." << std::endl;
  std::cin.get();

  uintptr_t pBaseAddr = 0x401000;
  uintptr_t phealth_address = pBaseAddr + 0xF8;
  uintptr_t pweap_address = pBaseAddr + 0x50C;
  uintptr_t pReserve_address = pBaseAddr + 0x510;
  uintptr_t sweap_address = pBaseAddr + 0x514;
  uintptr_t sReserve_address = pBaseAddr + 0x518;
  uintptr_t armor_address = pBaseAddr + 0xFC;
  uintptr_t position_address = pBaseAddr + 0x2D4;

  print_ctrl();

  while (true) {
    if (GetAsyncKeyState(VK_NUMPAD0)) {
      god_mode(phealth_address, procid);
    }
    if (GetAsyncKeyState(VK_NUMPAD1)) {
      infinite_ammo(pweap_address, pReserve_address, sweap_address, sReserve_address, procid);
    }
    if (GetAsyncKeyState(VK_NUMPAD2)) {
      infinite_armor(armor_address, procid);
    }
    if (GetAsyncKeyState(VK_NUMPAD3)) {
      teleport(position_address, procid);
    }
  }
  return 0;
}