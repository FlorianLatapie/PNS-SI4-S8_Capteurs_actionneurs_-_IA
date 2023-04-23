/**
  ******************************************************************************
  * @file    weights/fc.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#define INPUT_SAMPLES 32
#define FC_UNITS 3


const int16_t dense_15_bias[FC_UNITS] = {3, -2, -2}
;

const int16_t dense_15_kernel[FC_UNITS][INPUT_SAMPLES] = {{112, -215, -133, 23, 92, 71, 185, 137, -226, -103, -121, 169, -74, 44, 181, -153, -173, -77, 155, 45, -70, 90, 201, -23, -139, 130, -213, 137, 62, -1, -17, 189}
, {-212, -95, -48, 167, 177, -90, 145, 41, -67, 188, -2, 30, 216, -11, -25, 98, 198, 51, 94, 72, -126, 204, -213, 137, 58, -179, 215, 25, -186, -175, 98, -104}
, {-136, -35, -177, 251, 217, 121, -209, 198, 196, -198, 78, 95, 113, -253, 57, 202, -75, 123, 59, 23, -1, -62, 124, -76, 50, 124, 30, -227, 182, -147, 48, -141}
}
;

#undef INPUT_SAMPLES
#undef FC_UNITS