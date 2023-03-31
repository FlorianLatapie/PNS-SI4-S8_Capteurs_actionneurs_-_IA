/**
  ******************************************************************************
  * @file    weights/fc.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#define INPUT_SAMPLES 60
#define FC_UNITS 2


const int16_t dense_1_bias[FC_UNITS] = {1, -1}
;

const int16_t dense_1_kernel[FC_UNITS][INPUT_SAMPLES] = {{-88, 18, 79, 132, -129, -143, 101, -48, -22, -24, 52, 68, 121, 110, 18, -47, 73, 21, 78, -104, -5, -132, -3, -4, -121, 75, 127, -149, 58, -57, 136, 19, 62, -16, 0, -119, 70, -58, 43, -31, 72, 132, -67, 79, 105, 134, 135, -106, 102, -15, -113, -106, 40, -44, -102, -12, -137, -132, -74, 60}
, {-16, 97, 62, -36, -92, 118, -24, -65, 31, 74, -38, -113, 56, 61, -56, 153, -54, 89, 19, -29, 83, 143, 145, -133, 52, 31, 141, -22, -15, -132, 40, 0, -80, -120, 69, 15, 70, -101, 149, -80, 38, -7, 67, 11, -19, 109, 108, -56, 25, -28, -19, 34, -92, -59, 44, 98, 82, 104, -105, 140}
}
;

#undef INPUT_SAMPLES
#undef FC_UNITS