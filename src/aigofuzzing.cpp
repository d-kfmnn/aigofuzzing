/*------------------------------------------------------------------------*/
/*! \file aigofuzzing.cpp
    \brief main file of our tool AIGoFuzzing

  Part of AIGoFuzzing : Mutation-Based AIG Fuzzer
  Copyright(C) 2022 Daniela Kaufmann, Johannes Kepler University Linz
*/
/*------------------------------------------------------------------------*/
#define VERSION "1.0"
/*------------------------------------------------------------------------*/
// / Manual of AIGoFuzzing, will be printed with command line '-h'
static const char * USAGE =
"[agf] \n"
"[agf] ### USAGE ###\n"
"[agf] usage : aigofuzzing  <inp> [out] [-h] [-m n] [-s n] [-t n] \n"
"[agf] \n"
"[agf] inp     name of input file\n"
"[agf] \n"
"[agf] out     name of output file\n"
"[agf] -h      prints this help\n"
"[agf] -m n    sets the number of modifications to 'n', default 1 \n"
"[agf] -s n    sets the seed to 'n'\n"
"[agf] -t n    sets the selected technique to 'n' \n"
"[agf]         possible values: 1 switch internal signs \n"
"[agf]                          2 switch sign of output \n"
"[agf]                          3 modify input \n"
"[agf]                          4 insert constant \n"
"[agf] \n";
/*------------------------------------------------------------------------*/
#include "fuzzer.h"
#include <algorithm>
#include <climits>
#include <cstring>
#include <time.h>

/*------------------------------------------------------------------------*/
// / Name of the input file
static const char * input_name = 0;
static const char * output_name = 0;
static double seed = 0;
static int technique = 0;
static int max_tech = 4;

/*------------------------------------------------------------------------*/
static bool isNumber(const std::string &s) {
  return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

/**
    @see init_all_signal_handlers()
*/
static void init_all() {
  init_all_signal_handers();

  if(!seed) {
    srand(time(NULL));
    seed = rand()%INT_MAX;
  }

  msg("Initialization");
  msg("==========================================================");
  msg("  Seed:            %.f", seed);
  srand(seed);

  init_time = process_time();
}
/*------------------------------------------------------------------------*/

/**
    Calls the deallocaters of the involved data types
    @see reset_all_signal_handlers()
*/
static void reset_all() {
  reset_all_signal_handlers();
  reset_aig_parsing();

  reset_time = process_time();
}
/*------------------------------------------------------------------------*/
/**
    Main Function of AIGoFuzzing.
    Reads the given AIG and depending on the selected mode, either
    calls the substution engine or the polynomial solver.

    Prints statistics to stdout after finishing.
*/
int main(int argc, char ** argv) {
  msg("AIGoFuzzing " VERSION);
  msg("AIG Generation based Fuzzer");
  msg("Copyright(C) 2022, Daniela Kaufmann, Johannes Kepler University Linz");
  msg("____________________________________________________________________");
  msg("");
  msg("");

  int max = 1;

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-h") ||
    !strcmp(argv[i], "--help")) {
      fputs(USAGE, stdout);
      fflush(stdout);
      exit(0);
    } else if (!strcmp(argv[i], "-t")) {
      if(i == argc-1) die("no value for option '-t' given");

      if(!isNumber(argv[++i])) die("argument '%s' invalid, \n         "
        "option '-t' needs to be followed by a number between 1 and %i", argv[i], max_tech);
      else technique = std::stoi(argv[i], nullptr);

      if (!technique || technique > max_tech) die("argument '%s' invalid, \n         "
        "option '-t' needs to be followed by a number between 1 and %i", argv[i], max_tech);

    } else if (!strcmp(argv[i], "-s")) {
      if(i == argc-1) die("no value for option '-s' given");

      if(!isNumber(argv[++i])) die("argument '%s' invalid, \n                  "
        "option '-s' needs to be followed by a nonnegative integer", argv[i]);
      else seed = std::stoi(argv[i], nullptr);

    } else if (!strcmp(argv[i], "-m")) {
      if(i == argc-1) die("no value for option '-m' given");

      if(!isNumber(argv[++i])) die("argument '%s' invalid, \n                  "
        "option '-m' needs to be followed by a nonnegative integer", argv[i]);
      else max = std::stoi(argv[i], nullptr);
    } else if (output_name) {
      die("too many arguments '%s', '%s' and '%s'(try '-h')",
        input_name, output_name, argv[i]);
    } else if (input_name) { output_name = argv[i];
    } else {
      input_name = argv[i];
    }
  }

  if (!input_name)  die("no input file given(try '-h')");

  init_all();
  parse_aig(input_name);

  int sel_tech = technique;

  for(int i = 0; i < max; i++){
    if(!technique){
      sel_tech = rangeRandomZeroMax(4) + 1;
    }

    switch(sel_tech){
      case 1:
        flip_internal_sign();
        break;
      case 2:
        flip_output_sign();
        break;
      case 3:
        change_input_of_internal_node();
        break;
      case 4:
        insert_constant();
        break;
    }
  }

  if(output_name) { write_fuzzed_model(output_name);
  } else {
    msg("WARNING: No output file given. Nothing will be printed.");
    msg("");
  }

  reset_all();


  print_statistics();

  return 0;
}
