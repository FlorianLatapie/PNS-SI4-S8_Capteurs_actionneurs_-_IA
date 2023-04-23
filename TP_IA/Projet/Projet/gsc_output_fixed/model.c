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
#include "max_pooling1d_60.c" // InputLayer is excluded
#include "conv1d_45.c"
#include "weights/conv1d_45.c" // InputLayer is excluded
#include "max_pooling1d_61.c" // InputLayer is excluded
#include "conv1d_46.c"
#include "weights/conv1d_46.c" // InputLayer is excluded
#include "max_pooling1d_62.c" // InputLayer is excluded
#include "conv1d_47.c"
#include "weights/conv1d_47.c" // InputLayer is excluded
#include "max_pooling1d_63.c" // InputLayer is excluded
#include "average_pooling1d_15.c" // InputLayer is excluded
#include "flatten_15.c" // InputLayer is excluded
#include "dense_15.c"
#include "weights/dense_15.c"
#endif

void cnn(
  const number_t input[MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES],
  dense_15_output_type dense_15_output) {

  // Output array allocation
  static union {
    max_pooling1d_60_output_type max_pooling1d_60_output;
    max_pooling1d_61_output_type max_pooling1d_61_output;
    max_pooling1d_62_output_type max_pooling1d_62_output;
    max_pooling1d_63_output_type max_pooling1d_63_output;
  } activations1;

  static union {
    conv1d_45_output_type conv1d_45_output;
    conv1d_46_output_type conv1d_46_output;
    conv1d_47_output_type conv1d_47_output;
    average_pooling1d_15_output_type average_pooling1d_15_output;
    flatten_15_output_type flatten_15_output;
  } activations2;


  //static union {
//
//    static input_16_output_type input_16_output;
//
//    static max_pooling1d_60_output_type max_pooling1d_60_output;
//
//    static conv1d_45_output_type conv1d_45_output;
//
//    static max_pooling1d_61_output_type max_pooling1d_61_output;
//
//    static conv1d_46_output_type conv1d_46_output;
//
//    static max_pooling1d_62_output_type max_pooling1d_62_output;
//
//    static conv1d_47_output_type conv1d_47_output;
//
//    static max_pooling1d_63_output_type max_pooling1d_63_output;
//
//    static average_pooling1d_15_output_type average_pooling1d_15_output;
//
//    static flatten_15_output_type flatten_15_output;
//
  //} activations;

  // Model layers call chain
 // InputLayer is excluded 
  max_pooling1d_60(
     // First layer uses input passed as model parameter
    input,
    activations1.max_pooling1d_60_output
  );
 // InputLayer is excluded 
  conv1d_45(
    
    activations1.max_pooling1d_60_output,
    conv1d_45_kernel,
    conv1d_45_bias,
    activations2.conv1d_45_output
  );
 // InputLayer is excluded 
  max_pooling1d_61(
    
    activations2.conv1d_45_output,
    activations1.max_pooling1d_61_output
  );
 // InputLayer is excluded 
  conv1d_46(
    
    activations1.max_pooling1d_61_output,
    conv1d_46_kernel,
    conv1d_46_bias,
    activations2.conv1d_46_output
  );
 // InputLayer is excluded 
  max_pooling1d_62(
    
    activations2.conv1d_46_output,
    activations1.max_pooling1d_62_output
  );
 // InputLayer is excluded 
  conv1d_47(
    
    activations1.max_pooling1d_62_output,
    conv1d_47_kernel,
    conv1d_47_bias,
    activations2.conv1d_47_output
  );
 // InputLayer is excluded 
  max_pooling1d_63(
    
    activations2.conv1d_47_output,
    activations1.max_pooling1d_63_output
  );
 // InputLayer is excluded 
  average_pooling1d_15(
    
    activations1.max_pooling1d_63_output,
    activations2.average_pooling1d_15_output
  );
 // InputLayer is excluded 
  flatten_15(
    
    activations2.average_pooling1d_15_output,
    activations2.flatten_15_output
  );
 // InputLayer is excluded 
  dense_15(
    
    activations2.flatten_15_output,
    dense_15_kernel,
    dense_15_bias, // Last layer uses output passed as model parameter
    dense_15_output
  );

}
