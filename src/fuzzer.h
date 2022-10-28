/*------------------------------------------------------------------------*/
/*! \file fuzzer.h
    \brief contains functions necessary to parse the AIG

  Part of AIGoFuzzing : Mutation-Based AIG Fuzzer
  Copyright(C) 2022 Daniela Kaufmann, Johannes Kepler University Linz
*/
/*------------------------------------------------------------------------*/
#ifndef AIGENFUZZER_SRC_FUZZER_H_
#define AIGENFUZZER_SRC_FUZZER_H_
/*------------------------------------------------------------------------*/
#include "parser.h"

int rangeRandomZeroMax (int max);

/**
    Flips a single sign in an AND gate
*/
void flip_internal_sign();


/**
    Flips a single sign of an output gate
*/
void flip_output_sign();


/**
    Randomly change input signals of intenral nodes
*/
void change_input_of_internal_node();


/**
    Randomly insert a constant via and or or
*/
void insert_constant();



#endif  // AIGENFUZZER_SRC_FUZZER_H_
