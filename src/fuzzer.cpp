/*------------------------------------------------------------------------*/
/*! \file fuzzer.cpp
    \brief contains functions necessary to parse the AIG

  Part of AIGoFuzzing : Mutation-Based AIG Fuzzer
  Copyright(C) 2022 Daniela Kaufmann, Johannes Kepler University Linz
*/
/*------------------------------------------------------------------------*/
#include "fuzzer.h"
/*------------------------------------------------------------------------*/
int rangeRandomZeroMax (int max){
    int n = max;
    int remainder = RAND_MAX % n;
    int x;
    do{
        x = rand();
    }while (x >= RAND_MAX - remainder);
    return x % n;
}



static unsigned choose_random_aiger_and(){
   return 2*(rangeRandomZeroMax(A) + I + 1);
}

static unsigned choose_random_aiger_and_max(unsigned max){
   return rangeRandomZeroMax(max);
}

static int choose_random_rhs(){
   return rangeRandomZeroMax(2);
}

static int choose_random_output(){
   return rangeRandomZeroMax(O);
}

/*------------------------------------------------------------------------*/

static void flip_internal_sign_edge(){
  msg("Technique: Flipping internal sign - Edge");
  msg("==========================================================");

  unsigned rand_lit =  choose_random_aiger_and();
  int rhs = choose_random_rhs();

  msg("  selected and node %i", rand_lit);
  msg("  flipping rhs%i", rhs);

  aiger_and * and_node = aiger_is_and (model, rand_lit);
  msg("  old: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);

  if(rhs){
    and_node->rhs1 =  and_node->rhs1 + 1 - 2*aiger_sign(and_node->rhs1);
  } else {
    and_node->rhs0 =  and_node->rhs0 + 1 - 2*aiger_sign(and_node->rhs0);
  }
  msg("  new: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
  msg("");
  msg("");
}
/*------------------------------------------------------------------------*/
static void flip_internal_sign_node(){
  msg("Technique: Flipping internal sign - Node");
  msg("==========================================================");

  unsigned rand_lit =  choose_random_aiger_and();

  for (unsigned i = 0; i < O; i++){
    if (aiger_strip(model->outputs[i].lit) == rand_lit){
      msg("  selected output node %i %i", i, model->outputs[i].lit);
      model->outputs[i].lit =  model->outputs[i].lit^1;
      msg("  new: %i", model->outputs[i].lit);
      msg("");
      msg("");
      return;
    }
  }

  msg("  selected and node %i", rand_lit);

  aiger_and * and_node;
  for(unsigned i = rand_lit/2; i < A; i++){
    and_node = aiger_is_and (model, 2*i);

    if(aiger_strip(and_node->rhs0) == rand_lit){
      msg("  old: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
      and_node->rhs0 =  and_node->rhs0^1;
      msg("  new: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
      msg("");


    } else if (aiger_strip(and_node->rhs1) == rand_lit) {
      msg("  old: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
      and_node->rhs1 =  and_node->rhs1^1;
      msg("  new: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
      msg("");


    }
  }
  msg("");
}
/*------------------------------------------------------------------------*/
void flip_internal_sign(){
  if(!rangeRandomZeroMax(2)) flip_internal_sign_edge();
  else flip_internal_sign_node();
}

/*------------------------------------------------------------------------*/

void flip_output_sign(){
  msg("Technique: Flipping output sign");
  msg("==========================================================");

  unsigned rand_idx =  choose_random_output();

  unsigned rand_lit = model->outputs[rand_idx].lit;

  msg("  selected output node %i %i", rand_idx, rand_lit);

  model->outputs[rand_idx].lit =  rand_lit^1;

  msg("  new: %i", model->outputs[rand_idx].lit);
  msg("");
  msg("");
}

/*------------------------------------------------------------------------*/

void change_input_of_internal_node(){
  msg("Technique: Change input of internal node");
  msg("==========================================================");

  unsigned rand_lit =  choose_random_aiger_and();
  int rhs = choose_random_rhs();
  unsigned rand_input_lit =  choose_random_aiger_and_max(rand_lit);

  assert(rand_input_lit < rand_lit);

  msg("  selected and node %i", rand_lit);
  msg("  changing rhs%i to %i", rhs, rand_input_lit);

  aiger_and * and_node = aiger_is_and (model, rand_lit);
  msg("  old: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);

  if(rhs){
    and_node->rhs1 =  rand_input_lit;
  } else {
    and_node->rhs0 =  rand_input_lit;
  }
  msg("  new: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
  msg("");
  msg("");
}

/*------------------------------------------------------------------------*/
static void and_constant_zero(){
  msg("Technique: Insert constant - ANDs a constant one");
  msg("==========================================================");

  unsigned rand_lit =  choose_random_aiger_and();
  int rhs = choose_random_rhs();

  unsigned new_node = 2*(++M);


  msg("  selected and node %i", rand_lit);

  aiger_and * and_node = aiger_is_and (model, rand_lit);
  msg("  old: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);

  if(rhs){
    unsigned tmp = and_node->rhs1;
    and_node->rhs1 =  new_node;
    aiger_add_and(model, new_node, tmp, 0);
  } else {
    unsigned tmp = and_node->rhs0;
    and_node->rhs0 =  new_node;
    aiger_add_and(model, new_node, tmp, 0);
  }
  msg("  modified: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
  and_node = aiger_is_and (model, new_node);
  msg("  inserted: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
  aiger_reencode(model);

  msg("");
  msg("");
}

/*------------------------------------------------------------------------*/
static void and_constant_one(){
  msg("Technique: Insert constant - ANDs a constant one");
  msg("==========================================================");

  unsigned rand_lit =  choose_random_aiger_and();
  int rhs = choose_random_rhs();

  unsigned new_node = 2*(++M);


  msg("  selected and node %i", rand_lit);

  aiger_and * and_node = aiger_is_and (model, rand_lit);
  msg("  old: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);

  if(rhs){
    unsigned tmp = and_node->rhs1;
    and_node->rhs1 =  new_node;
    aiger_add_and(model, new_node, tmp, 1);
  } else {
    unsigned tmp = and_node->rhs0;
    and_node->rhs0 =  new_node;
    aiger_add_and(model, new_node, tmp, 1);
  }
  msg("  modified: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
  and_node = aiger_is_and (model, new_node);
  msg("  inserted: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
  aiger_reencode(model);

  msg("");
  msg("");
}

/*------------------------------------------------------------------------*/

static void or_constant_zero(){
  msg("Technique: Insert constant - ORs a constant zero");
  msg("==========================================================");

  unsigned rand_lit =  choose_random_aiger_and();
  int rhs = choose_random_rhs();

  unsigned new_node = 2*(++M);

  msg("  selected and node %i", rand_lit);

  aiger_and * and_node = aiger_is_and (model, rand_lit);
  msg("  old: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);

  if(rhs){
    unsigned tmp = and_node->rhs1;
    and_node->rhs1 =  new_node + 1;
    aiger_add_and(model, new_node, tmp^1, 1);
  } else {
    unsigned tmp = and_node->rhs0;
    and_node->rhs0 =  new_node +1;
    aiger_add_and(model, new_node, tmp^1, 1);
  }
  msg("  modified: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
  and_node = aiger_is_and (model, new_node);
  msg("  inserted: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
  aiger_reencode(model);

  msg("");
  msg("");
}

/*------------------------------------------------------------------------*/

static void or_constant_one(){
  msg("Technique: Insert constant - ORs a constant one");
  msg("==========================================================");

  unsigned rand_lit =  choose_random_aiger_and();
  int rhs = choose_random_rhs();

  unsigned new_node = 2*(++M);

  msg("  selected and node %i", rand_lit);

  aiger_and * and_node = aiger_is_and (model, rand_lit);
  msg("  old: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);

  if(rhs){
    unsigned tmp = and_node->rhs1;
    and_node->rhs1 =  new_node + 1;
    aiger_add_and(model, new_node, tmp^1, 0);
  } else {
    unsigned tmp = and_node->rhs0;
    and_node->rhs0 =  new_node +1;
    aiger_add_and(model, new_node, tmp^1, 0);
  }
  msg("  modified: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
  and_node = aiger_is_and (model, new_node);
  msg("  inserted: %i %i %i", and_node->lhs, and_node->rhs0, and_node->rhs1);
  aiger_reencode(model);

  msg("");
  msg("");
}

/*------------------------------------------------------------------------*/
void insert_constant(){
  int mod = rangeRandomZeroMax(4);

  switch(mod){
    case 0:
      and_constant_zero();
      break;
    case 1:
      and_constant_one();
      break;
    case 2:
      or_constant_zero();
      break;
    case 3:
      or_constant_one();
      break;
  }
}
