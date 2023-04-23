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
#include "conv1d_81.c"
#include "weights/conv1d_81.c" // InputLayer is excluded
#include "max_pooling1d_66.c" // InputLayer is excluded
#include "conv1d_82.c"
#include "weights/conv1d_82.c" // InputLayer is excluded
#include "max_pooling1d_67.c" // InputLayer is excluded
#include "conv1d_83.c"
#include "weights/conv1d_83.c" // InputLayer is excluded
#include "max_pooling1d_68.c" // InputLayer is excluded
#include "flatten_43.c" // InputLayer is excluded
#include "dense_27.c"
#include "weights/dense_27.c"
#endif

void cnn(
  const number_t input[MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES],
  dense_27_output_type dense_27_output) {

  // Output array allocation
  static union {
    conv1d_81_output_type conv1d_81_output;
    conv1d_82_output_type conv1d_82_output;
    conv1d_83_output_type conv1d_83_output;
  } activations1;

  static union {
    max_pooling1d_66_output_type max_pooling1d_66_output;
    max_pooling1d_67_output_type max_pooling1d_67_output;
    max_pooling1d_68_output_type max_pooling1d_68_output;
    flatten_43_output_type flatten_43_output;
  } activations2;


  //static union {
//
//    static input_28_output_type input_28_output;
//
//    static conv1d_81_output_type conv1d_81_output;
//
//    static max_pooling1d_66_output_type max_pooling1d_66_output;
//
//    static conv1d_82_output_type conv1d_82_output;
//
//    static max_pooling1d_67_output_type max_pooling1d_67_output;
//
//    static conv1d_83_output_type conv1d_83_output;
//
//    static max_pooling1d_68_output_type max_pooling1d_68_output;
//
//    static flatten_43_output_type flatten_43_output;
//
  //} activations;

  // Model layers call chain
 // InputLayer is excluded 
  conv1d_81(
     // First layer uses input passed as model parameter
    input,
    conv1d_81_kernel,
    conv1d_81_bias,
    activations1.conv1d_81_output
  );
 // InputLayer is excluded 
  max_pooling1d_66(
    
    activations1.conv1d_81_output,
    activations2.max_pooling1d_66_output
  );
 // InputLayer is excluded 
  conv1d_82(
    
    activations2.max_pooling1d_66_output,
    conv1d_82_kernel,
    conv1d_82_bias,
    activations1.conv1d_82_output
  );
 // InputLayer is excluded 
  max_pooling1d_67(
    
    activations1.conv1d_82_output,
    activations2.max_pooling1d_67_output
  );
 // InputLayer is excluded 
  conv1d_83(
    
    activations2.max_pooling1d_67_output,
    conv1d_83_kernel,
    conv1d_83_bias,
    activations1.conv1d_83_output
  );
 // InputLayer is excluded 
  max_pooling1d_68(
    
    activations1.conv1d_83_output,
    activations2.max_pooling1d_68_output
  );
 // InputLayer is excluded 
  flatten_43(
    
    activations2.max_pooling1d_68_output,
    activations2.flatten_43_output
  );
 // InputLayer is excluded 
  dense_27(
    
    activations2.flatten_43_output,
    dense_27_kernel,
    dense_27_bias, // Last layer uses output passed as model parameter
    dense_27_output
  );

}
