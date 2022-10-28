/*------------------------------------------------------------------------*/
/*! \file parser.cpp
    \brief contains functions necessary to parse the AIG

  Part of AIGoFuzzing : Mutation-Based AIG Fuzzer
  Copyright(C) 2022 Daniela Kaufmann, Johannes Kepler University Linz
*/
/*------------------------------------------------------------------------*/
#include "parser.h"
/*------------------------------------------------------------------------*/
// Global Variables
unsigned M, I, L, O, A;

/*------------------------------------------------------------------------*/
// Local Variables

aiger * model;   // /< aiger* object, used for storing the given AIG graph

/*------------------------------------------------------------------------*/
static unsigned get_model_num_latches() {return model->num_latches;}
static unsigned get_model_num_inputs()  {return model->num_inputs;}
static unsigned get_model_num_ands()    {return model->num_ands;}
static unsigned get_model_num_outputs() {return model->num_outputs;}
static unsigned get_model_maxvar()      {return model->maxvar;}

/*------------------------------------------------------------------------*/
void init_aig_parsing() {
  assert(!model);
  model = aiger_init();
  assert(model);
}
/*------------------------------------------------------------------------*/
void reset_aig_parsing() {
  assert(model);
  aiger_reset(model);
}
/*------------------------------------------------------------------------*/

bool is_model_input(unsigned i) {
  assert(model);
  assert(i < I);
  return aiger_is_input(model, i);
}
/*------------------------------------------------------------------------*/
aiger_and * get_model_and(unsigned i) {
  assert(model);
  assert(i < A);
  return aiger_is_and(model, i);
}
/*------------------------------------------------------------------------*/

unsigned get_model_inputs_lit(unsigned i) {
  assert(model);
  assert(i < I);
  return model->inputs[i].lit;
}
/*------------------------------------------------------------------------*/
const char* get_model_inputs_name(unsigned i) {
  assert(model);
  assert(i < I);
  return model->inputs[i].name;
}
/*------------------------------------------------------------------------*/
unsigned get_model_outputs_lit(unsigned i) {
  assert(model);
  assert(i < O);
  return model->outputs[i].lit;
}
/*------------------------------------------------------------------------*/
const char* get_model_outputs_name(unsigned i) {
  assert(model);
  assert(i < O);
  return model->outputs[i].name;
}
/*------------------------------------------------------------------------*/
/**
    Checks whether the input AIG fullfills requirements.
*/
void check_aiger_model() {

  M = get_model_maxvar();
  I = get_model_num_inputs();
  L = get_model_num_latches();
  O = get_model_num_outputs();
  A = get_model_num_ands();

  if (L) die("unexpected behaviour: AIGoFuzzing can not handle latches yet");
  if (!I) die("unexpected behaviour: model contains no inputs");
  if (!O) die("unexpected behaviour: model contains no outputs");

  msg("  MILOA:           %u %u %u %u %u", M, I, L, O, A);
  msg("");
  msg("");
}

/*------------------------------------------------------------------------*/

void parse_aig(const char * input_name) {
  assert(input_name);
  init_aig_parsing();

  msg("  Input File:      '%s'", input_name);
  const char * err = aiger_open_and_read_from_file(model, input_name);
  if (err) die("error parsing '%s': %s", input_name, err);

  check_aiger_model();
  assert(model);
}

/*=========================================================================*/

void write_fuzzed_model(const char * output_name) {

  FILE * output_file;
  if (!(output_file = fopen(output_name, "w")))
      die("can not write output to '%s'", output_name);

  if (!aiger_write_to_file(model, aiger_binary_mode, output_file))
        die("failed to write rewritten aig to '%s'", output_name);

  msg("Output");
  msg("==========================================================");
  msg("  Printed fuzzed AIG to: '%s'", output_name);
  msg("");

}
/*------------------------------------------------------------------------*/
