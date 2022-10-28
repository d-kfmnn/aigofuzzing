/*------------------------------------------------------------------------*/
/*! \file parser.h
    \brief contains functions necessary to parse the AIG

  Part of AIGoFuzzing : Mutation-Based AIG Fuzzer
  Copyright(C) 2022 Daniela Kaufmann, Johannes Kepler University Linz
*/
/*------------------------------------------------------------------------*/
#ifndef AIGENFUZZER_SRC_PARSER_H_
#define AIGENFUZZER_SRC_PARSER_H_
/*------------------------------------------------------------------------*/
#include <assert.h>

#include "signal_statistics.h"

extern "C" {
  #include "../includes/aiger.h"
}
/*------------------------------------------------------------------------*/

extern unsigned M;   // /< stores the maximum variable num of the input AIG
extern unsigned I;   // /< stores the maximum variable num of the input AIG
extern unsigned L;   // /< stores the maximum variable num of the input AIG
extern unsigned O;   // /< stores the maximum variable num of the input AIG
extern unsigned A;   // /< stores the maximum variable num of the input AIG

extern aiger * model;
/*------------------------------------------------------------------------*/

/**
    Initializes the 'aiger* model', which is local to aig.cpp
*/
void init_aig_parsing();

/*------------------------------------------------------------------------*/

/**
    Resets the 'aiger* model', which is local to aig.cpp
*/
void reset_aig_parsing();
/*------------------------------------------------------------------------*/
// Functions that interfer with aiger* model, that is used to store the
// input AIG.


/**
    Checks whether the given value corrensponds to an input of 'aiger* model'

    @param i an unsigned integer

    @return true when i is an input in 'aiger* model'.
*/
bool is_model_input(unsigned i);

/**
    Searches for the AIG node with value 'i' in 'aiger* model'

    @param i an unsigned integer

    @return an aiger_and* object, with lefthand side i
*/
aiger_and * get_model_and(unsigned i);

/**
    Returns the value of the i'th input in 'aiger* model'

    @param i an unsigned integer, has to be smaller than NN

    @return an unsigned integer corresponding to the value of the i'th input
*/
unsigned get_model_inputs_lit(unsigned i);

/**
    Returns the name of the i'th input in 'aiger* model'

    @param i an unsigned integer, has to be smaller than NN

    @return a const char * corresponding to the name of the i'th input
*/
const char* get_model_inputs_name(unsigned i);


unsigned get_model_outputs_lit(unsigned i);


/**
    Reads the input aiger given in the file called input_name to the aiger 'model'
    using the parserer function of aiger.h

    @param input_name char * ame of input file
*/
void parse_aig(const char * input_name);


/**
    Writes the 'aiger* model' to the provided file.

    @param file output file

*/
void write_fuzzed_model(const char * output_name);


#endif  // AIGENFUZZER_SRC_PARSER_H_
