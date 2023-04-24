/**
  ******************************************************************************
  * @file    weights/fc.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#define INPUT_SAMPLES 3040
#define FC_UNITS 3


const int16_t dense_5_bias[FC_UNITS] = {-17, 4, 14}
;

const int16_t dense_5_kernel[FC_UNITS][INPUT_SAMPLES] = {{19, 10, -6, 13, -10, -8, -18, -1, -25, 7, -39, 1, 0, -30, -34, 1, -12, -20, -19, -25, -2, 4, -9, -18, -11, -14, -27, 18, 7, -4, -3, -12, 40, 28, 16, 14, -7, -34, -10, 3, 36, -7, -16, -17, 0, 11, 11, 15, 7, -18, 42, 9, -29, -9, -2, 2, 10, 2, -32, 2, 14, 11, 4, 15, 0, -25, 20, 19, 11, 17, 11, 24, -18, -33, -6, 31, -5, 13, -10, 25, 4, 22, 3, 20, 21, 45, 9, -8, 15, 35, 28, -20, 22, 21, 27, 6, 10, 10, 8, 1, 16, -9, 13, -17, -13, -1, 0, -4, -7, -12, -4, 6, 21, 16, 16, -13, 26, 32, 38, 13, 28, -14, 22, 22, 7, 0, 0, -10, -10, 10, 3, 0, -10, -3, -20, 12, 19, 21, -15, 7, 29, 2, 13, -12, -20, -1, 1, -2, 1, -8, 9, -7, -9, -11, -30, -15, -8, 12, 12, 0, -1, 11, 16, 16, 1, -14, -5, -23, -17, 6, -20, 9, 5, -38, -27, 10, 9, -9, -1, -34, 4, 14, 2, -15, -7, -2, -8, -7, -22, -6, 11, 43, -7, 13, 11, 2, 33, -1, -7, 12, 12, 6, 20, 3, 12, 23, 28, 28, 27, 19, 5, 10, 31, -4, 18, 12, 27, -1, 22, 9, 20, 17, 23, 33, -3, 25, 1, 27, 5, -9, -8, 32, 18, 9, -2, 0, 11, 4, 4, -25, -15, 5, 22, 4, 9, 15, 36, 1, 30, 32, 41, 17, 38, -11, -13, 29, 16, 12, -5, 3, 16, 4, -3, 26, 31, 21, 25, 19, 24, 26, 31, 34, 34, -4, 0, -4, 20, 15, 23, 23, 3, 37, 5, 21, 23, -10, 9, 23, 22, 15, 28, -13, 12, -23, 9, 19, -5, 15, -30, -14, 13, -2, 17, -9, -12, 10, -9, -10, 13, -20, -13, -20, -20, 19, 10, 7, 2, 12, 22, 29, 0, -10, -23, -17, 2, 25, 18, -18, 8, -7, 24, 11, -16, 6, -21, 31, 27, -10, -15, -5, -15, 8, 31, -3, 12, 25, 39, 25, 47, 29, 0, 12, -5, 12, -13, -18, -1, -5, -35, 21, 20, -2, -22, -3, 8, -27, -6, -7, -4, -18, -3, -5, 17, -13, 20, 2, 4, -8, -24, 19, 28, 13, 10, 7, 16, 25, -10, 17, -3, 0, 15, 21, -6, 0, 12, -17, -2, 19, 0, -12, 31, -3, 5, -14, 20, 5, -4, 16, -6, -2, 26, -2, -6, -18, 6, -19, 14, -9, 10, 10, 6, 12, -1, 14, -12, 0, -3, -7, 5, -5, 11, -24, -21, -2, -9, -16, 0, -9, 22, 2, 6, -7, -4, 0, -19, 20, -13, 17, -7, -10, 13, -21, -18, -21, 6, -3, 0, -5, 0, -17, 7, -8, -20, -3, -19, -14, -11, 21, -16, -19, 2, 19, 6, 1, -1, -5, 6, 9, 10, 9, -17, -36, -5, -17, -8, -14, 15, -41, -28, -6, -18, 36, -1, -25, -15, 0, 8, 16, -12, 11, -14, -9, -13, -42, 0, 17, -16, 15, -18, 1, -19, -3, -15, -33, -26, -12, 12, 10, -16, -20, 15, 15, -35, -3, 3, -15, -9, 10, 13, 5, 3, 2, -38, -14, 10, -10, 4, 4, -9, 9, 26, -14, 3, 4, 6, 4, -23, 10, -13, -11, -3, 0, 14, 0, -5, 29, 22, -9, -12, 21, 11, -4, -1, -15, -13, -27, 18, -27, -20, -38, 28, 17, -5, -10, -9, 13, -9, 22, 29, -5, 13, -7, 26, 28, 19, -10, 19, 1, 15, 12, -7, 26, 5, 34, 0, -7, 24, 2, -14, -9, -12, 7, 0, -11, 14, -6, 19, 19, 13, -3, 3, 24, 0, 8, 10, 11, -9, 0, 3, 10, 13, 15, -16, -9, 7, 2, -12, -3, 31, 14, 16, 36, 7, 0, 15, 30, 17, 15, -12, -27, -9, 4, -14, 22, -2, -8, 8, 22, 26, 26, -1, 21, 0, 10, 33, 22, 4, 36, 4, 3, 31, 42, 13, 17, 18, -17, -28, 0, 0, 16, 0, 16, 31, -1, -30, 7, -27, -15, -2, -4, -25, -14, 12, -22, -10, 2, -20, 2, 11, 23, 0, -22, 6, 10, -17, 0, -5, 16, -8, 5, 13, 2, -3, -10, -15, 10, -14, 20, -12, 16, 21, -13, 5, -13, 7, 15, 10, -20, 5, -14, 11, 12, 14, 21, -19, -18, 12, -10, -12, -7, -22, -13, -23, -23, -6, 1, 0, -13, -2, -10, -2, 8, -4, 19, -21, 13, -4, 9, 1, -25, -4, -15, 15, -5, 0, -8, 19, 0, -28, -7, -7, 10, 7, -5, -6, -3, 22, 19, 19, -5, 13, 16, -19, 1, -23, 10, -18, 2, -2, -1, 9, -8, 10, 18, 22, 0, -7, 11, 3, -3, -16, -9, -7, -15, -4, -15, -17, 16, -26, 19, -22, -14, 2, -11, 15, 3, -6, 12, -10, -26, -17, -14, -26, 3, -5, 1, -5, 8, 15, 9, 15, 0, -4, 20, -3, 1, 1, 8, 15, -3, -11, 10, -2, -18, 0, -1, 6, -1, 15, 10, 11, 16, 18, 16, 5, -10, -14, 18, 14, 13, 6, -14, -14, -4, 17, -3, -36, 2, -21, -9, 5, -28, -20, -7, -19, -21, -26, -28, -32, 2, -12, -37, 9, -25, -14, 11, 7, 13, 0, -11, -21, 4, -6, -17, -12, -18, -36, -31, -4, -8, 6, -29, -29, 3, -30, -11, -34, -36, -10, -36, -24, -24, -20, 11, -1, -13, 0, -34, -24, -22, -34, -33, 18, -28, 13, -27, -3, -10, -24, -6, -10, -5, 0, 17, 7, -3, -6, -10, -12, -3, -10, -21, -28, -18, -15, -8, 1, -25, -21, -9, -4, -9, -14, 5, 0, -15, -11, -14, 7, -7, 34, 46, -4, 19, 11, 32, -3, 12, 25, 0, -8, -2, -7, 21, -7, 36, 3, -10, 13, 22, -11, -23, 23, 3, -4, 3, -1, 20, -8, 28, -2, -7, 19, 1, -6, 18, 30, 6, 11, 12, -9, 0, 14, 0, 0, 21, -21, 6, 11, -2, -18, -5, 23, 6, -8, -16, -1, 23, 14, 17, 43, 13, 18, -14, -7, 22, 32, -15, 5, 14, 4, -3, 24, 28, 7, -22, -1, 4, 7, 8, -19, 35, 13, 3, 15, -1, 4, 20, -11, -10, 6, 16, 21, 24, 22, 13, 26, 15, 13, 18, -11, 21, -22, 13, -11, -4, -5, -16, 7, 24, 20, 2, 7, 23, 14, 10, 8, 15, -1, 28, 21, -14, -17, -7, -2, -8, -7, 25, 9, 11, 21, 19, -15, 17, 8, -3, -8, -6, 12, 21, 20, -23, -16, -21, 1, -10, -15, -3, -2, -18, 19, 14, 22, -7, 6, 2, 18, -1, -2, 1, -8, -5, -1, 2, -16, 16, 11, -17, 2, -11, 3, 14, -1, -6, 5, 15, -14, 6, 10, 18, 15, 0, 21, 27, 18, -6, 12, 0, 23, 22, 38, 15, 8, -3, 23, 22, 31, -2, 22, 31, -9, 21, -3, 10, 19, 12, 24, 27, 15, 29, 6, 9, 0, 5, 28, 31, 4, 0, 11, 12, 15, 17, 7, -5, 4, 29, 3, 17, 20, 28, -7, 22, -3, 6, 28, -11, 20, -7, 24, 29, 10, 2, 3, 10, 26, -6, 12, 14, 14, 7, 25, 35, 25, -4, -2, 23, 2, 16, -7, 1, -16, 0, -1, 11, 23, -11, 3, 4, 14, -21, -14, -5, 12, 29, -2, -12, 28, 27, -4, 32, 38, 6, -7, 5, 6, -15, 22, 18, 3, 0, 0, 2, 9, -7, 21, -2, 1, 0, -21, 21, -2, -4, -19, -19, -3, 3, 5, 7, -21, -16, 16, -11, -11, 14, -11, 6, 24, 11, -5, -19, -22, -10, 14, -10, 3, -25, -4, 19, -8, 21, -16, -7, 11, -15, -14, -8, -16, -9, -12, 3, 13, -5, 0, -1, -7, 10, 4, 3, -1, -21, 5, 2, 4, -7, -13, -29, 8, 11, 11, -5, 6, -16, 15, 0, 4, 1, -2, -2, -2, -14, 11, 7, 0, -4, 4, -11, -17, -4, 0, -2, 24, 6, -15, -10, 15, 13, -6, 14, -17, -1, 0, -14, -6, -24, -30, -44, 7, 32, -5, 14, 14, 35, 5, -7, -30, -13, 16, 30, 1, 5, 6, 4, 5, 34, 7, 12, -11, -39, -11, 1, 39, 19, -16, -1, 6, -4, 3, 13, 19, -64, -21, -11, 8, -11, 2, -29, 6, 6, -29, 10, 0, 5, 28, 6, 7, -14, -32, 8, 4, 8, -22, 27, 5, -11, -5, 18, 5, 18, -15, 8, -5, -8, 7, 1, 30, 26, 5, -1, -6, 37, -12, -33, 16, 4, -1, -23, 0, -33, -20, -24, -9, 12, -28, -6, -18, -1, 7, -6, 3, 2, 2, -12, -26, -44, -43, -9, -8, -2, 9, -1, -8, -36, -23, -21, -27, -30, -2, -21, -11, -14, -32, -15, -29, -20, 0, 1, -18, -10, -23, -4, 16, -1, 3, 0, -21, -7, -2, 2, -20, -25, -7, -30, -5, 11, 2, 16, -10, -12, -10, -3, 15, 15, 22, 5, 12, 3, -27, 11, -12, -17, -7, -19, 20, -10, 7, -25, -30, -6, -18, 0, -10, 14, -13, -26, -22, 6, 7, -35, -22, 2, 26, -7, -4, 6, 16, -7, 7, 27, 6, -1, 6, -27, 5, -2, -12, -11, -7, 1, 21, 3, 24, -7, 19, 18, 8, -14, 14, 22, 28, 29, -11, 12, 14, 2, 27, 14, 31, -2, 25, 5, 15, 5, -7, 13, 10, 25, 22, -10, 20, -11, -13, -1, -20, -6, 22, -6, -7, 5, 1, 15, 2, -13, -8, 10, 18, 23, -25, 11, -17, -29, -22, -26, -16, -4, -14, -9, -21, 1, 17, -28, -27, -1, -19, 0, -14, -14, -5, 5, 8, -38, 1, 14, -32, -13, -13, 2, 11, 19, 7, 12, -17, 11, -23, 2, -20, -26, -9, -5, 8, 5, -23, -15, 1, -7, 11, -18, 2, -5, -12, 17, 2, 17, -5, 6, -9, -17, 15, -26, 11, 19, 11, 11, 8, 7, -2, 1, -12, 4, 20, 15, 1, 21, 14, -3, -5, -16, 7, 7, 15, -13, -26, -20, 18, 18, 9, -5, 3, -7, 3, 6, 25, 22, -17, 21, -16, 0, 4, -23, -9, -24, 0, -9, -4, 0, -26, 8, 8, 14, -15, 6, 7, 23, 3, -19, -13, 14, -15, 2, -17, 11, -12, 1, 1, -12, -8, 12, 5, 4, 14, -12, -22, -26, 0, 24, -1, 4, 0, 20, 17, -2, -15, -14, 15, -14, 7, 25, -21, 5, 17, -9, 0, -12, -14, -10, 26, -10, -13, 33, 22, -7, 11, -12, 9, -4, -14, 20, 17, -8, 19, -7, 6, -7, 15, -6, 13, -15, -25, -15, -10, -7, 9, -11, -14, -16, -10, -10, 0, 3, -22, -21, -29, 0, 17, 18, -20, -15, 3, -2, 12, -15, -5, -5, 16, -14, 21, -23, -13, -18, 0, -10, 6, 12, -3, 5, 1, 9, 1, -7, 12, 4, 7, -7, -1, 3, 0, 12, -24, -10, -22, 0, -12, -26, -27, 11, -3, 19, -18, 6, 13, -1, 23, 0, 27, 0, -1, 17, -7, 16, 22, 12, -13, -16, 14, 20, 14, -14, -14, -16, -20, 1, 9, 0, -10, 0, 25, 7, -15, 5, 10, 14, -17, -6, 29, 12, -1, 9, 18, -5, 14, -8, -18, 14, -17, 3, -8, -10, 3, -6, 13, 0, 2, 5, 0, -17, 13, -16, -21, -15, 10, 3, -16, -7, -6, -21, -22, 8, -21, -15, -23, -13, 5, -11, -9, -10, -2, 7, -13, -4, -17, -25, -16, -17, -9, 0, -10, -24, 1, 7, -11, 0, -32, 0, 0, -36, 11, -11, -21, 22, -6, 12, -10, -12, -2, 8, -9, 16, -14, -16, -1, 2, 4, -19, -10, -6, -15, -23, -17, -22, -27, -15, 19, 4, -14, 11, -10, 8, 13, 5, -19, -22, 10, -14, -11, 2, -6, -31, 9, -21, -23, -16, -27, -12, -9, -8, 12, -7, -19, -1, -32, -9, -16, -1, 10, -24, -8, -5, -12, 13, 5, 19, 26, 3, -34, -3, -5, 7, 5, 3, -16, -20, -20, 10, -2, 8, -15, 0, 4, 11, -12, -10, 17, 21, 9, 9, -21, -10, 0, 14, -16, -4, 3, 22, 12, 28, 16, -9, 4, 3, 13, -20, 5, -13, -7, -10, -3, -11, -10, 18, 17, -5, 2, 22, 12, -20, -19, 19, -12, 7, 18, -1, 16, -1, 4, 27, 12, 11, -6, 26, -16, 22, -12, -12, 3, 22, 15, 9, 19, 6, 15, 20, 16, 26, -3, 10, 10, -19, -17, 9, -16, 4, 3, 13, 6, -5, 21, 4, -23, -14, 20, 4, -13, 0, 4, -15, 18, 22, 4, -14, 0, -9, -17, -27, 22, 11, 5, 19, -14, 0, -10, 3, -4, -20, 2, -1, -17, 3, -15, 22, 8, 1, 8, 3, 9, -12, -18, 3, -4, 3, -3, 12, 10, 14, -1, -14, 11, 7, 7, 1, 10, -1, -15, -11, -8, -25, -4, -13, -15, 16, -8, -21, 9, -12, 0, -14, -21, -3, 0, 9, -31, -15, 0, 2, -21, 8, 2, -11, -23, -7, 14, 2, -25, 14, -3, -19, 16, -16, 6, -3, 10, 9, 19, -17, -2, -24, 2, -28, -2, 7, -10, 13, -16, -10, -2, 16, -23, -6, -16, 12, 10, -14, -4, 12, -3, -11, -21, -19, -3, -20, -13, 6, 16, -14, 8, -4, -5, 3, -21, -24, -9, -16, -28, -20, 0, -7, -3, -14, -17, -7, -15, -17, -8, 16, 2, 9, -23, 7, 4, 13, 15, -14, -16, -3, -10, 0, -16, -12, -22, -15, -26, -26, -9, -13, -32, -14, -33, -20, 4, -5, -10, -18, -18, -10, -17, -21, -24, 0, -21, 9, -11, -14, -4, -2, -8, -6, -4, 1, -8, -23, -13, -45, 0, -18, 11, 10, -2, -12, 11, -14, 5, -14, -17, 0, 0, -17, 22, 11, -2, 20, -13, 17, 0, 14, -8, 8, 10, 4, -1, 6, 0, -13, -3, 6, -4, -6, -9, -3, 3, 10, 17, -10, 21, -4, -8, 21, 14, 16, 2, -8, 6, 3, 19, -14, 5, 7, -26, -11, 15, 13, 2, -7, -1, 14, -10, 7, 16, 10, -7, 11, 10, -16, 18, 8, -20, 17, -19, 5, 0, -22, -5, 11, -4, 6, -3, -1, 18, 20, 5, -16, 21, 22, 5, -4, 6, -4, -20, -19, 2, 17, 2, -10, -3, 11, 17, 11, -6, 3, 22, 12, 7, -19, 9, 11, 4, -5, 8, -12, -9, 1, 24, -13, -1, -11, 14, -5, -15, -15, 25, 5, 5, -8, 1, 13, 2, 7, 12, -10, -8, 0, 13, 24, -5, -9, -9, 0, 0, -17, -19, -11, -12, 9, 13, -10, -12, 4, 3, 0, 27, -11, 8, -2, 21, 19, -3, 3, 5, 17, -11, 11, -3, 8, 16, -14, -20, 3, -12, -8, 14, 24, 12, 21, 19, 4, 7, 11, -5, -2, 13, -14, 6, 0, 8, -16, 19, 12, -5, -4, 1, -15, 6, 11, -18, -8, 25, 3, -14, 6, 7, -6, 11, 0, -7, -7, 16, 0, 11, -6, 10, -1, 32, 31, 1, 27, 3, -2, -10, -9, -5, 4, 17, -14, 10, 22, -11, 0, 13, 8, 1, 10, -19, -2, -16, 5, -5, -13, 9, 9, 12, 12, 0, -9, -5, -10, 16, 14, -24, -28, 1, -5, 1, 26, -18, -13, -12, 5, -4, -13, -4, 6, 1, 14, 1, 0, 0, -5, -4, 0, 12, 19, -3, -2, 19, 21, 21, 12, 15, 5, -10, 0, 8, -2, 20, -20, 13, -7, 10, -12, 18, 2, 15, 15, 8, -19, -20, -6, -3, 2, 26, -14, -3, 25, 10, -2, -5, 1, 6, 20, -5, -7, -3, -10, 9, 7, -16, -18, 1, 15, -14, 16, 20, -16, -16, -7, -15, 2, 8, 12, 21, -12, 2, -4, -17, 6, 1, 7, 8, -5, 11, 21, 10, -11, -10, -17, -18, 10, -18, 5, -11, -7, -21, -16, 0, -11, 6, -21, -14, 4, 10, 18, 8, 13, -11, 2, 12, -9, -6, 12, -18, 7, 11, -8, 16, 18, 10, 11, 7, 17, 20, 17, 13, 27, -2, 19, -10, 22, 1, 21, -5, -8, 12, 21, 31, 13, 20, 7, 12, 23, -17, -4, -9, -8, -16, 11, 9, 27, 18, -5, 10, -13, 14, 4, 14, 20, -1, 14, -10, 17, -11, -17, 7, 16, 8, -6, 11, 17, -13, 4, 22, 24, 14, -17, 19, 18, 16, 10, 9, 0, -6, 27, 13, -7, -10, 18, 19, 20, 21, 16, 13, -22, -7, 22, 10, -3, 14, 11, 9, 16, -8, 13, -4, -7, 4, 16, 0, 30, 5, -42, -14, -22, -11, -9, -7, -35, -26, -13, 8, -3, -34, -36, -3, -1, -19, -14, -11, -35, -34, 12, -32, -30, -22, -29, -23, -15, -16, 0, 4, -1, 8, -24, -28, -32, -19, -20, -7, -9, -3, -34, -19, 5, -5, -12, -22, -16, -22, -33, -30, -22, -39, 6, -22, -13, -1, -41, -18, -29, -27, -27, -34, -21, -1, -9, -33, -26, -19, -16, -26, -45, -43, -55, -31, -48, -45, -5, -33, -11, -32, -36, -20, -47, -33, -19, -36, 0, -6, -6, 14, 1, -30, 0, -42, -26, 22, 11, 6, 11, 34, 34, 9, 30, 21, 13, 16, 33, 19, 18, 32, 23, 28, 16, 30, -2, 27, 21, 2, 10, 31, 10, 1, -8, -11, -8, 25, 12, 27, 6, 14, 29, 27, -10, -4, 37, 5, 0, 20, 24, 29, 13, 22, -18, 5, 6, 14, 3, 15, -6, 19, 6, 28, 0, 6, -9, -6, -1, 3, 30, 6, 27, 20, 0, -1, -8, 4, 20, 33, 26, 34, 16, 0, 18, -6, 9, -1, 5, -9, -6, 19, 0, 16, 30, 17, 18, 5, 30, 2, 22, 18, 9, -14, 22, 21, -12, 20, -8, -1, 17, -10, 2, -27, -14, 17, 14, 7, 0, -23, 15, -10, 22, -3, 10, -12, 23, 33, 10, 27, 15, 8, 4, 6, 21, -21, 4, 0, -2, 22, 15, 16, -8, -16, 27, -3, -5, 7, -5, -5, 1, -22, -21, 6, -27, -14, -1, 21, -4, 6, -3, -16, -17, 4, -6, -8, 9, 16, -4, 12, 20, 10, 17, 7, 21, 17, 9, 8, -21, 7, -7, 29, 16, -6, 14, 11, -19, -9, -22, -5, 17, 20, -1, 21, -20, -12, 0}
, {2, -27, -17, 0, -4, 3, -22, -10, 1, 4, 12, -6, 15, 14, 27, 9, -24, -14, -21, 11, 19, -14, 11, 28, 0, 35, -13, -24, -14, -10, 30, 16, -28, 10, -18, -6, 2, 36, 21, -30, -23, -18, -10, 3, -21, 12, -19, 4, -16, -15, -1, 0, 14, 4, 37, -5, -32, -20, 22, -1, -10, 30, 22, -11, -18, -7, 17, -10, 10, 26, -23, -27, -20, 2, -21, 3, 16, 7, -10, -13, 10, 13, 18, 25, -2, -18, 11, -20, 1, -14, -4, 14, -17, -7, -4, 8, -26, -24, -24, -12, -22, -30, -44, -14, -38, -12, -27, -53, -49, -38, 1, 5, 10, -27, -29, 11, -38, -25, -33, -26, 7, -1, 19, 28, -29, -1, 11, -30, -17, -16, -36, -4, -20, -7, 21, 6, 27, -7, 6, -3, -25, 15, 7, -8, 24, -20, -52, -51, -6, -12, -14, -16, -7, -40, -7, -28, -15, -50, -25, -42, -35, 6, -9, 23, 5, 16, 22, -26, -25, 10, -11, 8, -2, 3, 10, -51, -19, -48, -28, 7, -10, -15, 4, -38, -37, -21, -25, -8, 6, -13, -48, -16, 4, -15, -36, -36, -11, -19, -4, -11, -15, 13, 18, 11, -19, -16, -36, -17, 4, -32, 1, -8, -7, 15, -10, -14, -4, -26, -28, -3, -16, -20, -22, -28, 0, -21, -13, 13, -21, -20, -12, -30, -12, -18, 6, 9, -27, -24, -16, 4, -12, -14, -6, -7, -33, -10, -11, -31, -33, -21, -10, -35, -27, -32, -20, -37, -30, -17, -7, -15, -39, -39, 0, -13, -29, -19, -23, -34, -30, -4, -32, -15, -27, -24, -7, -40, -28, -27, -54, -18, -18, -41, -46, -52, -38, 10, -36, -11, -24, -12, -29, 8, -2, 27, -12, 12, 12, -30, 16, 27, 17, 1, 8, -14, -7, 21, -26, -11, 4, 2, -4, 8, 28, 2, -6, 16, 28, -11, -8, -12, -3, -25, 21, 32, 14, -3, -21, -17, -25, -18, -4, 9, 12, 8, -6, -45, -34, 1, 8, -9, 2, -6, 7, -2, -23, 4, 1, -7, -10, -2, -16, 16, -9, -13, 3, 14, -31, -3, 18, -6, -1, -14, 41, 23, -19, -27, 18, -13, 10, -16, -35, 23, 6, -33, -37, 11, 1, 15, -12, -8, -10, 2, -18, -12, -8, -30, 9, -8, 0, -7, 5, -1, -26, -25, 0, -15, -13, -16, -6, -25, -23, -15, -2, -9, 0, -9, -19, 8, -9, -28, -14, 2, -18, -18, -2, -14, -14, -4, 0, 1, 4, 17, 8, -6, -10, 1, 14, 22, -17, -9, -9, -15, -10, -2, -18, 7, -23, -29, -11, -21, 1, -4, 6, 10, 2, -31, -21, 16, 18, -16, -18, -24, 0, 15, -1, 2, -21, -4, -22, -19, -16, -12, 10, -2, 9, -27, 10, 17, 4, -23, 0, -23, -27, 4, -4, 2, 1, -14, 11, 4, 6, -14, -11, -7, 17, 15, 1, -3, 7, -5, 17, -24, 3, 21, 19, 10, -15, 8, -5, -2, 9, -9, 14, -8, -2, 6, 7, -25, 24, -5, 6, 8, -21, 12, 12, 16, 5, 9, -3, -13, 14, -16, -2, -2, -17, -6, -2, 5, 0, -10, -7, -7, 5, 10, -18, -5, 9, -3, -9, 14, 7, 19, 1, -2, -17, 21, 24, -1, -27, -16, 15, -23, 10, -11, 6, -8, 0, 1, 0, 1, 29, 17, 3, 8, 19, 6, -11, 15, 4, -3, -17, -2, 6, -19, -12, -2, -6, -9, 9, -12, -15, 11, 10, -17, -2, -2, 6, 7, -17, -17, -9, -21, -22, 15, -20, 10, -5, -26, -11, -16, 15, 11, 14, 10, 12, -2, 11, -12, 16, -16, -17, 16, 6, 9, -11, -1, -7, -28, 10, -19, -5, 0, 6, -2, -16, 1, 16, -11, -9, -21, -6, 0, -2, 9, 20, -21, 9, -4, 4, -10, -7, 23, -7, -27, 10, 13, 6, -29, 17, -20, 16, -5, -8, 12, 2, -7, 23, -13, 4, -3, -19, -10, -20, -15, -22, 17, 43, 35, 43, -7, 29, 0, -22, -21, -16, -13, 13, 17, 15, -7, 15, -10, -4, 29, -29, -29, -16, -18, -5, -7, -3, 8, 17, -17, -8, 11, 25, 18, 0, 7, -6, 14, -18, 29, 20, 10, 6, -1, 32, -5, -7, 22, 8, 21, 27, 14, 19, -22, -4, -12, -7, 3, 16, -10, -21, -12, 28, 10, -11, 34, 6, 26, 8, 23, 37, 38, 15, 0, 18, -7, 4, 22, 20, -1, 12, 12, -10, 15, 14, 5, 17, 19, -13, -8, -1, 19, 26, 21, 37, 28, 11, 12, 15, 17, -1, -5, 7, -14, 5, 10, -4, -13, 8, -17, 2, -1, 15, 23, -15, -14, 0, -21, -13, -8, -21, 7, -6, 4, 3, -2, 15, -7, -17, 26, 11, 1, -5, 23, 16, -3, 3, 6, -11, 13, 0, 14, -14, -24, 7, -12, 9, 17, -9, 0, 16, 18, 22, 13, -11, -9, -12, 8, 2, 9, 13, 8, -8, -1, -9, 18, 18, 13, -16, 8, 6, -11, 8, 15, 0, -3, 14, -3, 6, 9, 12, 19, -6, -6, 0, 10, -7, -13, 20, -2, 24, 22, 0, 7, 12, -3, 24, 16, -22, -5, -16, -8, -19, 10, 6, 8, -18, 16, 23, 4, 20, 16, -16, -10, -11, -11, -6, -3, 11, 17, 3, 0, -2, -9, 3, 21, 3, -3, 6, -3, 4, 9, 23, 5, -9, -8, 10, 15, 13, -18, 10, -14, -9, 2, 17, -6, 10, -11, 20, -12, -9, -23, 16, 16, 21, -2, 0, -10, 6, -7, -5, -9, 6, 21, 0, -15, 13, 12, -8, 13, 19, -1, 13, 13, -5, -11, 22, 29, 19, 13, 2, 20, -19, -9, -9, -9, -9, -14, -21, -16, -5, 9, 14, -20, -15, -19, 23, -9, -23, -9, -5, -34, -21, -27, -1, 2, -26, 2, 0, -7, 22, 13, 9, -11, 19, 9, -14, 22, -16, -25, -3, 7, -21, 7, -1, -13, 5, -10, -32, -5, 7, -6, -7, 10, -24, -15, -23, -32, -3, -7, 9, -12, 4, -7, 10, -16, 9, -9, 0, 13, -20, -13, 5, 19, -6, -2, 0, 12, -6, 8, -22, 12, 0, -22, -24, 10, -22, -12, -28, -18, 10, -11, 17, 17, -14, -11, -23, -14, -14, -15, -10, -30, -31, -34, -34, -36, -24, -28, -20, -24, -35, -30, 0, -29, -12, -35, 0, -7, -23, -20, -1, -14, -33, -21, 7, 8, -21, -14, -14, -5, -19, -31, -15, 9, -22, 6, -19, -14, -15, -5, 15, 18, -22, -1, 1, -10, 5, 13, -15, -24, 3, 0, -24, -16, 13, 11, -16, 10, 1, -5, -5, 0, -7, 4, -27, -16, 3, 5, 4, -27, -22, 12, -13, -1, -8, -17, 2, -17, -4, -13, -19, -14, -10, 10, -6, -24, 14, 6, 1, -20, -11, -29, -16, -22, -4, -8, -20, 28, 16, 12, -15, -5, 20, -29, 8, -5, -19, 1, -1, 6, -12, -21, -8, 1, -25, -19, -3, 0, -15, 4, -3, 2, -13, -13, 0, 15, 22, 26, 19, 12, 6, -16, 17, -4, 22, 28, 0, -10, 12, -6, -17, -17, 13, 14, -25, 6, 0, -1, -11, 2, 1, -17, 4, 2, 1, -20, 0, 9, 0, -4, 3, 22, -1, -3, 0, 38, 10, -6, 12, -18, 21, 21, 26, -7, 0, -2, 25, 18, 17, 5, 7, 0, -4, 32, 6, -9, -10, 21, 23, 37, 2, 17, 7, -6, 1, 1, 6, 14, -5, 2, -9, -15, -1, -6, 6, 19, -4, 12, -22, 5, -11, -13, 26, 16, 10, -12, -22, 3, -23, -9, -6, -24, 9, -6, 0, -13, -20, 14, 7, -7, 5, 13, -5, 16, -12, 18, 12, 11, 9, -9, -10, -18, -9, 8, 3, -23, -12, 5, -21, 17, -8, -9, 12, 8, -9, -7, -1, -5, 4, -10, -8, -4, 24, 4, 0, -12, -16, 11, 4, 1, 9, -10, 15, -1, -23, -23, 16, 4, -1, -17, -24, -5, 5, -20, -9, -9, 6, -25, -39, -34, -5, -2, -10, -8, -3, 14, 11, 28, -39, 14, -4, 16, -2, -35, -19, -9, -31, -53, -38, 23, 12, 21, 0, 21, -17, -39, -8, 7, -21, -7, -25, 19, -9, 21, 21, -12, -28, -41, 2, -33, -26, -34, -6, 20, -20, 25, -33, -30, -11, -5, 22, 21, -13, 12, -31, -29, -21, -22, -17, 6, 27, 17, 12, 22, 33, 24, 10, -41, 6, 17, 2, -46, -5, 32, -9, -69, -38, 4, -14, 15, -41, -37, 16, -23, -42, -66, -4, 23, 12, -32, 10, 17, 4, 41, 1, 12, 19, 13, 34, 19, 53, 19, 24, 42, 27, 27, 17, 3, 32, 43, 6, -1, -7, 30, 17, -10, 6, -4, 22, -17, -5, 36, 25, 7, 15, 25, 28, -1, 15, 14, 3, 6, 22, -5, 3, -3, 15, 33, 11, 14, 27, 26, 6, 1, -3, 36, -9, 26, 12, 15, 23, 36, 39, 51, 16, 15, 9, 39, 6, 13, -4, 39, 16, 12, 3, 43, 21, 47, 29, -4, 21, 38, 8, 13, 3, 2, 19, 1, 19, 33, 30, 29, 18, 55, 38, 22, 2, 27, 25, 10, 5, 7, 22, 4, -18, 0, 4, 28, -19, -21, -12, -22, 24, -14, 24, 13, -4, -8, 11, -14, -7, 14, 3, 0, -15, 5, 19, -17, 0, -17, 0, 4, -30, -8, -16, -9, 17, -12, 13, -15, 15, 12, 16, -14, -32, 19, 13, -7, 0, -21, 4, -17, 3, -17, -20, 4, 12, -7, -2, 32, -5, -7, -4, 5, 18, 9, 40, 0, 27, 4, 4, 9, 21, 23, -9, 38, 29, 16, 35, -6, 12, 14, 15, -2, 22, 15, -5, 2, 28, 28, 14, 14, 21, 18, -15, 9, -16, 17, 4, -21, -22, -5, 13, -19, -6, -9, -15, 16, 13, 0, 13, -9, 4, -20, -2, -9, 8, -8, -5, -14, 7, 16, -4, 4, -6, -19, -12, 0, 4, -1, 25, -13, -16, 3, 23, -13, -13, -23, 16, 16, 11, 3, -2, -3, -12, -10, 11, -15, -13, 4, -3, -9, -21, 16, -12, -20, -12, -3, 7, 14, 0, 15, -21, 1, -4, 8, 9, 3, -13, -1, -10, -4, -4, -18, 4, -22, -7, 8, -20, 18, 0, 2, -6, -6, 2, 6, 40, 15, 40, -8, 15, 18, 14, -4, 26, 21, 13, -7, 3, -18, -3, -14, 26, 23, 19, 5, -28, -8, 9, -10, 8, -16, -2, 2, 0, -9, -9, -6, -1, -28, -23, 4, 3, -29, 10, -5, -12, 12, -16, 1, -7, -16, -8, 7, 15, 7, 15, -16, 12, -2, 9, -13, -7, 6, 2, -14, 23, 29, 34, -3, 0, 37, 13, 6, -4, 0, 16, 4, 13, -8, -10, -6, 7, -12, 8, -6, 13, 7, 27, 2, 17, -11, -12, 12, 0, 37, 26, 13, 31, 17, 14, -1, 13, 14, 11, 30, 0, 32, 5, 29, 20, 29, -6, -9, 10, -3, 1, -3, 32, 24, 19, -11, -10, 11, 3, 9, 10, 19, 1, -20, -10, -12, 3, 7, 11, 11, 0, 7, -11, 21, 10, -16, 18, 0, 10, -13, 9, -12, 7, 23, -7, 16, 12, 13, 12, 2, 21, 24, 4, 12, -1, 33, 30, 23, 0, -2, 7, 3, 15, 23, 5, 34, 2, -9, 8, 24, -6, 11, -6, 0, 24, 6, -5, 34, 10, -6, 33, 27, -9, 0, 10, 18, 24, 44, 5, 17, 23, 17, -9, 5, -12, 15, -6, -3, 20, 18, -11, 0, -9, 10, 17, 17, -10, -7, 40, 6, 36, 9, 19, 19, 0, 16, 18, 16, 12, 0, 16, 24, 3, 28, -1, 6, 14, 18, 24, 17, -16, 6, 3, 19, -17, -8, -2, -7, -11, 0, 5, 2, 15, 2, -12, -1, -8, 4, 25, -15, 9, 5, 8, 16, 14, 20, 2, 16, 18, 20, -9, -10, 16, 21, -7, -4, 15, -5, 17, 19, 11, 4, 24, 0, 0, 25, 20, 6, 15, -11, -13, 27, 8, -12, 13, -10, -19, -1, -3, -11, -10, 6, -19, 4, 16, -1, -13, 3, 11, 25, -3, -11, -11, 8, -24, 10, 18, 9, 6, -25, -16, 19, 8, 13, 5, -6, -15, -12, 5, -21, -5, 14, -14, -6, 20, 12, 4, -17, -2, 20, -18, 19, -14, 18, -17, -20, 20, 18, 8, 18, 14, -12, -8, 1, -1, -20, -6, 16, -5, -20, -1, 19, 2, 18, -6, 6, -5, -14, 12, -7, 23, -15, 20, -8, -14, -9, 5, -1, 11, 10, 13, 9, 24, -8, 7, 2, -12, 18, -11, -11, 36, 5, 31, 30, 22, -5, 14, -16, -14, 7, -9, 1, -1, -23, -2, -22, 13, -14, 9, 20, -31, 15, -19, -21, -13, -23, 12, 2, 8, -10, -10, 3, 3, -20, -21, 4, 13, 1, -19, 1, 1, 25, -7, 20, 25, 14, 18, 9, 6, 8, -16, 20, 16, -3, 16, -4, 0, -25, 4, 19, -19, -2, 25, 35, 25, 21, 31, -12, 21, 6, 23, 2, 9, 0, -15, 3, 5, -2, 7, 2, 26, 9, 14, 26, 19, 6, 0, -8, -3, 19, -10, 20, 12, 17, 22, 22, 11, 4, 1, 3, 16, 13, 21, 20, 11, 1, 10, 3, 10, 2, 0, 26, 12, 4, -6, 14, -13, 19, 12, 9, -4, 0, 26, 10, 10, -11, 26, 10, -9, -7, 20, -11, -2, 10, 23, 0, 15, 20, -5, 28, -13, 23, 23, -16, 1, 4, -9, 18, 3, 10, 27, 2, -8, -2, -10, 7, 11, 27, 42, 24, -8, 24, -1, -1, -7, 39, -11, -3, 12, -4, -13, 6, -10, -16, 6, -14, 20, 29, 10, 1, 5, 3, 11, 25, 0, 9, 25, 17, 17, 34, 3, 7, -6, -14, -17, 18, 20, 0, -16, -23, 2, 14, 13, 7, 7, 18, -11, 12, 14, -1, 18, 14, 1, -20, -2, -15, 2, 15, -11, 10, -16, -19, 3, -4, 4, 14, 9, 1, 21, 13, 11, -6, 14, 11, 4, 0, -19, -14, -23, -19, 9, -10, 16, 3, -14, -3, -10, 15, 4, 20, 18, -6, 19, 9, -25, 0, -4, -14, 11, -2, 16, -3, -4, -7, 4, 14, -6, 0, -16, 10, -22, -4, -14, -24, -9, -3, -4, -15, -16, -11, -13, 9, 6, -15, 9, 22, -17, 18, -15, -3, 1, 2, -19, 7, -17, 0, 2, 8, 0, -9, -5, -9, 16, 6, 9, 11, 7, -3, 11, 21, 9, 9, 2, 2, -12, -15, 17, -18, -13, 13, 3, 7, -11, 0, -13, -3, 0, 13, 23, 23, 3, -16, 12, 18, -11, 16, 7, -11, 17, -10, 4, -13, 4, 19, 1, 10, 7, -13, -4, -17, 21, -20, 0, 2, 1, -10, -22, 7, 3, -18, 19, -16, 18, -13, 12, 11, 4, 18, 1, 2, 0, -18, 0, -9, 18, 6, 4, -23, -8, 22, -29, -13, 14, -1, -5, 4, -16, 5, 4, 0, -5, 0, -21, 4, -18, -17, -26, 16, 0, -11, -28, 2, -7, 7, -2, -13, 7, 8, 11, 0, 3, -19, 10, -9, -8, -21, -21, 23, -14, -10, -10, -11, -23, 4, 11, 2, 0, 14, -7, -1, 5, 18, 7, -29, -17, -21, -19, 0, -14, -20, -23, -26, -23, 10, 6, 14, -27, -16, -25, -11, 15, -24, -2, -26, -26, -14, 4, 12, -17, 5, -23, -11, -1, -20, -15, 3, -7, 7, 2, -16, -8, -14, -32, -9, -16, -8, 17, 6, 7, -4, -12, 21, -8, -1, 0, -9, 23, -16, 9, -1, -6, 21, -3, 15, -8, 16, -15, -6, 14, 12, 17, 15, -15, -12, -8, -19, -16, 7, 18, -5, -14, 3, 14, -14, 0, 9, 9, -14, 19, -5, -1, -1, -19, -3, -16, 21, 7, 5, 20, 6, 1, 13, 23, -7, 9, 0, 4, -18, -5, -4, -1, -8, 3, 0, -2, -13, 15, -10, 0, 24, 5, -7, 11, 0, -14, 4, -14, 16, 22, 1, 10, -15, -4, 3, -11, 17, 5, 20, 5, 0, -43, -36, -33, -37, -39, -17, -7, -12, -24, -34, -27, -44, -13, -34, -4, -3, -8, -1, -25, -5, -30, -13, -36, -27, -7, -4, -17, -7, -10, -20, -1, -20, -9, -8, 1, -4, -6, 6, -19, -8, -3, 13, 7, 17, 16, -1, 20, 13, 11, 3, -16, -12, -16, -16, -13, -8, 3, -10, -21, -32, 9, -20, -3, -4, 9, -10, -7, -20, 8, -19, 13, -7, 15, -10, -34, 8, 5, -8, -23, 14, 1, -4, 2, -31, -26, 5, -2, -4, -29, 9, -29, -30, -30, -14, -5, -5, 4, -2, -15, 3, 8, 17, 19, -13, -3, -15, -25, 2, -20, -12, 10, -8, 17, -9, 21, 17, 6, -10, 14, 5, -8, 25, -8, -10, 1, 15, -6, -1, 0, 5, 14, -5, -3, -15, 1, -2, 5, 7, -9, 7, 13, 24, 6, 6, -2, 17, 6, -17, -26, 0, 11, 24, -21, -5, -10, -15, 12, -4, 13, 19, 0, 31, 23, -11, 0, 11, 6, -18, -15, 18, 6, 15, 20, 8, 26, -16, -11, 7, -9, -10, -10, 11, 12, 11, -28, 0, 4, 28, -4, 14, -25, -40, -37, -43, -67, -53, -58, -32, -62, -28, -29, -60, -52, -26, -34, -53, -40, -31, -40, -56, -39, -42, -52, -67, -41, -16, -38, -46, -27, -52, -37, -37, -37, -25, -9, -17, -31, -14, -33, -3, -27, -20, -26, 1, -14, -14, -19, -26, -19, -26, -2, -13, -6, -29, -30, -52, -15, -30, -38, -33, -33, -48, -21, -33, -28, -14, -12, -22, -15, -31, -12, -26, -23, -10, -43, -35, -19, -45, -46, -16, -40, -49, -16, -37, -20, -22, -43, 0, -41, -31, -14, -17, -40, -28, -5, -19, -6, -19, -10, -9, -26, -12, 12, 10, 3, -4, 16, 34, 28, -16, 7, 12, 2, 0, -4, -31, -7, -20, 16, -21, 0, 2, -19, -23, -18, 8, -11, -2, -16, 7, -4, 19, -14, -21, -22, 1, -20, -5, -13, 7, -20, 4, -16, -14, 26, 0, 4, 10, 13, -14, -6, -15, -3, -30, -19, 1, -10, -9, -31, -38, 1, -8, -21, 15, 10, -6, -1, 5, -21, -22, -11, 0, 3, -29, -32, -18, -8, -19, -11, -14, -29, -24, -21, -1, -28, -1, 16, 1, -38, -18}
, {-39, -3, 19, -3, 19, 31, 19, -15, 2, -11, 20, 0, 38, 13, -7, -21, -27, 30, -5, -20, -4, 13, -1, 17, 3, -20, -13, -2, 11, 23, -12, -11, -29, -24, -15, -18, 31, -16, -12, 6, 12, 0, 18, -6, -12, -19, 5, -27, -1, 1, -17, 9, -10, -24, -7, -2, -20, -5, 8, 25, 0, -15, -27, 1, 20, -17, -40, 12, -10, -7, 1, -1, -7, -7, 10, -5, 6, -27, 21, 14, -13, -26, -3, -25, -1, -25, -13, 16, 22, -12, -34, 12, -10, -30, -47, -2, -3, -23, 0, 7, -3, 19, -9, 23, 9, -9, 12, 5, 15, 2, -10, 21, 8, 0, -17, 24, -1, 9, 8, -10, -28, 4, -21, -31, 7, 10, 26, 14, 39, 3, 18, -13, 26, -2, 3, 6, -29, -7, -9, 1, -29, -23, 12, -25, -16, 7, 8, 0, 19, 20, -12, -12, 13, 7, 48, 7, 32, 16, 17, 0, 22, 7, 10, -5, -4, 14, 24, 31, 19, -10, 16, 19, -4, -5, -1, 1, 16, 33, 38, 15, 28, -1, 33, -5, 9, -4, 19, 31, 4, 2, -17, -26, -15, 0, 14, 0, -17, 0, 19, 6, -18, 15, 0, 12, 4, -6, 9, 19, 23, 6, 1, 23, -20, -2, 1, -2, 1, -29, 9, -22, -16, -1, 19, 23, -7, 19, 16, 20, -22, 0, 22, -6, 2, -15, 4, 4, -5, 3, -4, -5, 4, 0, 0, -17, 9, -15, 17, -6, 26, 5, 4, -1, -11, -2, 0, 21, 6, 26, -4, 12, -21, 1, 19, 10, 12, 2, -12, 10, -15, 20, 2, -13, 1, -9, 18, -4, -3, 3, -3, -16, 3, -12, -14, -7, 10, -1, 13, 2, 0, -9, 7, 15, -5, 10, 1, -6, 16, 36, 8, 13, -4, -24, -8, 14, 5, -2, -1, -18, -30, -19, -25, -27, 6, -4, -17, 2, -9, 0, 0, -19, -29, 5, -20, -27, -27, -7, 14, 21, -16, -16, -16, 16, 14, -33, -4, -2, -6, 5, 22, -11, 27, 28, -14, 4, 33, -4, -22, -28, -12, 1, -33, 1, -26, -14, 15, 0, 2, 8, 24, -8, -26, -2, 0, 10, 38, 4, 24, 13, -26, 9, 0, -35, -13, 19, 17, 26, -6, -8, 27, -20, 19, 10, 30, 23, -5, 19, 24, -4, 0, 38, 3, 26, 14, 9, 20, 4, 25, 22, -3, 25, 4, -11, 31, 19, 5, 14, -1, 20, 24, -4, 20, 21, 23, -5, 27, 4, 19, 24, 22, -13, 27, 10, -2, -11, -8, 15, 23, 24, 2, 18, 28, 25, -6, 0, 32, -8, 30, 1, 11, 28, 14, 13, 7, -2, 15, 12, 12, 8, -13, 25, 1, 16, 0, 12, 8, 4, 13, 7, 28, 4, 19, 0, -8, 13, 18, 2, 0, 19, 20, 18, 24, -3, 7, 21, 1, -16, -12, 2, 14, -7, 21, 10, -17, 21, -17, 5, -25, -8, 11, -16, -12, -2, 15, 11, 2, -5, 0, -13, 8, 1, 14, -2, -6, 17, 7, 4, 7, 20, -21, -2, -18, 22, 7, 11, 13, 25, -11, 9, -8, -3, 8, -14, 30, 13, 21, -25, 1, -10, -3, -6, 15, 12, 0, -2, -34, -21, 1, 18, -12, -3, 3, -28, 6, -12, -9, -11, 10, 18, -5, 0, 3, 13, 0, 6, -19, 5, -13, 13, -13, -27, -20, 23, -5, 15, 20, 15, 25, 20, 4, 29, -33, -7, -2, 0, -19, 6, -27, -2, -10, -2, -10, 6, -5, -37, -9, 10, -12, -9, -11, 3, 2, 8, -11, 2, 2, -25, 11, -9, -13, 7, 13, -20, -8, 16, 5, 0, -16, 14, 0, -15, -28, -17, -6, 5, -5, 6, 6, 6, 28, 6, -11, -12, 0, 9, -18, -21, -17, -9, -16, -7, 3, -19, -22, 0, -13, -11, 7, -15, -10, -6, 0, -21, -24, -17, -26, -4, 8, -4, -20, 13, 18, 10, -23, -39, -7, -16, -18, 1, -2, -31, -37, -37, -42, -47, -23, -34, -20, -26, -18, -24, -31, -5, -13, -17, -7, -5, 20, -15, -2, 11, 4, 20, 5, -8, 7, -7, -4, -16, 17, -4, -15, -7, -10, 10, -7, -24, -13, -19, -10, 2, -20, 7, 2, 2, 2, -25, -15, -24, -30, -7, -38, -14, -5, 3, -32, -6, 19, -8, 15, 14, 11, -11, -11, -28, 9, -12, -6, -34, -22, 1, -5, 18, -25, -9, -4, -16, 18, -9, 2, 17, 8, 3, 15, -6, -13, 3, -2, -33, 2, 5, 8, 16, 1, -9, -19, 9, 3, -2, -8, -34, 15, -5, -16, 15, 0, 14, 14, -15, -23, -18, 8, -5, 5, -13, -14, 19, 2, 2, -17, -17, 21, -15, 13, -6, 16, -9, -23, 3, -6, -21, 11, 10, 13, 2, -4, 12, 10, -19, 7, -18, -6, 15, 23, 20, -5, 0, -18, -13, -7, 4, 6, 17, -12, -8, 8, -5, -7, -13, -7, -3, 4, 18, 8, 0, 18, -2, 18, 9, 17, 13, -10, 9, -14, -2, 13, 0, 16, -18, 4, 10, 18, -25, 9, -9, 5, -2, 8, -12, -25, 1, 5, 16, -17, 9, -15, -12, -16, -25, -1, 17, -9, -17, 26, 2, 14, 14, 14, 28, 34, 16, 16, -10, 13, 11, 19, 23, 20, 10, 14, -18, 6, 14, -15, 13, -7, -16, 11, 2, -14, 0, 8, -13, 20, 1, 9, 0, 5, 31, -8, 10, -12, 25, 24, 26, -8, -7, 4, -3, 15, 19, -16, -1, 18, 23, 0, 15, 8, -27, 2, 19, 15, 10, -10, 12, -14, 4, 6, 12, 12, 21, 9, 4, -2, -14, 8, -17, -13, -8, -21, -19, -24, 5, 16, 22, -14, 15, -14, -12, -25, 11, -61, -20, -31, -30, -26, -29, 1, 23, 0, -11, -16, 7, 17, 8, 6, -11, 2, 11, -11, -6, -11, -6, -24, -42, -35, -28, 4, -19, -11, 11, 18, 3, -32, 3, -15, -10, -13, -5, 13, -18, -13, 15, 15, -7, 11, 27, 5, 1, 4, 0, -10, 2, -28, -35, 12, -12, -2, -18, 3, 4, -15, -39, -26, -11, 12, -36, -24, 7, -8, 12, -22, 10, -11, -9, 4, 1, 12, 22, -19, 6, -8, -17, 14, -29, -13, 12, 12, 0, -29, -22, 2, -25, -7, -43, -6, 4, 7, 22, 5, 18, 31, 4, 24, 8, 23, 0, 11, 9, 0, 7, 24, 18, 13, -1, 0, -12, 11, 10, 0, 1, 28, 11, 25, 10, 19, -12, 18, 2, -18, -19, -5, 11, -3, 14, -8, -14, 0, 9, 13, 5, 27, 9, 18, 13, -9, 0, -7, -13, -4, 14, 15, -9, 1, 25, -16, -18, 1, -13, 24, -13, 4, 32, -9, 0, 3, 6, -8, 3, -7, -17, -2, -9, 30, 29, 8, -1, -13, 4, -18, 12, 1, -17, 17, 2, 12, -6, 14, 2, -21, -23, -29, -46, -22, -26, -47, -42, -46, -3, -15, -25, -21, -36, -10, 0, -10, -36, 8, -8, -20, 6, 0, -7, -4, 3, -7, -29, -26, -9, -11, -14, -2, -25, -22, -13, -11, -18, 6, -5, 0, -32, -21, -41, -26, 0, -14, -46, -37, 11, -6, -21, -25, -21, 15, -1, -6, -17, 4, -17, -4, -1, -22, -13, -21, -4, -40, -2, -22, -6, -48, -28, -30, -25, -9, -1, -28, -37, -8, -6, -34, -1, -40, -25, -39, -38, -21, -15, -26, -50, -28, -46, -17, -22, -55, -27, -28, -18, 14, 15, -8, -7, 18, 0, -8, -21, -18, 14, -21, -14, 7, 15, 21, 11, -15, 7, -2, -11, 18, 4, 19, -19, -15, 0, -20, -14, -18, -18, 18, 23, -8, 0, 17, 21, -7, -7, 10, 13, -23, -4, 16, 20, 5, -25, -12, -5, 2, 5, 13, 25, -15, -5, -16, -12, 6, -18, -10, 10, 8, -11, -15, -17, -9, 3, 14, -27, 11, 0, 4, -23, 20, 0, -21, 15, -17, 25, 10, -14, 5, -8, 23, 22, 16, 1, 14, -14, 22, 4, 10, 15, 5, 17, -25, -12, 26, 1, 10, 16, 1, -11, -5, 0, -16, 8, 18, 36, 9, 8, -16, 17, -1, -11, -16, 8, -21, -1, -17, -20, -4, -18, 13, -12, -14, 4, 22, -38, -34, 4, 6, 10, -27, 2, -24, 25, 14, 2, 34, -22, 13, 8, -3, 24, 28, 14, 28, 9, -2, 39, -14, 16, 52, 0, 8, -8, -10, -1, -3, 0, -5, 0, -37, -26, -19, 10, 36, 8, -19, -15, -4, -54, -25, 26, 12, 27, -5, -6, 20, 10, -30, 33, 8, 10, -10, 7, -12, 9, 3, -50, -26, -38, -35, -25, -14, -40, -23, -19, -14, -10, -22, -36, -31, 1, 8, -20, 2, -14, 20, 12, 21, 5, -13, -16, 15, -18, -6, -5, -21, -19, -16, -11, -14, 6, -34, -18, 4, 6, -7, -21, -30, -13, 5, -18, -37, -9, -9, -30, -29, -33, 4, 23, 3, -3, -16, 10, 14, -1, -41, -10, -6, -31, -35, -14, -38, -31, -6, -4, -11, -15, -17, -36, -31, -35, -16, -10, -44, -37, -38, -28, -24, -10, 1, -37, -39, -29, -4, -26, -4, -43, -13, -36, -16, -18, -51, -49, -45, -47, -27, -17, -6, 3, -9, -15, -8, 0, 2, 5, 7, 1, 3, -3, 3, -8, 8, -2, -2, 3, -16, -15, -28, 0, -30, -2, 2, 8, -2, 29, -1, -10, 12, 11, -11, -15, -13, -17, -32, 3, -30, -42, -2, -15, 9, -35, -3, 14, 47, 24, -7, -2, 1, 2, 4, 34, -7, 15, 3, -36, -30, -23, -20, 5, -8, -20, -13, -4, 6, 24, 15, 0, -37, -26, 8, -22, -9, 2, -6, -4, 17, -8, -8, -13, -26, -10, -18, -8, 1, 0, -24, 15, -9, 19, 7, 18, -11, -15, 19, 21, 0, 17, 4, -2, -3, -15, -12, 16, -2, -19, -8, 16, -13, 22, -19, 14, 8, -9, -15, -10, -14, 8, 21, 1, 16, 25, -13, -1, 18, 4, 4, -20, -28, -10, -17, 11, 12, -6, 17, 12, -3, 7, 18, 19, -1, -7, 5, 18, 12, -2, -18, -1, 22, 13, -6, 14, 12, 20, 19, -6, -6, -14, -13, -4, -11, 0, -16, 5, -17, -11, 19, 17, -12, 14, 2, 17, 3, -7, 0, 3, -10, -8, -8, -8, 14, 13, -19, -16, -37, -27, -29, -38, -46, -10, -3, -26, 2, -15, -14, -8, -18, -16, -25, 17, -24, -3, -2, -14, 0, -21, 23, -11, 17, -12, 0, 14, -15, -2, -21, 9, -22, -3, 1, -16, -26, -2, -9, -30, -1, -4, -17, -12, -10, -19, -32, -32, -35, -12, 11, 20, -7, 17, 0, -15, -1, -35, -31, -45, -15, -32, -1, -14, -6, -21, 0, -5, -6, -8, -11, 11, -1, -30, -23, -29, -7, -22, 2, -18, -30, 0, 9, -4, -19, -22, -38, -32, -33, -22, -40, -49, -29, -10, -35, -22, -5, -23, -50, -6, -34, -8, -19, -11, -2, -21, -27, -19, -18, 10, 0, -6, -17, -16, 6, 14, -23, 12, 7, 3, -14, -14, -13, -33, -4, -17, -21, -16, 14, 4, -28, 1, 3, -6, -26, -6, -4, -35, -24, -11, 8, -14, -31, -22, -5, 1, -3, -3, 0, -6, -6, -30, -14, -18, -23, -3, -3, 6, 6, -13, -20, -35, -28, 8, 0, 14, -20, 0, -28, -14, -4, -7, 2, -2, 0, -10, -14, -25, -18, -9, 0, -22, 0, -41, -28, -37, -3, -33, -27, -1, -1, 1, 13, -13, 3, -4, -18, 0, 26, -21, -4, -1, 15, 12, 19, -16, -6, -4, -6, 23, -1, 12, 16, 0, -17, 17, 8, 2, 6, 23, -2, 3, 7, -14, -15, -21, 3, 12, 10, -11, 18, 21, -22, 6, 3, 21, 3, 14, -4, -14, 4, -1, -20, 16, 12, 22, -17, 15, -16, -4, 0, -19, 12, 6, 14, 5, -13, -23, 16, 9, -4, -1, -5, 9, 26, -4, -3, -2, -17, -19, 8, -12, -19, -7, 15, -21, -18, -11, -19, 3, -4, 15, -1, 16, -8, -17, 12, -21, -3, -29, -2, 6, -25, -14, -5, -6, -4, 9, -16, -24, 12, -24, 16, 13, 22, 5, -13, 6, -22, -16, 3, -3, -2, -1, -15, 13, 1, 15, -3, -6, 15, -22, -15, -2, 10, -9, 10, 3, -19, 13, 0, 8, -1, 20, 19, -16, 10, 6, -17, 9, 14, -7, -25, 2, 5, -16, 15, -4, -5, 16, -11, -8, 16, 24, 15, 10, 0, -4, -20, 12, -16, -14, 7, 13, 14, -7, -25, 16, -6, -9, -12, 12, 10, -15, -18, -18, -17, 9, -10, -42, -7, 0, -17, -16, -14, 3, -5, -5, 8, -14, -6, -18, -5, 6, -5, 8, 10, -10, -1, -16, 0, 14, 24, -7, 18, -22, 2, 5, -1, 5, -11, -9, -12, 15, -23, 14, -30, 11, 17, -26, -19, 14, -16, 2, -11, -6, -8, -15, 9, 9, 28, 0, -8, -12, 27, 7, -3, -20, 4, -25, -11, -4, 6, -12, -6, -9, -21, 20, -13, 12, 19, -13, 0, -2, 12, 11, -6, -18, 7, 14, 9, -16, 14, -14, -8, -14, 18, -25, -27, -28, 6, 7, -11, -34, -31, -5, -4, -13, -14, -1, -7, -1, 13, -10, 6, -1, -1, -22, -6, 15, 0, -7, -22, 1, 22, -14, 19, 15, -4, -4, 7, -7, -3, -16, -18, -17, 4, -24, 1, -26, 20, -8, -24, -15, 10, 10, -15, 11, 4, 3, 3, 7, 12, -10, 3, 12, -12, 8, 4, -8, -13, 13, -13, -18, -10, -9, -31, -8, 16, -24, -14, -32, 0, 13, -15, -4, 24, 14, 7, -12, -6, 9, 17, 0, 17, 2, 2, 7, -13, 13, -10, 5, 15, -18, 2, 8, -1, 17, -20, 11, 17, -20, -11, -6, -19, -13, -5, 20, -4, -11, -14, -7, 19, -2, 18, -6, 17, 12, -16, -2, 7, 18, 4, -14, -2, -21, -15, 16, -18, 15, 8, 19, 20, -5, 11, -1, 16, -26, -12, 14, -14, -12, 12, -13, -2, 2, 10, 19, -9, 11, 19, 18, 5, -6, 22, 5, 21, 3, -17, -9, 3, 11, 12, 11, -5, -2, 6, 12, 0, 13, 9, 2, 22, -15, -1, -5, 10, -6, -18, 0, -4, 6, 13, 17, -19, -3, 6, -8, -8, -14, 19, -17, 9, 10, 3, 8, -16, 0, 16, 14, -9, -23, 9, -25, 17, 7, 16, -2, 13, 15, -4, 4, -17, -18, 18, 3, 0, -6, -9, -11, -26, 10, -1, 0, -3, -16, -20, 1, -17, 17, -22, -10, 1, -21, -9, 1, -10, -8, -20, -1, 2, 2, 19, 18, -5, -17, 16, 6, -20, 0, 11, 13, -15, -12, -24, -6, 1, 14, -15, 6, 11, -5, 10, 15, -10, -16, -11, 13, -17, 9, -13, 0, -22, -8, 14, -18, -11, 9, -1, 14, -2, 13, 4, 16, 15, 3, 13, -12, -9, 15, -13, -22, 23, 0, 22, -21, -14, 9, -16, -3, -15, 19, -14, 21, 18, 12, 20, 23, -1, -10, -3, -1, -14, 23, -24, 23, 6, -4, -10, 15, 10, -15, -10, 14, 23, 18, 22, 2, 17, 7, 21, -12, -18, 10, 17, 4, 25, 29, 15, 25, 13, 23, 12, 9, 11, 23, 21, -1, -1, 10, -7, 19, -7, 13, 5, 28, 22, 10, 8, 0, -13, -20, -6, 9, 3, 2, -3, -7, -3, -12, 1, 14, 25, -11, 19, 0, -8, 2, -9, 18, -11, 2, -5, -7, -14, 12, 3, 10, -20, -20, -8, -12, 8, -12, -3, -17, -23, -10, 14, -4, -21, 11, 4, -8, 6, 3, 1, 10, -18, 13, -13, -15, 5, -20, -18, 13, 14, 7, 2, 0, 1, -18, -7, -19, 14, -20, -26, 13, 14, 11, -25, -15, -16, 0, 15, -14, 0, 18, -14, -13, 13, 6, -3, 14, -23, -4, 16, 10, -19, -26, -6, -8, -18, -9, 5, -20, -11, 18, -17, -19, -14, -23, 13, -14, 16, -7, 4, 11, 6, 12, 9, -23, 4, 14, -13, 4, -16, 8, -5, 4, 3, -11, 16, 0, 8, 26, 27, 35, 13, 28, 33, 15, 3, 5, 33, 26, 18, 4, 14, -14, -16, 8, -2, 2, 2, 24, 5, 34, -6, 0, 8, 2, 24, 3, 0, -3, 6, -28, 6, -9, 0, 5, 10, -1, 22, 2, 15, 5, 11, -3, 18, 15, 28, 23, 1, 31, -7, 11, -7, -10, 19, -1, -1, 23, 13, 3, -15, 4, 20, 6, 16, 1, 28, 2, 2, -3, 35, 1, 27, 16, 13, 15, 31, 5, -3, 12, 5, -17, -3, 11, 6, 8, 27, 0, 31, 13, -10, -17, 20, 35, 15, 13, 5, 33, -2, 6, 18, 1, 10, 8, 27, -7, -11, 17, -23, 1, -6, 19, 11, 16, -5, -15, 15, 15, 26, 20, 3, 4, 0, 9, -4, 2, 1, 30, 9, 13, -8, 6, 6, 16, -3, -5, 16, 18, -9, 3, 10, 5, 0, 19, 27, 9, 21, 22, 30, 1, -3, -9, 24, 18, -6, -20, -20, 15, 12, -4, 32, 27, 25, 25, 12, 31, 20, -5, 12, 13, 22, -18, -3, -13, 13, 3, 36, 13, 17, 7, -6, 16, 39, 15, -1, -1, -5, 26, 8, 11, 5, 35, 33, 17, 30, 25, 21, 20, 32, 12, 5, 34, -11, 24, 5, 4, 0, 29, 24, 19, 48, 23, 4, 0, 37, 37, 10, 21, 11, 32, -8, 5, 0, -10, 3, -13, 15, 17, -6, 10, 11, 24, 34, 32, 6, 16, 8, 19, 4, 8, -2, 2, 30, 25, 14, 9, 0, -7, 0, 16, 6, -9, 9, 15, 25, 6, 6, 23, 0, 13, 26, 1, -4, 36, 31, 32, 16, -2, 29, 9, 18, 15, 26, 23, 20, 15, 28, 25, 4, 0, 11, -12, -7, -3, -12, 4, 22, 2, 15, -15, -15, -17, -10, -15, 1, 23, 21, 13, 33, -1, 15, 23, 20, -16, -5, -1, -26, -4, 18, 18, -12, 24, 22, 7, -4, -11, -21, -15, -1, -10, 3, 17, 8, 1, 14, 6, -4, 13, -3, 18, 17, -13, -4, 18, -8, 11, -8, 0, -10, -2, 24, 18, 23, 11, 21, 15, -2, 11, 6, -5, 13, 16, 18, 14, 2, 19, 25, 23, 4, 5, -10, 17, -5, 16, 16, 12, 11, 23, 19, 24, -2, 28}
}
;

#undef INPUT_SAMPLES
#undef FC_UNITS