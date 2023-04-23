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
#include "max_pooling1d_24.c" // InputLayer is excluded
#include "conv1d_18.c"
#include "weights/conv1d_18.c" // InputLayer is excluded
#include "max_pooling1d_25.c" // InputLayer is excluded
#include "conv1d_19.c"
#include "weights/conv1d_19.c" // InputLayer is excluded
#include "max_pooling1d_26.c" // InputLayer is excluded
#include "conv1d_20.c"
#include "weights/conv1d_20.c" // InputLayer is excluded
#include "max_pooling1d_27.c" // InputLayer is excluded
#include "average_pooling1d_6.c" // InputLayer is excluded
#include "flatten_6.c" // InputLayer is excluded
#include "dense_6.c"
#include "weights/dense_6.c"
#endif

void cnn(
  const number_t input[MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES],
  dense_6_output_type dense_6_output) {

  // Output array allocation
  static union {
    max_pooling1d_24_output_type max_pooling1d_24_output;
    max_pooling1d_25_output_type max_pooling1d_25_output;
    max_pooling1d_26_output_type max_pooling1d_26_output;
    max_pooling1d_27_output_type max_pooling1d_27_output;
  } activations1;

  static union {
    conv1d_18_output_type conv1d_18_output;
    conv1d_19_output_type conv1d_19_output;
    conv1d_20_output_type conv1d_20_output;
    average_pooling1d_6_output_type average_pooling1d_6_output;
    flatten_6_output_type flatten_6_output;
  } activations2;


  //static union {
//
//    static input_7_output_type input_7_output;
//
//    static max_pooling1d_24_output_type max_pooling1d_24_output;
//
//    static conv1d_18_output_type conv1d_18_output;
//
//    static max_pooling1d_25_output_type max_pooling1d_25_output;
//
//    static conv1d_19_output_type conv1d_19_output;
//
//    static max_pooling1d_26_output_type max_pooling1d_26_output;
//
//    static conv1d_20_output_type conv1d_20_output;
//
//    static max_pooling1d_27_output_type max_pooling1d_27_output;
//
//    static average_pooling1d_6_output_type average_pooling1d_6_output;
//
//    static flatten_6_output_type flatten_6_output;
//
  //} activations;

  // Model layers call chain
 // InputLayer is excluded 
  max_pooling1d_24(
     // First layer uses input passed as model parameter
    input,
    activations1.max_pooling1d_24_output
  );
 // InputLayer is excluded 
  conv1d_18(
    
    activations1.max_pooling1d_24_output,
    conv1d_18_kernel,
    conv1d_18_bias,
    activations2.conv1d_18_output
  );
 // InputLayer is excluded 
  max_pooling1d_25(
    
    activations2.conv1d_18_output,
    activations1.max_pooling1d_25_output
  );
 // InputLayer is excluded 
  conv1d_19(
    
    activations1.max_pooling1d_25_output,
    conv1d_19_kernel,
    conv1d_19_bias,
    activations2.conv1d_19_output
  );
 // InputLayer is excluded 
  max_pooling1d_26(
    
    activations2.conv1d_19_output,
    activations1.max_pooling1d_26_output
  );
 // InputLayer is excluded 
  conv1d_20(
    
    activations1.max_pooling1d_26_output,
    conv1d_20_kernel,
    conv1d_20_bias,
    activations2.conv1d_20_output
  );
 // InputLayer is excluded 
  max_pooling1d_27(
    
    activations2.conv1d_20_output,
    activations1.max_pooling1d_27_output
  );
 // InputLayer is excluded 
  average_pooling1d_6(
    
    activations1.max_pooling1d_27_output,
    activations2.average_pooling1d_6_output
  );
 // InputLayer is excluded 
  flatten_6(
    
    activations2.average_pooling1d_6_output,
    activations2.flatten_6_output
  );
 // InputLayer is excluded 
  dense_6(
    
    activations2.flatten_6_output,
    dense_6_kernel,
    dense_6_bias, // Last layer uses output passed as model parameter
    dense_6_output
  );

}
