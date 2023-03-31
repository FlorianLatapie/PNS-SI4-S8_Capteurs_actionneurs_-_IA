/**
  ******************************************************************************
  * @file    model.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "number.h"
#include "model.h"

 // InputLayer is excluded
#include "conv1d_1.c"
#include "weights/conv1d_1.c" // InputLayer is excluded
#include "flatten_1.c" // InputLayer is excluded
#include "dense_1.c"
#include "weights/dense_1.c"
#endif

void cnn(
  const number_t input[MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES],
  dense_1_output_type dense_1_output) {

  // Output array allocation
  static union {
    conv1d_1_output_type conv1d_1_output;
    flatten_1_output_type flatten_1_output;
  } activations1;


  //static union {
//
//    static input_2_output_type input_2_output;
//
//    static conv1d_1_output_type conv1d_1_output;
//
//    static flatten_1_output_type flatten_1_output;
//
  //} activations;

  // Model layers call chain
 // InputLayer is excluded 
  conv1d_1(
     // First layer uses input passed as model parameter
    input,
    conv1d_1_kernel,
    conv1d_1_bias,
    activations1.conv1d_1_output
  );
 // InputLayer is excluded 
  flatten_1(
    
    activations1.conv1d_1_output,
    activations1.flatten_1_output
  );
 // InputLayer is excluded 
  dense_1(
    
    activations1.flatten_1_output,
    dense_1_kernel,
    dense_1_bias, // Last layer uses output passed as model parameter
    dense_1_output
  );

}
