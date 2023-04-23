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
#include "max_pooling1d_32.c" // InputLayer is excluded
#include "conv1d_24.c"
#include "weights/conv1d_24.c" // InputLayer is excluded
#include "max_pooling1d_33.c" // InputLayer is excluded
#include "conv1d_25.c"
#include "weights/conv1d_25.c" // InputLayer is excluded
#include "max_pooling1d_34.c" // InputLayer is excluded
#include "conv1d_26.c"
#include "weights/conv1d_26.c" // InputLayer is excluded
#include "max_pooling1d_35.c" // InputLayer is excluded
#include "average_pooling1d_8.c" // InputLayer is excluded
#include "flatten_8.c" // InputLayer is excluded
#include "dense_8.c"
#include "weights/dense_8.c"
#endif

void cnn(
  const number_t input[MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES],
  dense_8_output_type dense_8_output) {

  // Output array allocation
  static union {
    max_pooling1d_32_output_type max_pooling1d_32_output;
    max_pooling1d_33_output_type max_pooling1d_33_output;
    max_pooling1d_34_output_type max_pooling1d_34_output;
    max_pooling1d_35_output_type max_pooling1d_35_output;
  } activations1;

  static union {
    conv1d_24_output_type conv1d_24_output;
    conv1d_25_output_type conv1d_25_output;
    conv1d_26_output_type conv1d_26_output;
    average_pooling1d_8_output_type average_pooling1d_8_output;
    flatten_8_output_type flatten_8_output;
  } activations2;


  //static union {
//
//    static input_9_output_type input_9_output;
//
//    static max_pooling1d_32_output_type max_pooling1d_32_output;
//
//    static conv1d_24_output_type conv1d_24_output;
//
//    static max_pooling1d_33_output_type max_pooling1d_33_output;
//
//    static conv1d_25_output_type conv1d_25_output;
//
//    static max_pooling1d_34_output_type max_pooling1d_34_output;
//
//    static conv1d_26_output_type conv1d_26_output;
//
//    static max_pooling1d_35_output_type max_pooling1d_35_output;
//
//    static average_pooling1d_8_output_type average_pooling1d_8_output;
//
//    static flatten_8_output_type flatten_8_output;
//
  //} activations;

  // Model layers call chain
 // InputLayer is excluded 
  max_pooling1d_32(
     // First layer uses input passed as model parameter
    input,
    activations1.max_pooling1d_32_output
  );
 // InputLayer is excluded 
  conv1d_24(
    
    activations1.max_pooling1d_32_output,
    conv1d_24_kernel,
    conv1d_24_bias,
    activations2.conv1d_24_output
  );
 // InputLayer is excluded 
  max_pooling1d_33(
    
    activations2.conv1d_24_output,
    activations1.max_pooling1d_33_output
  );
 // InputLayer is excluded 
  conv1d_25(
    
    activations1.max_pooling1d_33_output,
    conv1d_25_kernel,
    conv1d_25_bias,
    activations2.conv1d_25_output
  );
 // InputLayer is excluded 
  max_pooling1d_34(
    
    activations2.conv1d_25_output,
    activations1.max_pooling1d_34_output
  );
 // InputLayer is excluded 
  conv1d_26(
    
    activations1.max_pooling1d_34_output,
    conv1d_26_kernel,
    conv1d_26_bias,
    activations2.conv1d_26_output
  );
 // InputLayer is excluded 
  max_pooling1d_35(
    
    activations2.conv1d_26_output,
    activations1.max_pooling1d_35_output
  );
 // InputLayer is excluded 
  average_pooling1d_8(
    
    activations1.max_pooling1d_35_output,
    activations2.average_pooling1d_8_output
  );
 // InputLayer is excluded 
  flatten_8(
    
    activations2.average_pooling1d_8_output,
    activations2.flatten_8_output
  );
 // InputLayer is excluded 
  dense_8(
    
    activations2.flatten_8_output,
    dense_8_kernel,
    dense_8_bias, // Last layer uses output passed as model parameter
    dense_8_output
  );

}
