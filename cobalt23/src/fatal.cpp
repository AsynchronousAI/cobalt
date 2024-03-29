#include <iostream>
#include <csignal>

#include "fatal.h"

extern int manual = 0;
int line = 0;
const char *file = NULL;

void fault_handler(int signal) {
  if (manual){
    printf(FATAL_ERR_MANUAL, file);
    std::exit(signal);
  }
  printf(FATAL_ERR, file, line, signal);
  std::exit(signal);
}

int handlefatal(int line, const char file[30]) {
  std::signal(SIGABRT, fault_handler);
  std::signal(SIGFPE, fault_handler);
  std::signal(SIGILL, fault_handler);
  //std::signal(SIGINT, fault_handler); -- Raised on Ctrl+C 
  std::signal(SIGSEGV, fault_handler);
  std::signal(SIGTERM, fault_handler);
  
  return 0;
}