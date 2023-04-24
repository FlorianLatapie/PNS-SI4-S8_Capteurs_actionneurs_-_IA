/**
  ******************************************************************************
  * @file    weights/conv.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#define INPUT_CHANNELS    1
#define CONV_FILTERS      32
#define CONV_KERNEL_SIZE  80


const int16_t conv1d_6_bias[CONV_FILTERS] = {0, -54, -4, 0, 0, 0, 15, 14, 0, 11, -48, 4, 0, 4, -32, -18, 19, 46, 7, 13, 0, 13, 0, 0, -6, -10, -19, 1, -31, 0, 11, -52}
;

const int16_t conv1d_6_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{10, 12, 3, 14, 3, -25, 18, -17, -15, -8, -7, -2, -9, -16, 17, 7, 10, -32, -2, 0, -28, -25, 20, 24, -3, 13, -18, -41, 2, 7, 13, 2, 22, -3, -46, -39, -5, 24, 30, 22, 18, -26, 2, -16, -25, -16, 10, 6, 0, -14, 21, -6, 25, 7, -8, -14, 10, 10, 6, 14, 18, 17, -11, 2, -1, -3, 10, 8, -5, -14, 17, 6, 0, 1, 0, -12, 21, -4, 15, 9}
}
, {{27, 24, -8, 2, -17, 4, -20, -3, -9, -4, -23, 18, -23, -11, -29, -12, 12, 16, -14, -17, 20, 16, 11, -22, 7, 11, 14, 19, -9, 5, 11, 0, -5, -32, -18, 12, -19, -27, 18, 11, 9, -22, -22, -2, 2, 10, -17, -13, 18, -1, -8, -4, -28, -23, -22, 9, 19, -11, 2, 26, -18, 4, 6, 23, 13, 23, 19, 13, -4, -18, -28, -19, -5, 12, -19, 16, -6, -8, -7, -28}
}
, {{15, 11, -10, -12, -15, 14, 16, -18, -2, -20, 2, -18, -19, -1, -11, -12, 4, -19, 3, 20, -13, -3, -1, 15, 22, -10, 20, -20, 14, 12, -12, -5, 13, 3, 1, -18, -15, -27, 10, 8, -29, -10, 7, 7, 11, 17, -9, -16, 15, 17, -13, -9, 18, 11, 4, 23, 0, -8, -15, 20, -2, -28, 7, -24, 5, -20, 9, 11, -10, 11, -18, -14, -15, 5, -17, -5, -18, 2, -9, 19}
}
, {{-21, 0, 10, -7, -1, 4, -30, 7, 16, 4, 20, 24, 8, -40, -39, 13, 3, 11, -15, 8, -29, 0, 17, 3, 3, 8, -17, -2, -12, 19, 10, -8, -21, -18, 12, 0, 11, 11, -29, -3, -18, -16, 28, 26, -6, 8, 4, 1, -11, 10, 0, 13, 13, -24, 3, -4, 6, -10, 17, 14, 20, -31, -14, 1, 25, 7, 4, -21, 11, -4, -9, 2, -6, 5, 15, -22, 16, -13, -7, 17}
}
, {{5, 9, -4, -11, 17, -24, 17, 26, -20, 6, -5, 9, -21, 7, -7, 0, 15, 22, 3, 1, -22, -28, -15, 10, 20, 17, 32, 27, -18, -29, -23, -20, 23, 4, 13, -5, 2, -20, 5, 0, 22, 22, -19, 19, 17, 0, -4, 6, -18, -11, 9, -16, -5, 28, 9, -7, 19, -8, -27, 2, 2, -1, 10, -34, -28, 10, 7, 15, -19, -5, 20, -11, -9, -18, -16, -20, 0, 23, 2, -17}
}
, {{-9, 12, 21, -12, -24, -30, 2, 19, 4, 21, 12, -16, 12, -24, 19, -13, 13, -21, 8, -8, -5, 19, -8, -5, 18, -27, 0, -7, 2, 16, 27, 18, -32, -33, -18, 32, 11, 27, -2, -37, -14, 14, 33, 29, -24, 7, -24, -2, 28, 8, -6, -25, 23, -2, -13, -1, -7, -2, -3, 14, -13, 6, 9, -22, -18, -15, 11, 16, 22, 1, -19, 16, 0, 8, -15, -14, 4, 0, 20, -15}
}
, {{-48, -22, -46, -19, -39, -26, -19, -14, -28, -16, -22, -20, 7, -19, -15, 0, -2, -21, -11, -25, -27, -14, 4, -19, -18, -14, -2, -14, -18, 15, -9, -31, 11, 0, -2, -19, 0, -15, 0, 1, -4, -27, -22, -28, -21, -14, -15, 0, -42, -18, -2, -19, -34, -1, -16, 11, -26, 8, -13, -19, 0, 5, -18, 2, -4, -12, -2, -2, 7, -32, -11, -22, -39, -10, 5, -40, -16, -39, -23, -33}
}
, {{11, -11, -30, 8, 25, -13, 21, 19, 9, -25, -6, -13, 25, 20, 13, 6, -11, -17, 1, 1, 16, 0, 21, 14, -4, -14, 26, 18, 4, -9, 30, -10, -7, 9, -14, 26, 10, 21, -5, -15, -6, -3, -2, 0, 5, -18, 8, -5, -24, -23, 15, 3, -11, -2, 12, -8, 5, -18, -17, -24, -19, 6, 1, -33, -28, -8, -10, -16, -9, -6, -35, -6, -34, -30, -22, -23, -22, -20, -26, -3}
}
, {{-31, -11, -25, 13, 2, 17, -17, 6, -37, 14, -6, 14, 0, 14, -39, -3, 11, 18, 24, -2, 5, -36, -28, 14, 18, 1, 24, 0, -13, 13, -21, -15, 17, -5, 7, -27, -5, 2, 22, 21, 9, -22, 7, -12, -11, -9, -1, -21, 14, 0, 7, 26, 26, 14, 8, -15, -26, 1, 5, 29, -17, 6, 11, 17, -36, -30, 9, 21, 27, -8, -13, 11, -15, -34, 4, 3, 22, 2, 20, -29}
}
, {{-23, -1, 6, -13, 27, 12, -6, -5, -6, 24, -5, 10, -25, -1, -6, -11, -24, 13, 9, 18, 1, -18, 4, -6, -7, 11, -23, -1, -10, -13, 0, 13, 25, -5, -16, -24, -26, 18, 4, 27, -16, 13, 1, -27, -43, -20, -5, -28, -11, 23, 6, 19, 28, 10, -25, 2, -25, -38, -34, -25, 16, -3, 28, 15, -6, 21, 7, -12, 8, -22, -10, -3, -30, -10, 20, -13, 1, 13, -12, 7}
}
, {{-9, 6, 13, -1, -10, 10, -22, 18, 0, 9, -24, -21, 19, -8, -14, 1, 0, -4, 15, -1, 21, -23, -3, -16, 0, 14, 10, -14, -26, -2, -29, -4, -1, 0, -24, 21, -6, -24, 8, -16, -10, 10, 0, -16, -5, 7, 0, 8, 8, -20, -4, 5, 22, 0, 12, -24, -8, -22, 2, -15, -18, -2, -10, -15, 3, 0, 0, -18, 8, -16, 9, -9, 19, 3, -9, 17, 22, -5, -10, -23}
}
, {{-11, -8, 0, -26, 10, 5, -12, 21, 21, -5, -19, -26, 1, -28, 6, 18, -1, 25, -9, -1, 0, -15, -12, -18, -2, -3, 21, -3, -9, 15, -18, -16, 17, 26, -7, -15, 19, -30, -27, -20, 18, 26, 31, -8, 18, -17, -15, -33, -9, 17, 12, -8, 20, 12, -23, 16, 13, -20, -25, -16, 14, -20, 9, 7, 8, -17, -9, 27, 11, -1, 13, -32, -27, -27, 24, 7, 15, 24, -5, -15}
}
, {{-7, 8, 10, 3, -15, -10, -14, 2, -14, -3, -8, -5, -10, 3, -2, 6, 1, 12, 25, 25, 1, -11, -40, 13, 26, 10, 25, 10, -34, -24, -9, -6, 30, 11, -4, -26, -18, 11, -14, 10, -1, 10, -10, -7, 17, 21, -17, -14, -27, 16, -21, 20, -5, 12, 4, -8, -6, 10, -1, 4, 13, -9, 20, -24, -34, -3, 28, 16, 14, 31, -32, -43, 11, 12, 12, 36, 28, -30, -20, -3}
}
, {{-31, -4, -27, -6, -9, -36, -29, 6, -16, -18, 6, 5, -12, 12, -2, 3, 3, -9, 9, -16, -11, 11, -11, 5, -12, -10, 26, -4, -10, -3, 11, 29, 13, 12, 14, 10, 15, -25, 8, -20, 6, -1, -2, -21, -34, -10, 15, -18, -15, -3, 4, 16, 30, -2, 32, 19, 16, 24, -8, 8, 2, -12, -4, -30, -38, -46, -11, -16, -36, -17, -14, -6, 0, 15, -2, 22, -2, -18, -10, -16}
}
, {{-4, 9, -15, 19, 14, 4, -19, -4, -23, -14, 29, 25, -11, 11, -26, 16, 0, 3, 2, 24, -17, 7, 9, -6, 30, 0, 8, -16, 0, -11, 20, -5, -12, 22, 3, -8, 12, -4, 1, 25, 23, 15, 4, -6, 3, 3, 6, 13, 15, -2, -28, 19, 21, 10, 17, 3, -15, -13, 20, 16, 29, -12, -2, 23, 3, -9, 15, -21, -11, 2, 29, -15, -2, -18, -20, 29, -4, 12, 11, -24}
}
, {{13, -28, 9, -15, -12, -3, -1, -10, -15, 16, -4, -6, 12, 4, 13, 20, 10, 11, 21, -1, -27, 0, 22, -5, 14, -17, -24, -1, 24, 25, -7, 23, 8, -26, 0, 23, 17, -2, 23, -9, -4, 12, 6, 7, 25, -9, -10, -28, -10, 4, 0, 22, 3, -8, 4, -7, 12, 18, 4, -11, 0, 2, 4, -4, -17, -31, 3, -19, 26, -16, -2, 17, -25, 8, -14, 20, -27, 10, -7, 6}
}
, {{8, -13, -23, -22, -15, -22, -5, -23, -24, 9, -28, 8, -23, 1, 8, -8, -27, -10, -27, -33, -33, -20, -7, 1, -6, -12, 18, -12, 12, 13, -6, -5, -10, -10, -29, -9, -23, -11, 20, -10, 9, 24, 24, 8, 11, 16, -5, -6, -27, 0, -23, -43, -33, -34, 4, 13, -18, 1, 14, -7, -24, 17, 4, -20, -20, -3, -20, 7, -5, -14, -13, 26, 23, 1, -4, 12, 11, -13, 0, 1}
}
, {{3, -22, 0, -8, 0, -22, -8, 3, -16, 0, -17, -2, -18, -19, -19, 6, -19, -6, -13, -33, -4, 0, -11, -6, 5, -22, -24, -9, -1, -7, -9, -6, 3, -15, -24, -8, 16, -16, -17, -3, -14, -16, -10, -18, 0, -7, -2, 15, -8, -35, -16, 18, -15, 1, -23, 6, -21, 7, 9, -5, 5, -5, 8, -1, -3, 19, -17, 2, 5, -4, -12, -7, -1, 0, -2, 4, -27, -3, -18, -13}
}
, {{13, 24, -7, 1, -25, -25, -2, -25, 23, 3, 0, 11, -6, 12, -20, 17, -7, -8, -14, 13, 18, 9, -6, -32, 7, 13, 2, 5, -6, 6, 28, -21, -16, 11, -24, -16, 21, 25, -13, 10, 10, -22, -41, -7, 10, 21, 27, 15, 8, -22, -3, -1, -14, 19, -2, 4, 1, -11, -20, 28, 5, 32, 0, 3, -44, -40, 8, -2, 20, 6, 4, -28, -23, 20, 34, -4, 23, -2, -30, -24}
}
, {{-5, -7, 12, -12, -6, 21, 8, -11, -11, 5, -4, 31, 27, -7, -23, -17, 10, 6, 12, -24, -19, 5, 0, -9, 26, 23, -29, 6, -1, -20, 19, 8, 6, 2, -2, -8, -22, 14, -16, 21, -11, 7, 2, -42, 16, 14, 0, 21, 13, 13, -16, -26, 3, -9, -14, 13, -4, 14, -14, -28, 15, -18, 23, 19, -3, -9, -10, -17, 4, -16, 31, 28, -20, -19, 16, 3, -5, 15, -9, -17}
}
, {{-18, -27, 8, 12, 0, -7, 0, -22, 0, 23, -5, 4, -13, 5, 2, 2, 9, 10, 5, -22, 27, 26, -6, 17, 1, -43, -20, 15, 21, -10, 12, -16, -8, 15, -1, -3, -16, -2, -16, 28, 23, 5, -6, -5, -14, 6, 22, 17, 22, -8, -20, -16, 15, 33, -6, 11, 14, 3, -37, -14, 15, -1, 11, -14, -7, -10, -2, 18, 30, 19, 25, 28, -21, -25, -16, 35, 23, 18, 3, 9}
}
, {{17, 7, 2, 8, 8, 9, 16, 13, 9, -25, -24, -13, 30, 21, 10, 24, -27, -16, -17, 5, -3, 15, -5, 8, -2, 3, 10, 7, -18, 0, 8, 14, -3, -20, -20, -10, 3, 17, 5, -20, -7, 12, -39, -41, -14, -1, -7, 2, 2, 9, 26, -13, -25, -43, 3, -25, 3, -9, 25, 5, -42, -10, 20, 19, 27, 22, -3, -20, -43, -44, -19, 18, 14, 10, 16, 6, -41, 6, -28, 21}
}
, {{-20, 12, 12, 18, 24, -13, 6, 15, -33, -11, 27, 16, 10, 15, -18, -24, -3, 25, -13, -18, -18, 0, -17, -4, -15, 7, 6, -4, -1, -19, 4, 15, 6, 22, 7, 11, -15, -17, 25, 21, -24, -12, 19, 16, -8, 9, -23, -26, 11, 19, 27, -8, 11, -20, -22, 24, 0, 3, -6, -1, -25, -22, 15, 23, 4, -6, -8, -8, 8, 10, -4, 25, -16, -35, -22, 1, 25, 32, 6, -21}
}
, {{-16, 18, -25, -13, -2, -9, 12, -19, 18, 14, -13, -8, -11, 26, -24, -4, -9, -5, 22, 27, -31, -20, 1, 16, 2, 12, -19, 0, -34, 13, 10, 14, -10, 20, 2, -1, -20, -28, -6, 12, 27, 6, 12, -11, -23, 0, -10, -4, 18, 18, 3, 0, 21, 4, -12, 23, 9, -13, 14, -8, -5, -5, 10, 0, -1, 27, -18, -24, -11, 4, 24, 38, -13, 4, -27, -23, 2, 27, 5, -9}
}
, {{18, -8, -27, -27, -32, -22, -37, -23, 8, 30, 23, 27, 12, 14, 3, -21, -33, -28, -8, 2, 24, 23, 27, 30, -3, 19, -3, 0, -15, -10, -25, 2, -14, -37, 1, -21, -30, -16, 0, -3, 23, 20, 4, 3, -11, 13, -2, 9, 6, 25, 4, -2, -32, 3, -28, -15, -25, 8, -14, -15, -29, -2, -25, 5, 23, 33, 37, 40, 41, 2, 0, -23, -28, -4, -32, -29, 17, -13, -20, -18}
}
, {{-11, 24, 23, 29, 0, 3, 0, 2, -5, -1, -9, 1, 18, 26, 12, -8, 5, 30, -11, -6, 10, 0, 13, 5, -1, 11, 10, 7, -12, -7, 17, 21, 18, -2, -3, 20, -4, 12, 17, -19, -2, 0, -14, 11, 9, 6, 22, 1, 16, 4, 8, 15, 7, 22, 22, 3, 20, 1, 18, 25, 14, 7, 27, -8, 14, 22, 8, 11, 19, 10, 1, -6, -7, 10, 21, -8, 7, 19, -11, 24}
}
, {{-16, -31, -27, -16, -12, 2, 26, 25, 32, -1, 30, -8, 26, -8, -22, 10, -27, -19, -21, -43, -43, -46, -16, -37, -22, -20, 10, 28, -2, 22, 9, 21, 21, -4, 1, 5, -8, 9, -1, 7, 2, 13, -21, -17, -23, -35, -41, -2, -11, -21, -17, 23, 34, 10, 27, 30, 21, -2, -3, -23, -23, -15, -8, -26, -24, 0, -24, 2, -20, -6, -22, -21, -3, -5, -12, 16, 22, 8, -9, 20}
}
, {{-30, -28, 25, 22, 25, -15, 18, -41, -10, 22, -1, 30, -3, -31, 0, 8, 26, 8, -1, -7, -11, 11, 0, 27, 7, -16, 22, -14, -5, -18, -17, 16, 9, 14, -8, 7, -24, -23, -7, -10, 33, -9, 7, 2, -25, -28, 9, 8, 11, 5, -10, -22, 19, 0, -9, 14, -10, 12, -20, 2, -10, 21, 11, -20, -13, -3, 14, -1, 4, -9, -12, -12, -4, 4, 17, -18, 22, 25, -2, 0}
}
, {{5, 18, 11, 8, -9, -2, 7, 17, 24, -8, 2, -22, 20, 6, -15, 24, 4, -4, 0, -21, 20, 12, 6, -4, 18, -5, -13, -4, -17, 3, -13, 9, 12, 3, 6, 3, -6, 9, -8, -20, 14, -6, -18, -19, 2, 10, -23, 11, 11, 16, -11, -18, -32, -19, -25, 5, 19, -26, 14, -11, -31, 2, -22, 1, -24, -9, 10, -6, -34, -11, -12, -15, -3, 0, 11, -16, -18, 4, -1, 11}
}
, {{18, -7, -14, -22, -24, -3, 21, 16, 9, 25, -25, 10, -2, -21, -16, 24, -14, -8, -10, 0, 16, -13, -20, -1, -12, -15, 14, 5, -6, 15, -9, 7, -14, 14, 5, 21, 20, 9, -18, 15, -17, -28, 21, 34, 24, 22, 16, -21, -14, -11, 21, -1, 0, -3, 21, -2, 16, 30, -4, -1, 2, 33, 2, 8, -22, 19, -6, 25, 37, 33, -12, -25, 1, -1, 37, 24, 19, 20, -17, -23}
}
, {{-13, 13, -19, -11, 16, 13, -2, -10, 14, -2, -17, -22, 13, -1, -18, 4, -8, 0, 13, 0, 13, -2, -20, -13, -18, -2, -14, 10, -20, 6, 7, -5, 0, 9, -22, 0, 13, -15, -9, 7, -28, 5, -14, -16, -8, 0, 11, 2, -2, 15, 3, -11, -30, 8, -8, -27, 14, -23, -32, 9, 15, -2, -2, -23, -19, -8, 0, 15, 4, 4, -13, -9, 6, 11, -26, -19, -28, -7, 8, 13}
}
, {{11, -9, -13, -4, -3, -7, 2, -5, 21, -12, 13, 1, 28, 20, -18, 27, 20, 28, -4, 14, 12, 19, -13, 9, 19, 3, -11, -14, -10, -9, -2, -2, 20, -1, -1, 13, 22, 0, 13, 0, -11, 26, 12, 5, 5, -3, 9, 25, 8, -7, -8, 1, 22, 6, 13, 6, -7, -4, 2, -4, 17, -19, -5, 22, -13, 2, 23, -13, 8, 28, -8, 21, 14, 4, 20, 16, 7, 20, -12, 16}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE