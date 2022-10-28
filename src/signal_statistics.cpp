/*------------------------------------------------------------------------*/
/*! \file signal_statistics.cpp
    \brief used to handle signals, messages and statistics

  Part of AIGoFuzzing : Mutation-Based AIG Fuzzer
  Copyright(C) 2022 Daniela Kaufmann, Johannes Kepler University Linz
*/
/*------------------------------------------------------------------------*/

#include "signal_statistics.h"

/*------------------------------------------------------------------------*/
// Global variable
void(*original_SIGINT_handler)(int);
void(*original_SIGSEGV_handler)(int);
void(*original_SIGABRT_handler)(int);
void(*original_SIGTERM_handler)(int);
/*------------------------------------------------------------------------*/

const char * signal_name(int sig) {
  switch (sig) {
    case SIGINT: return "SIGINT";
    case SIGSEGV: return "SIGSEGV";
    case SIGABRT: return "SIGABRT";
    case SIGTERM: return "SIGTERM";
    default: return "SIGUNKNOWN";
  }
}

/*------------------------------------------------------------------------*/
static void catch_signal(int sig) {
  printf("c\nc caught signal '%s'(%d)\nc\n", signal_name(sig), sig);
  printf("c\nc raising signal '%s'(%d) again\n", signal_name(sig), sig);
  reset_all_signal_handlers();
  fflush(stdout);
  raise(sig);
}

/*------------------------------------------------------------------------*/

void init_all_signal_handers() {
  original_SIGINT_handler  = signal(SIGINT,  catch_signal);
  original_SIGSEGV_handler = signal(SIGSEGV, catch_signal);
  original_SIGABRT_handler = signal(SIGABRT, catch_signal);
  original_SIGTERM_handler = signal(SIGTERM, catch_signal);
}

/*------------------------------------------------------------------------*/



void reset_all_signal_handlers() {
  (void) signal(SIGINT, original_SIGINT_handler);
  (void) signal(SIGSEGV, original_SIGSEGV_handler);
  (void) signal(SIGABRT, original_SIGABRT_handler);
  (void) signal(SIGTERM, original_SIGTERM_handler);
}


/*------------------------------------------------------------------------*/

void msg(const char *fmt, ...) {
  va_list ap;
  fputs_unlocked("[af] ", stdout);
  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  va_end(ap);
  fputc_unlocked('\n', stdout);
  fflush(stdout);
}

/*------------------------------------------------------------------------*/

void die(const char *fmt, ...) {
  fflush(stdout);
  va_list ap;
  fputs_unlocked("*** [af] ", stderr);
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fputc('\n', stderr);
  fflush(stderr);
  exit(1);
}

/*------------------------------------------------------------------------*/
// Global variables

double init_time, reset_time;

/*------------------------------------------------------------------------*/

size_t maximum_resident_set_size() {
  struct rusage u;
  if (getrusage(RUSAGE_SELF, &u)) return 0;
  return((size_t) u.ru_maxrss) << 10;
}

/*------------------------------------------------------------------------*/

double process_time() {
  struct rusage u;
  if (getrusage(RUSAGE_SELF, &u)) return 0;
  double res = u.ru_utime.tv_sec + 1e-6 * u.ru_utime.tv_usec;
  res += u.ru_stime.tv_sec + 1e-6 * u.ru_stime.tv_usec;
  return res;
}

/*------------------------------------------------------------------------*/

void print_statistics() {
  msg("");
  msg("Statistics");
  msg("==========================================================");
  msg("  maximum resident set size:     %15.2f MB",
    maximum_resident_set_size() / static_cast<double>((1<<20)));
  msg("  used time for initializing:    %15.2f seconds",
      init_time);
  msg("  used time for reseting:        %15.2f seconds",
      reset_time - init_time);
  msg("  total process time:            %15.2f seconds",  process_time());
  msg("____________________________________________________________________");
}
