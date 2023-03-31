/**
  ******************************************************************************
  * @file    weights/conv.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#define INPUT_CHANNELS    3
#define CONV_FILTERS      2
#define CONV_KERNEL_SIZE  3


const int16_t conv1d_1_bias[CONV_FILTERS] = {0, 0}
;

const int16_t conv1d_1_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{-176, 296, -119}
, {234, -129, -9}
, {-220, 26, 114}
}
, {{91, -169, -200}
, {-301, 284, 56}
, {-159, -154, 135}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE