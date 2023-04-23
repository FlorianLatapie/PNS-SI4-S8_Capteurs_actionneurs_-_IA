#define SINGLE_FILE
/**
  ******************************************************************************
  * @file    number.hh
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    2 february 2021
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef __NUMBER_H__
#define __NUMBER_H__

#include <stdint.h>

#define FIXED_POINT	9	// Fixed point scaling factor, set to 0 when using floating point
#define NUMBER_MIN	-32768	// Max value for this numeric type
#define NUMBER_MAX	32767	// Min value for this numeric type
typedef int16_t number_t;		// Standard size numeric type used for weights and activations
typedef int32_t long_number_t;	// Long numeric type used for intermediate results

#ifndef min
static inline long_number_t min(long_number_t a, long_number_t b) {
	if (a <= b)
		return a;
	return b;
}
#endif

#ifndef max
static inline long_number_t max(long_number_t a, long_number_t b) {
	if (a >= b)
		return a;
	return b;
}
#endif

#if FIXED_POINT > 0 // Scaling/clamping for fixed-point representation
static inline long_number_t scale_number_t(long_number_t number) {
	return number >> FIXED_POINT;
}
static inline number_t clamp_to_number_t(long_number_t number) {
	return (number_t) max(NUMBER_MIN, min(NUMBER_MAX, number));
}
#else // No scaling/clamping required for floating-point
static inline long_number_t scale_number_t(long_number_t number) {
	return number;
}
static inline number_t clamp_to_number_t(long_number_t number) {
	return (number_t) number;
}
#endif


#endif //__NUMBER_H__
/**
  ******************************************************************************
  * @file    maxpool.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_CHANNELS  1
#define INPUT_SAMPLES   16000
#define POOL_SIZE       10
#define POOL_STRIDE     10
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_24_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_24(
  const number_t input[INPUT_CHANNELS][INPUT_SAMPLES], 	    // IN
  number_t output[INPUT_CHANNELS][POOL_LENGTH]) {	// OUT

  unsigned short pos_x, k; 	// loop indexes for output volume
  unsigned int x;
  number_t max, tmp; 

  for (k = 0; k < INPUT_CHANNELS; k++) 
    for (pos_x = 0; pos_x < POOL_LENGTH; pos_x++) {
#ifdef ACTIVATION_LINEAR
      max = input[k][pos_x*POOL_STRIDE];
      x = 1;
#elif defined(ACTIVATION_RELU)
      max = 0;
      x = 0;
#endif
      for (; x < POOL_SIZE; x++) {
        tmp = input[k][(pos_x*POOL_STRIDE)+x]; 
        if (max < tmp)
          max = tmp;
      }
      output[k][pos_x] = max; 
    }
}

#undef INPUT_CHANNELS  
#undef INPUT_SAMPLES
#undef POOL_SIZE
#undef POOL_STRIDE
#undef POOL_PAD
#undef POOL_LENGTH
#undef ACTIVATION_LINEAR
/**
  ******************************************************************************
  * @file    conv.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_CHANNELS      1
#define INPUT_SAMPLES       1600
#define CONV_FILTERS        32
#define CONV_KERNEL_SIZE    80
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_18_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_18(
  const number_t input[INPUT_CHANNELS][INPUT_SAMPLES],               // IN
  const number_t kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE], // IN

  const number_t bias[CONV_FILTERS],						                // IN

  number_t output[CONV_FILTERS][CONV_OUTSAMPLES]) {               // OUT

  unsigned short pos_x, z, k; 	// loop indexes for output volume
  unsigned short x;
  short input_x;
  long_number_t	kernel_mac;
  static long_number_t	output_acc[CONV_OUTSAMPLES];
  long_number_t tmp;

  for (k = 0; k < CONV_FILTERS; k++) { 
    for (pos_x = 0; pos_x < CONV_OUTSAMPLES; pos_x++) { 
      output_acc[pos_x] = 0;
	    for (z = 0; z < INPUT_CHANNELS; z++) {

        kernel_mac = 0; 
        for (x = 0; x < CONV_KERNEL_SIZE; x++) {
          input_x = pos_x * CONV_STRIDE - ZEROPADDING_LEFT + x;
          if (input_x < 0 || input_x >= INPUT_SAMPLES) // ZeroPadding1D
            tmp = 0;
          else
            tmp = input[z][input_x] * kernel[k][z][x]; 
          kernel_mac = kernel_mac + tmp; 
        }

	      output_acc[pos_x] = output_acc[pos_x] + kernel_mac; 
      }
      output_acc[pos_x] = scale_number_t(output_acc[pos_x]);

      output_acc[pos_x] = output_acc[pos_x] + bias[k]; 

    }

    for (pos_x = 0; pos_x < CONV_OUTSAMPLES; pos_x++) {
#ifdef ACTIVATION_LINEAR
      output[k][pos_x] = clamp_to_number_t(output_acc[pos_x]);
#elif defined(ACTIVATION_RELU)
      // Activation function: ReLU
      if (output_acc[pos_x] < 0)
        output[k][pos_x] = 0;
      else
        output[k][pos_x] = clamp_to_number_t(output_acc[pos_x]);
#endif
    }
  }
}

#undef INPUT_CHANNELS
#undef INPUT_SAMPLES
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE
#undef CONV_STRIDE
#undef ZEROPADDING_LEFT
#undef ZEROPADDING_RIGHT
#undef CONV_OUTSAMPLES
#undef ACTIVATION_RELU
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


const int16_t conv1d_18_bias[CONV_FILTERS] = {1, 6, 1, 10, 7, 2, 1, 23, 11, 1, -11, 4, 3, 0, 2, 6, -27, 7, 2, 2, 1, 0, -15, 59, 11, 75, 1, 6, -7, 8, 2, 53}
;

const int16_t conv1d_18_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{-17, 1, 14, -20, 2, -31, 14, 13, -6, 18, -24, -13, 25, -20, 3, -16, 26, -22, 10, -22, 27, -15, -9, -14, 14, -22, 0, 16, -26, 2, -2, 8, 8, -18, 31, -12, -1, -11, 17, 13, 8, -5, 10, -2, 28, -7, 17, -20, 30, 17, -15, 36, -3, 10, -17, 29, -11, -6, -10, 23, 2, -1, -34, 18, -40, 27, -29, 10, 20, -35, 24, -28, -5, -8, -21, 11, 25, -29, 29, -12}
}
, {{0, 0, 1, 8, 24, 18, -6, 0, 7, -3, -2, 2, -1, 6, 21, 19, -2, -13, -14, -10, 13, 10, -7, 17, 27, 14, 22, 18, -3, 7, 29, -7, -13, 0, -19, -17, -5, 19, -8, -10, 2, -21, -9, 20, -9, 0, -13, -16, 19, 9, 12, 7, -4, -21, -6, -10, 6, 8, 8, 4, 5, -6, 9, 10, 15, 14, -20, -22, -15, -14, -2, 7, 0, -4, -24, -11, 7, 24, 16, 3}
}
, {{-4, 25, -17, 17, 5, 31, -21, 34, -30, 23, -11, -5, 8, 2, 6, -8, -3, 0, -4, -18, 14, -18, -11, -16, 5, 7, -21, 14, 14, 11, 1, -2, 10, 21, -27, 19, -18, 5, 16, -18, -3, 8, 8, 8, 0, 0, -25, 12, -14, 13, 4, 2, 8, 3, -1, -26, 13, -22, 13, -6, 8, 13, 0, -18, 12, 8, 19, -34, 29, -27, 30, -5, 19, -18, 21, 7, 14, 5, 9, 25}
}
, {{-8, 9, -20, -27, -30, 3, -17, -19, -34, -1, -24, 6, -11, -8, -11, 9, 4, -2, -14, 19, 0, 26, 22, -5, 31, -18, -16, 0, 10, -19, -17, -24, -25, -11, -8, -5, 5, -24, 16, 20, 23, 30, 2, -18, 20, 11, 3, -16, -11, 11, 5, 6, -17, 6, -14, -6, -2, 16, -6, -5, 10, -15, -8, -12, 5, -17, 21, -17, -5, -2, -11, 19, -30, 11, -21, 29, -8, 1, 11, -8}
}
, {{-9, 0, -32, 22, 11, 8, 17, 6, 25, 0, -1, 1, 5, -22, -5, -20, -10, -7, 27, -31, 14, -6, -1, 2, -2, -23, -5, -8, 1, -22, 3, -28, -11, 16, 8, 7, -11, -1, -12, 13, 14, 6, 4, -28, 12, -11, 1, 0, -6, 10, -1, -14, 20, 0, 2, -11, 24, -15, -12, 11, 16, -9, -8, 9, -22, -6, -16, 22, 21, -3, -18, 9, 1, 18, -17, 24, -34, 27, -17, 10}
}
, {{6, -7, -4, 9, 18, -19, -18, 21, 9, -3, -14, -10, -9, 0, -5, -6, -4, 18, -16, 6, -24, 19, -18, -15, 9, 7, -18, -9, 12, -25, 11, -3, -12, 17, 25, -13, 25, -2, 0, 8, -22, 14, -7, 8, -13, 30, -9, 3, 9, -23, 40, -47, 37, -14, -5, 3, -16, 4, -13, -14, -12, 9, 16, 24, -29, 29, -16, 32, -24, -7, -11, 15, 15, 3, -7, 32, -22, -1, 1, -2}
}
, {{1, -13, 2, 6, -7, 14, -6, -13, -15, 18, -32, 20, 0, -3, -7, 7, 0, -1, 3, -5, 20, -24, 9, -20, -9, -16, 2, 18, -4, -15, 12, 4, -13, 23, 4, 15, 9, 4, -26, -6, -32, 22, -28, 12, 6, 2, -7, 10, -25, -19, -29, 9, -8, -19, 26, -23, 14, 6, 2, 6, -17, -7, -12, 10, 18, 21, -9, -3, 21, -4, 26, -16, 7, 20, -12, 30, -27, 19, -14, 12}
}
, {{9, -5, 15, -10, -9, 13, -19, -15, -12, -21, -24, -9, -18, -34, -3, -5, -27, -12, -33, -15, -38, -17, -30, -37, -28, -19, -38, -33, -22, -44, -37, -35, -26, -47, -46, -41, -49, -47, -44, -26, -23, -44, -48, -35, -59, -41, -42, -45, -44, -36, -39, -25, -11, -19, -17, -9, -26, -25, -22, -19, -4, -5, -12, -6, -7, -1, -18, 1, -22, -16, 5, -14, -20, -16, -7, -13, -4, 1, -1, 4}
}
, {{27, 0, 29, -23, 0, 15, 10, 13, -30, 12, 33, -15, -8, 18, -3, -3, -22, 9, -10, -8, 13, -7, -8, 24, 0, -12, 17, -8, 39, -35, 30, -18, -21, 24, 0, -18, 28, -20, 27, -21, -5, 0, 8, -14, 14, 5, -13, -12, 24, -10, 23, -14, -5, 15, -17, -20, 9, 16, -39, 26, -36, -20, -3, -15, -16, -3, -35, -6, -25, -5, -21, 10, -9, -4, -26, 10, 6, 14, 4, -5}
}
, {{5, -14, 0, -10, -2, 0, -19, 8, 9, -13, 25, -7, 3, 17, 14, 4, 28, 19, -11, -5, -3, -14, 4, 0, 11, 26, 3, 34, -20, 31, -31, 31, -27, 19, -8, -15, -9, -24, 20, -2, 18, -4, 23, 16, -11, 13, 20, 5, 7, -10, 26, -12, 26, -10, -10, -12, 7, -6, 4, -24, -11, -7, 1, -1, 5, -22, 27, -23, 6, -25, 11, -26, 31, -5, 31, 26, 0, 24, -24, 37}
}
, {{2, 8, 16, -7, -8, 0, -5, 14, -4, -13, -3, -7, 13, 9, 23, 16, -4, 11, 10, 11, 0, -12, -2, 14, 20, 9, 3, 9, -8, 9, 32, 6, -9, 0, -1, -5, -17, -12, 11, 29, 28, 11, 16, 9, 1, 7, 20, 33, 20, 3, 6, 7, 19, 17, -14, -12, 10, 6, 26, 25, -5, -3, -1, -10, -11, 15, 18, 7, 29, 25, 6, 9, 10, 22, 25, 15, 7, -11, -5, 16}
}
, {{-6, 3, -3, -30, 10, -31, 25, -14, 9, -26, 6, -1, 6, -6, -26, 6, 0, -17, -12, -3, 9, 28, -19, 27, 11, 9, 3, -27, 25, -13, 21, -7, -16, 31, -12, 13, 0, 1, -31, 30, -25, 2, -16, 10, 8, 10, -12, 0, -7, -8, 19, -16, -14, 22, -16, -8, -26, 17, 1, 0, 21, -23, -12, 19, 14, -10, -15, 3, -15, 16, -13, 11, -3, 9, 20, 0, -20, 22, -16, 25}
}
, {{5, -25, 25, -29, 1, -1, 20, 7, 14, -9, -13, -3, -9, 31, -14, 5, -26, 34, -9, 7, 8, 14, 6, -12, -19, 28, 8, 2, -27, 20, -6, -9, -8, 22, -26, 0, -21, 15, -12, 7, -14, 19, -24, 2, -15, 2, -2, -23, 8, -18, 5, 21, -19, 19, -1, 18, 2, -15, -24, 25, -17, -25, -1, 18, 12, 14, -29, 16, -25, -3, 1, 11, 20, 11, -9, -20, 20, 22, -35, 20}
}
, {{7, 11, -2, -32, -26, 0, 0, -21, -34, -29, -24, 9, 23, -4, -31, -24, -15, -15, 10, 24, 19, 21, -6, -21, -5, 7, 4, -1, -9, 0, 0, -28, -9, 7, -4, 26, 27, -16, -28, -7, 17, 11, 3, 30, 26, 8, 13, 1, -6, 9, 21, 11, 2, -6, -10, 0, 4, 2, -4, 14, 16, -15, -11, 5, -14, 13, 27, -2, -25, -14, -3, -3, -26, -8, 2, -5, 0, -15, -38, -18}
}
, {{10, 3, 18, 4, 22, -8, 16, 3, 0, -25, 0, -14, 13, -31, 26, 22, -19, -16, 13, 7, 7, -36, 28, -22, 8, 26, -8, 20, 3, 2, -31, 8, 2, 18, -20, 20, 2, -31, 6, -38, 15, -35, 4, -24, -18, 21, -15, -13, -6, 11, -39, 26, -5, -1, -5, 2, -5, 17, -13, 11, 7, 0, 23, -3, 20, -8, 6, -2, -20, 27, -8, 2, 11, 1, -1, -8, 19, 0, -20, 0}
}
, {{0, 0, -19, 0, -6, 7, 12, -16, -21, 14, -23, 12, -29, 14, 11, 15, 6, 11, 23, -8, 3, 22, 14, 0, 10, -2, 8, -16, 10, -30, 33, -8, -7, -12, 14, -10, -17, 4, -13, -5, 25, -41, 17, 23, -21, 27, -4, -9, -6, 9, 0, -31, 9, -11, 5, -11, 21, -38, 6, -35, 25, -6, -20, 5, 0, 20, -21, 22, -9, 18, 2, 13, 21, 6, 18, -20, 24, 1, -16, 11}
}
, {{11, 14, -12, 16, 6, 6, -12, -26, -26, -31, 0, -17, 8, 3, -22, -5, -5, -3, 11, -16, 19, 14, -16, -7, -17, -13, -5, -20, -2, -29, -4, 4, 3, -10, -31, -21, -7, 3, 10, 25, -8, 3, 9, -12, 7, 17, 16, -9, -23, 4, 8, -22, -16, 5, 0, -17, 1, -10, 23, -3, 6, -11, 24, -1, 21, 17, 14, 0, -15, -32, 6, -2, -20, 4, -16, 9, -7, 11, -14, 9}
}
, {{13, -27, 24, 13, 8, -15, -15, 25, -23, 11, -16, 8, -21, 5, 9, 0, -36, -9, -10, -20, 16, -9, -8, -9, -14, 21, -35, 17, 15, -5, -14, -21, 7, 14, 3, 0, 8, -3, 19, -5, -12, 21, -9, 18, 18, -17, 9, -20, 12, -15, 24, -7, -4, 28, -10, 17, -15, 10, 0, 3, -9, 24, -8, 15, -38, 30, -28, -5, -6, 4, -12, 26, -31, 9, 9, 12, -9, 0, 19, -14}
}
, {{15, 10, 5, -4, 22, -25, 20, -12, 0, 14, 0, -5, -11, -13, -26, 23, -7, -24, -6, -25, -6, -24, 7, -29, 22, 14, -23, -15, -8, 26, -26, -4, 7, 13, 23, -5, 2, 33, -33, 31, -5, 18, -14, -15, -5, 7, -16, 23, -30, 34, -16, 15, 12, 13, -13, 22, 0, -6, 1, 0, 7, 6, -10, -1, -6, 0, 7, -23, 5, -23, 3, -28, 21, 22, -36, 25, -16, 7, -6, 4}
}
, {{-16, -1, 0, 8, -20, 14, -1, -1, 8, -17, -15, 20, 11, 0, -12, -24, -7, -25, 4, 5, -12, 4, 20, -15, -16, 22, -24, -1, -12, 7, -3, -14, 20, -21, 4, 29, -13, 29, -3, 25, 13, -1, -11, -17, 6, -20, 28, 11, -8, 12, 8, 17, 5, -27, 13, -21, -13, 28, -19, 23, 13, 2, -7, -9, -17, 25, -30, 27, -38, 25, -15, 11, 7, -6, 0, -1, 22, -3, -18, 1}
}
, {{2, 3, -20, 8, 5, -15, 0, -1, -9, 15, -22, 7, 15, 10, 6, -23, 31, -11, -5, -19, 2, -22, 31, -37, 19, 12, -9, 16, 13, -9, -15, 10, -11, 21, -30, 14, 5, 15, -25, 24, -33, 1, 8, 5, 4, -1, 10, -6, -20, 10, 5, -24, -18, 7, -9, -19, 15, -8, 10, 0, -30, 19, -36, 24, -14, 0, 8, 14, 11, -7, -15, 3, 17, -2, 12, 4, 16, 14, -27, 9}
}
, {{14, 4, 1, 12, 29, 7, 7, 21, 16, -14, 14, 25, 0, 3, 7, -11, 1, 30, 24, 15, -2, 1, 9, 20, -12, 10, 16, 5, 26, 20, -7, 25, 27, -5, 19, 19, -7, 0, -6, 24, 8, 11, -1, -10, 19, 29, 4, -5, 18, 2, 22, 10, -10, 8, 5, -15, -2, 1, 2, 15, -9, -8, 10, -7, 18, 16, 21, 17, 9, -4, 2, -4, -12, 10, -3, -3, 8, -23, -15, 18}
}
, {{-4, -26, 23, -3, 6, -8, 12, -20, 8, 8, 22, -11, 2, -17, -23, -26, 9, 27, -2, -25, -39, -8, 16, -4, -27, 15, -2, 21, -14, 19, 18, -2, -15, 17, 8, -17, -15, -10, -26, 26, 20, 8, 12, -5, -10, 17, -30, -39, -10, 1, 12, -3, -28, -7, -16, 6, -3, -43, -37, -43, -13, -7, 29, 39, 26, 15, 14, 0, 20, 1, 0, 9, 19, -19, -12, 0, 17, -9, -2, 0}
}
, {{0, -15, -4, 4, 6, 5, -25, -17, -22, -22, -19, -15, 8, 0, 15, 8, -5, -26, -18, -24, 1, 1, -19, -8, -1, -14, -8, -1, -25, -13, -12, -17, 8, -21, 9, -4, 15, 5, -7, -3, -16, 3, -12, 0, 0, -21, -6, -20, -8, -6, -24, -4, -6, -8, -24, -14, -1, -11, -4, 1, 12, -9, -5, 0, -10, 0, -5, -9, 3, -2, 3, 15, 1, -10, -12, -18, -20, -26, -21, 2}
}
, {{7, 14, 15, 23, 8, -14, 9, 20, -8, 21, 6, -20, 20, 0, 16, 34, 21, 6, -25, -1, 30, 19, -4, 12, -22, -31, 20, -5, 18, 38, -20, -48, -28, 11, 10, 28, 22, -31, -54, -19, -5, -20, 28, -3, -40, 4, 0, -12, -12, -1, -26, -29, -10, -9, -2, -25, 4, 21, 26, 14, -6, 22, -7, -32, -9, 17, -1, -24, -27, -8, 11, 23, 25, -1, 17, -3, -37, -8, 3, 0}
}
, {{-1, -13, 2, -9, 5, -25, -19, -20, 4, 0, -24, -31, -27, -25, -24, -32, -28, -27, -30, 3, 1, -16, -19, 0, -7, 11, -23, -23, 2, -36, -1, -3, 12, 0, -16, -14, -2, -3, -14, -32, -15, -3, -5, 4, -1, -4, -6, -31, -24, -24, -2, -21, -5, -22, 4, -26, -1, -6, -23, -21, 0, 8, 15, 6, 7, -4, -17, -4, 18, 5, 4, -17, -8, -17, -16, -4, -3, -22, 4, -2}
}
, {{9, 13, -12, 10, 6, -21, -20, -5, -16, -29, 27, -31, 25, -5, 29, -15, -13, -1, -1, -3, 22, -30, 29, 1, -2, 18, -15, 16, 19, 25, -32, 7, 14, -15, 29, -34, 20, -10, -2, 17, -9, 8, -32, 31, -15, 23, 0, 18, -17, -13, -7, 12, -7, 16, 14, -24, -4, 2, -13, -10, 3, -27, -12, 8, -14, -10, -13, -9, 11, 24, -1, -12, -15, 6, 0, 31, -19, -2, 18, 7}
}
, {{-5, -17, 20, 0, -15, 7, 5, 22, -2, 13, 23, 9, -3, 12, 0, -5, -14, 2, 12, 4, 1, -7, 17, 24, -13, 30, 6, 6, 25, 7, 5, 8, -7, 5, -26, 24, -28, 3, -16, 22, -16, 3, 19, -12, -18, 5, -28, 9, -18, 9, 18, 20, 33, -7, 28, -18, 7, -21, 8, -26, 1, 17, -24, -7, -13, -4, -6, -30, 36, -28, 33, 7, 21, 6, -7, 3, 11, -3, -16, 11}
}
, {{-16, 13, -14, -19, -16, -29, 11, -24, -4, -10, 0, 8, 2, -10, -5, -28, 23, -33, 15, -5, -22, -6, -19, -8, 2, -22, 11, 18, 15, -10, -30, 20, -8, -15, -33, 14, -33, 7, -5, 0, -12, 0, -8, -23, 21, 3, 15, -15, -7, 25, 18, -4, -3, 22, -17, 26, 8, 8, 24, 27, 3, 2, 3, -10, 1, -12, 5, 29, -1, 1, 6, 8, -12, 24, 19, 30, -4, 31, 20, 29}
}
, {{-9, 32, -43, 30, -4, -26, -4, -2, 22, -12, 12, 21, 16, -24, 21, 16, 17, 9, 24, -32, -13, 22, -30, 10, 11, 16, 21, -25, 10, 16, 19, -9, 6, -29, 5, 4, 15, -35, -3, 9, -6, -21, -25, 25, -29, 1, 18, -2, 18, 16, -29, -21, -13, -10, -27, -9, -30, 6, -21, 8, -10, 23, -4, -8, -16, -27, 33, -18, -6, -7, -20, 10, -4, 10, 11, -9, 28, 3, -14, -15}
}
, {{22, 4, -11, 6, -6, -10, 5, -26, 20, -14, 11, 8, 11, -15, -4, -19, -13, -23, 25, -21, 26, 4, 10, -8, -23, 24, -20, 17, -14, 15, 16, -3, 18, -19, -7, -2, 2, -27, 25, -28, 16, -15, 16, 19, -21, 14, -22, -17, 18, -12, 18, -20, -21, 15, -2, 15, 27, -32, 24, 1, -23, 24, -39, 25, -30, 22, 2, -10, -12, 20, 26, -12, 32, -12, -8, -1, 30, -21, 26, -21}
}
, {{-22, -18, -14, -1, 10, 2, -2, -10, -5, -10, -29, -40, -47, -31, -21, -9, 3, 3, 5, 8, -11, -13, -23, -16, 1, 6, 14, 7, -4, -8, -19, -30, 0, -20, -6, -14, -17, -22, -15, -5, -7, 14, -12, 3, -7, 2, -1, -9, -7, -7, 10, 1, 18, 2, 2, 0, -7, -7, 0, 0, 12, 8, 6, -10, 0, -24, -14, -18, 0, 11, 17, 10, 2, 11, 0, 10, -12, -17, -23, -21}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE
/**
  ******************************************************************************
  * @file    maxpool.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_CHANNELS  32
#define INPUT_SAMPLES   1521
#define POOL_SIZE       4
#define POOL_STRIDE     3
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_25_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_25(
  const number_t input[INPUT_CHANNELS][INPUT_SAMPLES], 	    // IN
  number_t output[INPUT_CHANNELS][POOL_LENGTH]) {	// OUT

  unsigned short pos_x, k; 	// loop indexes for output volume
  unsigned int x;
  number_t max, tmp; 

  for (k = 0; k < INPUT_CHANNELS; k++) 
    for (pos_x = 0; pos_x < POOL_LENGTH; pos_x++) {
#ifdef ACTIVATION_LINEAR
      max = input[k][pos_x*POOL_STRIDE];
      x = 1;
#elif defined(ACTIVATION_RELU)
      max = 0;
      x = 0;
#endif
      for (; x < POOL_SIZE; x++) {
        tmp = input[k][(pos_x*POOL_STRIDE)+x]; 
        if (max < tmp)
          max = tmp;
      }
      output[k][pos_x] = max; 
    }
}

#undef INPUT_CHANNELS  
#undef INPUT_SAMPLES
#undef POOL_SIZE
#undef POOL_STRIDE
#undef POOL_PAD
#undef POOL_LENGTH
#undef ACTIVATION_LINEAR
/**
  ******************************************************************************
  * @file    conv.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_CHANNELS      32
#define INPUT_SAMPLES       506
#define CONV_FILTERS        16
#define CONV_KERNEL_SIZE    3
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_19_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_19(
  const number_t input[INPUT_CHANNELS][INPUT_SAMPLES],               // IN
  const number_t kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE], // IN

  const number_t bias[CONV_FILTERS],						                // IN

  number_t output[CONV_FILTERS][CONV_OUTSAMPLES]) {               // OUT

  unsigned short pos_x, z, k; 	// loop indexes for output volume
  unsigned short x;
  short input_x;
  long_number_t	kernel_mac;
  static long_number_t	output_acc[CONV_OUTSAMPLES];
  long_number_t tmp;

  for (k = 0; k < CONV_FILTERS; k++) { 
    for (pos_x = 0; pos_x < CONV_OUTSAMPLES; pos_x++) { 
      output_acc[pos_x] = 0;
	    for (z = 0; z < INPUT_CHANNELS; z++) {

        kernel_mac = 0; 
        for (x = 0; x < CONV_KERNEL_SIZE; x++) {
          input_x = pos_x * CONV_STRIDE - ZEROPADDING_LEFT + x;
          if (input_x < 0 || input_x >= INPUT_SAMPLES) // ZeroPadding1D
            tmp = 0;
          else
            tmp = input[z][input_x] * kernel[k][z][x]; 
          kernel_mac = kernel_mac + tmp; 
        }

	      output_acc[pos_x] = output_acc[pos_x] + kernel_mac; 
      }
      output_acc[pos_x] = scale_number_t(output_acc[pos_x]);

      output_acc[pos_x] = output_acc[pos_x] + bias[k]; 

    }

    for (pos_x = 0; pos_x < CONV_OUTSAMPLES; pos_x++) {
#ifdef ACTIVATION_LINEAR
      output[k][pos_x] = clamp_to_number_t(output_acc[pos_x]);
#elif defined(ACTIVATION_RELU)
      // Activation function: ReLU
      if (output_acc[pos_x] < 0)
        output[k][pos_x] = 0;
      else
        output[k][pos_x] = clamp_to_number_t(output_acc[pos_x]);
#endif
    }
  }
}

#undef INPUT_CHANNELS
#undef INPUT_SAMPLES
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE
#undef CONV_STRIDE
#undef ZEROPADDING_LEFT
#undef ZEROPADDING_RIGHT
#undef CONV_OUTSAMPLES
#undef ACTIVATION_RELU
/**
  ******************************************************************************
  * @file    weights/conv.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#define INPUT_CHANNELS    32
#define CONV_FILTERS      16
#define CONV_KERNEL_SIZE  3


const int16_t conv1d_19_bias[CONV_FILTERS] = {10, 8, 22, -18, -19, -9, 9, 12, -7, 2, 19, 9, 9, -6, -3, -28}
;

const int16_t conv1d_19_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{-17, -101, -11}
, {40, 18, 2}
, {-67, 79, 15}
, {-52, 45, 82}
, {-80, -57, -89}
, {-88, -59, -54}
, {-103, -89, 26}
, {96, 43, 65}
, {-23, 33, -37}
, {34, 42, -18}
, {64, 59, 35}
, {-49, 72, -40}
, {-125, 23, 54}
, {-21, 59, -18}
, {36, -32, -33}
, {63, 61, -57}
, {-54, 38, 60}
, {-92, 80, -49}
, {-87, -69, -44}
, {83, -108, -83}
, {-102, -34, -13}
, {109, -24, -3}
, {46, -34, -35}
, {99, 63, -37}
, {24, -70, 1}
, {80, 42, 61}
, {-13, 39, -76}
, {-11, -72, -83}
, {91, 20, 16}
, {41, 2, -95}
, {55, 70, -52}
, {117, -64, 87}
}
, {{-2, 28, -66}
, {72, -12, -54}
, {48, -42, 9}
, {45, 30, -49}
, {12, 93, 27}
, {-59, -54, 82}
, {46, -54, -48}
, {-47, -23, 24}
, {78, -51, 64}
, {46, 99, -62}
, {56, 94, -85}
, {74, 77, -66}
, {12, 67, -17}
, {-88, 34, -19}
, {-51, 39, 29}
, {76, -69, -9}
, {30, -70, -44}
, {-54, -84, 80}
, {-15, 63, -10}
, {71, -73, 22}
, {-36, 40, 93}
, {0, -25, 81}
, {-130, -65, -26}
, {93, 39, 14}
, {-22, 69, -94}
, {37, 41, -42}
, {25, 77, -80}
, {97, 9, -69}
, {-45, -11, 41}
, {3, 21, -32}
, {-2, -19, 38}
, {24, -41, -71}
}
, {{-81, 10, 39}
, {-17, -38, -7}
, {57, -112, -111}
, {75, 82, 48}
, {68, -9, -23}
, {92, -90, -99}
, {32, 54, -62}
, {30, -43, 106}
, {-72, 53, -96}
, {12, 17, -25}
, {88, 36, 66}
, {2, 63, -51}
, {88, -118, 12}
, {21, 92, 57}
, {-37, -101, -32}
, {91, -63, -5}
, {93, -15, -14}
, {-48, -36, -56}
, {-27, -21, -8}
, {-66, -19, 62}
, {64, -83, -114}
, {-2, 29, 50}
, {103, -90, -66}
, {13, 18, -4}
, {100, 9, 18}
, {58, 32, -92}
, {60, -33, -68}
, {-88, 57, -104}
, {-47, -21, 39}
, {-37, -23, -82}
, {-33, -110, -73}
, {42, 33, -55}
}
, {{128, 92, 56}
, {62, 33, 61}
, {42, -55, -22}
, {13, -6, -28}
, {70, -16, 88}
, {-31, -6, 66}
, {107, 12, 101}
, {-13, -2, -109}
, {-39, -91, -51}
, {84, -14, 86}
, {80, 3, -81}
, {52, 44, 26}
, {88, 55, 105}
, {5, 45, -29}
, {104, 22, -26}
, {-38, 104, 23}
, {-59, 3, 26}
, {88, -13, 41}
, {-61, 40, -2}
, {95, -87, 3}
, {91, 72, 35}
, {90, -12, 77}
, {39, -26, -4}
, {-3, 110, -30}
, {-107, 32, -14}
, {98, -49, 9}
, {90, 24, -42}
, {-20, -82, -11}
, {-40, 93, 81}
, {-87, 74, 97}
, {22, 90, -19}
, {45, 38, 28}
}
, {{-93, 75, -19}
, {52, -13, -98}
, {74, 54, -61}
, {60, 0, -19}
, {19, 26, -92}
, {-20, -15, 84}
, {84, -7, -62}
, {94, 18, -92}
, {-102, 88, -96}
, {77, 35, -87}
, {2, -47, 34}
, {16, -26, -98}
, {94, -48, 23}
, {-93, 5, -16}
, {20, -20, 99}
, {-93, -9, 54}
, {12, -36, 4}
, {-84, -27, -75}
, {71, -24, 9}
, {-32, 50, -7}
, {-30, 25, -80}
, {-2, -84, 37}
, {81, -6, 88}
, {30, 70, 90}
, {74, 115, 12}
, {2, -70, -92}
, {-86, -5, 40}
, {16, -32, -108}
, {82, 57, -81}
, {0, 32, 56}
, {-69, 67, -104}
, {-14, -107, 41}
}
, {{84, -42, 52}
, {29, 55, -87}
, {107, 11, 37}
, {83, -8, -76}
, {30, -79, 64}
, {87, 52, -82}
, {-88, 50, -92}
, {-41, -119, -48}
, {85, 29, 87}
, {30, 59, 96}
, {-21, -8, -60}
, {-63, 32, -47}
, {-87, -34, 62}
, {27, 82, 49}
, {37, -3, 27}
, {103, -79, -24}
, {-36, 30, 29}
, {53, 90, 83}
, {28, 23, 17}
, {35, -38, 112}
, {0, -27, 90}
, {-103, 94, 10}
, {-55, 52, -101}
, {87, 64, -74}
, {-45, -73, -53}
, {-11, 103, 82}
, {-76, 36, 119}
, {39, -57, -73}
, {-29, -26, -69}
, {-43, 75, -80}
, {99, 101, 101}
, {8, 29, -57}
}
, {{23, 61, 30}
, {-63, 42, 69}
, {-49, 48, -6}
, {-57, -91, 116}
, {-7, -47, 15}
, {-54, -105, -32}
, {35, 61, -2}
, {48, 8, -57}
, {-101, -33, 37}
, {-56, -106, -13}
, {23, -11, -1}
, {90, 23, 44}
, {83, -97, 7}
, {-12, 40, -86}
, {-65, -14, 1}
, {47, -14, -63}
, {-2, -82, -73}
, {-45, 87, 47}
, {-58, -92, -35}
, {29, -71, -45}
, {-39, 29, -80}
, {55, -10, -2}
, {79, 108, -73}
, {-56, -3, -42}
, {-37, 98, 74}
, {54, -17, -16}
, {33, 106, 34}
, {-68, 17, 64}
, {46, -75, -95}
, {-79, -63, 106}
, {-17, 67, -93}
, {-53, -27, -64}
}
, {{-116, 69, -81}
, {39, 89, 71}
, {47, -63, 61}
, {-56, -8, -100}
, {68, -61, -124}
, {31, -63, -113}
, {-10, 0, -19}
, {-5, -18, -72}
, {-72, -25, 73}
, {43, -28, 46}
, {-69, 92, 45}
, {-77, 33, -105}
, {-105, -86, -105}
, {48, -39, 52}
, {-83, 48, 47}
, {-45, 15, -17}
, {-76, -66, 54}
, {76, -65, 82}
, {-102, -66, -10}
, {-103, -105, -93}
, {-33, 25, -13}
, {48, 81, 62}
, {119, 73, 35}
, {-10, -39, 90}
, {-86, -119, 49}
, {80, -30, -25}
, {-119, -94, 58}
, {71, 66, -101}
, {-87, -42, -42}
, {-119, -94, 51}
, {60, -132, 39}
, {102, -47, 1}
}
, {{-23, -19, -35}
, {14, -116, 33}
, {45, -36, -68}
, {-6, 30, -7}
, {62, 16, 44}
, {-83, -107, -20}
, {45, -66, -112}
, {-55, -64, 89}
, {-39, 72, -102}
, {-74, -36, -110}
, {26, 60, -111}
, {-14, 14, 27}
, {-108, 13, 53}
, {-84, 78, -71}
, {44, 6, -71}
, {39, -64, -87}
, {24, 44, 59}
, {-94, -91, -53}
, {11, -26, -101}
, {48, 27, 22}
, {45, 66, -36}
, {10, 83, -43}
, {-55, -34, -101}
, {-75, -16, 87}
, {78, -104, 61}
, {78, 23, -27}
, {1, -52, -64}
, {57, 40, -55}
, {-39, -113, 77}
, {15, -67, -47}
, {-96, 92, 57}
, {-44, -99, 48}
}
, {{-91, 0, 81}
, {16, -14, 23}
, {28, 70, -6}
, {18, -42, 60}
, {-98, -3, -15}
, {14, 91, -29}
, {-11, -28, 25}
, {40, 69, -60}
, {27, -14, 57}
, {-78, -83, 39}
, {-86, -95, -65}
, {82, -39, -49}
, {-56, 32, -20}
, {-27, -2, -41}
, {-58, -49, -89}
, {71, 82, 63}
, {-84, 80, 54}
, {-61, -9, 52}
, {-66, 5, 108}
, {76, -85, -35}
, {65, 4, -52}
, {-87, 23, -24}
, {-66, 97, -40}
, {66, -39, -38}
, {-24, 55, -29}
, {12, -4, -73}
, {-38, 0, -22}
, {60, -83, 87}
, {-52, 70, -55}
, {-78, 7, -73}
, {-101, -80, -98}
, {-42, -26, -44}
}
, {{-57, -16, 51}
, {34, 20, -12}
, {-43, -33, -37}
, {93, -96, 69}
, {20, 6, 102}
, {89, 38, -72}
, {-36, 37, -65}
, {17, 44, -15}
, {63, 103, -46}
, {-9, 85, -53}
, {-12, -109, 63}
, {89, -25, 11}
, {-32, 64, 95}
, {79, -8, 23}
, {-54, -5, 67}
, {37, -22, 78}
, {66, 61, 15}
, {36, 55, 108}
, {-50, 22, 35}
, {-53, 15, -98}
, {-79, 38, 114}
, {88, 87, -55}
, {-51, 121, -70}
, {14, -25, -53}
, {100, -19, 41}
, {-111, 5, -33}
, {-29, -84, 56}
, {-36, 67, -33}
, {-66, -23, 90}
, {107, -4, 106}
, {-58, -77, 82}
, {-101, -96, -68}
}
, {{83, 20, 63}
, {84, 29, -15}
, {-75, 6, 87}
, {-73, -14, 114}
, {-35, 77, 17}
, {-50, 56, 54}
, {-68, -37, 115}
, {1, 7, 121}
, {-23, 83, 87}
, {-68, 26, 17}
, {7, 27, -61}
, {77, 44, 24}
, {122, 96, 95}
, {-44, -38, 57}
, {36, 65, 87}
, {102, 87, -44}
, {-99, 110, -71}
, {116, 55, 110}
, {-42, 15, -8}
, {93, -54, -59}
, {16, 63, 4}
, {-14, -96, 29}
, {-43, 103, -57}
, {22, -20, -87}
, {-2, 109, 63}
, {-15, -72, 20}
, {130, 49, 16}
, {-82, 19, 90}
, {27, -82, -2}
, {-57, 70, 109}
, {92, 28, -6}
, {-99, 81, -112}
}
, {{118, 23, 74}
, {16, 107, -45}
, {-70, 104, -12}
, {106, -54, 2}
, {-90, -55, -8}
, {20, 49, 9}
, {98, -5, 108}
, {-69, -72, -50}
, {-48, 95, 72}
, {-41, -39, 35}
, {52, -57, -36}
, {-8, 81, 91}
, {-59, 33, 11}
, {22, -43, 85}
, {16, 15, 116}
, {-79, 65, 97}
, {-25, -45, 7}
, {70, 71, 72}
, {110, 90, -25}
, {100, 105, 77}
, {84, 29, 80}
, {70, -36, -93}
, {-109, 74, -30}
, {88, 24, -7}
, {-27, 28, 94}
, {7, 40, -52}
, {104, -16, 19}
, {79, 42, -16}
, {85, -39, 108}
, {-71, 121, 118}
, {107, 15, 65}
, {10, 63, -26}
}
, {{-22, -53, 103}
, {63, -77, -67}
, {21, 63, 97}
, {-24, 73, 104}
, {1, -21, -70}
, {70, 47, -83}
, {7, 93, 57}
, {43, -93, 16}
, {-20, 6, -61}
, {96, -36, 112}
, {0, -28, -74}
, {-34, 101, 65}
, {13, -56, 63}
, {-59, -91, -74}
, {-1, 40, -19}
, {-64, 98, 79}
, {65, -13, 32}
, {-5, -1, -39}
, {-33, -19, 103}
, {44, -68, 19}
, {-19, 50, -29}
, {13, 57, -24}
, {57, -4, -27}
, {129, 119, 27}
, {-108, -92, -3}
, {67, 141, 132}
, {-15, -39, 28}
, {86, -10, 88}
, {80, -90, -74}
, {-61, -86, -46}
, {-23, 119, 119}
, {-9, -2, 113}
}
, {{68, 62, -47}
, {38, 4, 10}
, {68, -69, -30}
, {-6, 42, -23}
, {-52, -41, -48}
, {16, 28, 8}
, {-62, 51, 74}
, {74, -103, 50}
, {15, -84, -73}
, {-69, -94, 65}
, {-74, 73, -102}
, {34, 17, 83}
, {-70, -96, 25}
, {44, -99, -35}
, {70, 52, 15}
, {-23, -62, 73}
, {78, -32, -31}
, {49, -19, 72}
, {-28, 17, 79}
, {29, -17, 90}
, {45, -96, -107}
, {101, -92, 27}
, {10, 41, -70}
, {59, -65, 39}
, {0, -73, 64}
, {-115, 61, 32}
, {-48, -53, -27}
, {59, 53, -86}
, {-97, -24, 45}
, {-82, -19, 1}
, {-108, -93, -81}
, {31, 29, -86}
}
, {{49, 85, -96}
, {-20, 57, 54}
, {-58, -84, 8}
, {-98, 50, 62}
, {-77, 20, -101}
, {-105, -22, 71}
, {-64, 44, -94}
, {-25, -10, -9}
, {89, -13, 22}
, {39, -63, -32}
, {-32, 50, 93}
, {-40, -90, 55}
, {3, 17, 38}
, {20, -103, 48}
, {28, -65, 78}
, {84, -29, 78}
, {-31, -105, -31}
, {-56, -117, -77}
, {25, 45, -23}
, {-81, 82, -18}
, {4, 78, -52}
, {11, 70, 17}
, {48, 45, -59}
, {-56, -37, 112}
, {-17, -105, -55}
, {109, 82, -13}
, {91, -77, 59}
, {18, -73, 83}
, {65, -5, 51}
, {71, 61, 30}
, {-69, 43, -70}
, {-13, 63, 4}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE
/**
  ******************************************************************************
  * @file    maxpool.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_CHANNELS  16
#define INPUT_SAMPLES   504
#define POOL_SIZE       4
#define POOL_STRIDE     3
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_26_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_26(
  const number_t input[INPUT_CHANNELS][INPUT_SAMPLES], 	    // IN
  number_t output[INPUT_CHANNELS][POOL_LENGTH]) {	// OUT

  unsigned short pos_x, k; 	// loop indexes for output volume
  unsigned int x;
  number_t max, tmp; 

  for (k = 0; k < INPUT_CHANNELS; k++) 
    for (pos_x = 0; pos_x < POOL_LENGTH; pos_x++) {
#ifdef ACTIVATION_LINEAR
      max = input[k][pos_x*POOL_STRIDE];
      x = 1;
#elif defined(ACTIVATION_RELU)
      max = 0;
      x = 0;
#endif
      for (; x < POOL_SIZE; x++) {
        tmp = input[k][(pos_x*POOL_STRIDE)+x]; 
        if (max < tmp)
          max = tmp;
      }
      output[k][pos_x] = max; 
    }
}

#undef INPUT_CHANNELS  
#undef INPUT_SAMPLES
#undef POOL_SIZE
#undef POOL_STRIDE
#undef POOL_PAD
#undef POOL_LENGTH
#undef ACTIVATION_LINEAR
/**
  ******************************************************************************
  * @file    conv.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_CHANNELS      16
#define INPUT_SAMPLES       167
#define CONV_FILTERS        8
#define CONV_KERNEL_SIZE    7
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_20_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_20(
  const number_t input[INPUT_CHANNELS][INPUT_SAMPLES],               // IN
  const number_t kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE], // IN

  const number_t bias[CONV_FILTERS],						                // IN

  number_t output[CONV_FILTERS][CONV_OUTSAMPLES]) {               // OUT

  unsigned short pos_x, z, k; 	// loop indexes for output volume
  unsigned short x;
  short input_x;
  long_number_t	kernel_mac;
  static long_number_t	output_acc[CONV_OUTSAMPLES];
  long_number_t tmp;

  for (k = 0; k < CONV_FILTERS; k++) { 
    for (pos_x = 0; pos_x < CONV_OUTSAMPLES; pos_x++) { 
      output_acc[pos_x] = 0;
	    for (z = 0; z < INPUT_CHANNELS; z++) {

        kernel_mac = 0; 
        for (x = 0; x < CONV_KERNEL_SIZE; x++) {
          input_x = pos_x * CONV_STRIDE - ZEROPADDING_LEFT + x;
          if (input_x < 0 || input_x >= INPUT_SAMPLES) // ZeroPadding1D
            tmp = 0;
          else
            tmp = input[z][input_x] * kernel[k][z][x]; 
          kernel_mac = kernel_mac + tmp; 
        }

	      output_acc[pos_x] = output_acc[pos_x] + kernel_mac; 
      }
      output_acc[pos_x] = scale_number_t(output_acc[pos_x]);

      output_acc[pos_x] = output_acc[pos_x] + bias[k]; 

    }

    for (pos_x = 0; pos_x < CONV_OUTSAMPLES; pos_x++) {
#ifdef ACTIVATION_LINEAR
      output[k][pos_x] = clamp_to_number_t(output_acc[pos_x]);
#elif defined(ACTIVATION_RELU)
      // Activation function: ReLU
      if (output_acc[pos_x] < 0)
        output[k][pos_x] = 0;
      else
        output[k][pos_x] = clamp_to_number_t(output_acc[pos_x]);
#endif
    }
  }
}

#undef INPUT_CHANNELS
#undef INPUT_SAMPLES
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE
#undef CONV_STRIDE
#undef ZEROPADDING_LEFT
#undef ZEROPADDING_RIGHT
#undef CONV_OUTSAMPLES
#undef ACTIVATION_RELU
/**
  ******************************************************************************
  * @file    weights/conv.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#define INPUT_CHANNELS    16
#define CONV_FILTERS      8
#define CONV_KERNEL_SIZE  7


const int16_t conv1d_20_bias[CONV_FILTERS] = {-6, -8, -11, 19, 28, -4, 34, 16}
;

const int16_t conv1d_20_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{-93, -93, -34, -32, -72, -25, -2}
, {1, 59, -75, -63, -1, -95, 28}
, {42, -19, 10, -91, -39, 32, 1}
, {-48, -26, 8, 75, -90, 39, -26}
, {-24, 45, 72, 33, 87, -75, -96}
, {-103, -86, 10, -44, -30, 68, 63}
, {83, 11, -48, -35, -98, 50, 71}
, {0, -84, 26, -92, 86, 8, -42}
, {34, -27, 90, -68, 32, 21, 43}
, {-88, 56, 2, -38, 37, 24, -46}
, {37, 9, -79, 19, -17, 81, 13}
, {-86, 69, 37, 6, -30, -57, -83}
, {88, 23, 73, 7, -48, -72, -28}
, {16, -18, -29, 27, 65, -46, -37}
, {-5, 65, 53, 85, -23, -86, 23}
, {40, -53, 3, -46, -56, -38, 60}
}
, {{-37, 58, 42, -2, -105, -38, -23}
, {-62, 48, -74, -11, 55, 13, 53}
, {18, -57, 44, -37, -60, 6, 28}
, {7, -95, -23, -63, 56, -21, -103}
, {-74, 9, -46, -39, 39, 37, 56}
, {-58, -39, -1, 65, 84, -1, -102}
, {49, 41, -19, -53, -94, 23, -89}
, {-2, 3, -4, -71, 91, 28, 64}
, {19, 48, -21, -46, 2, -89, -49}
, {-50, -77, 14, 55, 17, -62, -54}
, {-36, -51, 44, -27, 0, 78, -78}
, {89, -27, -18, 6, -44, -18, -68}
, {-67, 30, -36, -70, -20, 1, 26}
, {19, -6, -31, 43, 24, 41, 20}
, {47, 49, -22, -33, 19, 22, 59}
, {80, -75, 8, -61, -21, -81, 15}
}
, {{35, -62, 2, -18, 11, 12, -2}
, {-25, -80, -6, -79, -54, 8, 15}
, {-3, 14, 47, -69, 77, -42, 78}
, {40, -38, 28, -36, 56, -67, -89}
, {59, 21, 11, -8, -79, -23, 61}
, {53, -95, 42, 12, 2, 84, 70}
, {-32, 87, -25, 89, -34, -3, 42}
, {42, -54, -5, 68, -75, 9, -78}
, {38, 45, 0, -35, -58, 40, 42}
, {113, 61, 112, 115, -75, -62, 59}
, {-92, -8, 19, 0, 2, 18, -72}
, {97, 32, -65, 81, 109, -43, -13}
, {41, 48, -31, 47, 36, 74, 79}
, {-38, -22, 39, -44, -81, 33, -36}
, {-49, -5, -7, 62, -49, 23, 82}
, {-17, 76, 19, -25, -38, -37, -100}
}
, {{-21, 25, -47, -53, 81, 58, 108}
, {72, 29, -83, 67, -59, 33, -55}
, {-73, 51, 25, -30, 79, 14, 3}
, {-14, 19, -23, -78, -83, -16, 68}
, {1, 31, 36, 90, -75, 28, -90}
, {52, 49, 24, -113, -58, -69, -101}
, {61, -32, 17, -70, -54, -68, -59}
, {100, 17, -14, 23, 98, 3, -20}
, {-64, 9, 72, -97, 14, 29, 38}
, {1, -36, 1, 74, 15, 38, -5}
, {22, 7, -83, -75, 95, -21, 84}
, {-133, -99, 19, -30, -106, 74, -113}
, {69, 16, -85, 4, -100, -88, -47}
, {27, 78, 4, 66, 87, -43, 3}
, {51, -94, -76, 68, 93, 94, -44}
, {102, 86, -81, -16, -10, 27, -71}
}
, {{-6, 15, 101, -41, 69, 80, 46}
, {28, -62, 48, 70, -30, -31, 15}
, {-25, 46, -43, 42, 85, 13, 90}
, {1, -37, 21, -20, -47, -40, 21}
, {-5, -48, 93, -2, -56, -27, 74}
, {-72, -2, -99, -13, -70, 14, -89}
, {-22, -34, -42, 77, 118, -26, 19}
, {-73, 67, 59, -13, -34, -8, 77}
, {-8, -82, 24, -64, 4, -20, 42}
, {29, -58, -39, 36, 25, -44, -21}
, {-3, -63, 43, -74, 35, 51, -17}
, {-54, 13, -62, 52, -14, 32, 47}
, {65, 14, 79, 38, -67, 79, 31}
, {26, -69, -87, 29, -64, -60, 80}
, {-50, -74, -45, -44, -61, 28, -8}
, {5, -53, -48, 23, -87, 68, 26}
}
, {{72, 50, 69, -57, -45, 38, -84}
, {47, 58, 30, -13, -46, 16, 33}
, {-8, 47, -47, -75, -68, 16, 7}
, {-52, 70, -17, 59, 58, 16, -23}
, {-59, -56, 30, -26, -22, 46, 50}
, {76, -44, 17, 112, 38, -5, 61}
, {13, -62, -18, -85, 56, 36, 90}
, {-51, 60, -90, -81, -18, -97, -23}
, {32, 0, 20, -28, -10, 9, 47}
, {60, 28, 77, -26, 41, 61, 66}
, {5, 53, 42, -43, 40, -32, -69}
, {75, -29, 46, -18, 31, 26, -49}
, {85, 102, 51, -14, 23, 67, 50}
, {83, 98, -21, -28, 74, 58, 81}
, {-79, -81, -52, -49, 43, -97, 4}
, {-33, 69, 27, -37, -35, 6, -8}
}
, {{-50, -23, -96, -52, 65, -87, -9}
, {94, -29, -64, 37, 79, -35, -46}
, {79, 96, -34, -92, -38, 9, 87}
, {-13, -49, -75, 83, -85, 0, -86}
, {94, -5, -27, 83, 28, 53, -73}
, {37, 46, -33, -40, 20, 51, 68}
, {17, -37, 22, 85, -78, 71, 78}
, {-40, -11, 13, 46, -43, -23, 29}
, {-25, -90, -87, 47, -16, -37, 61}
, {-32, 32, 8, 15, 70, 7, -93}
, {41, 95, 73, -22, 77, 91, -51}
, {-10, 37, -3, 51, -30, 72, 102}
, {-34, 60, -70, -11, -15, 75, -38}
, {-106, -29, -92, -45, -19, 70, 24}
, {14, 79, -91, 67, -39, 3, -55}
, {-101, -39, -39, 65, -107, 21, -31}
}
, {{-93, -85, -41, -21, -90, -61, 0}
, {-34, -35, 99, -56, 7, 75, -47}
, {-6, -5, -68, -109, -5, -110, 12}
, {43, -12, 85, -21, -54, 3, 41}
, {-31, 58, 38, 69, -1, 119, -30}
, {-55, -22, -39, 89, -54, 103, 3}
, {49, -83, -51, -82, 6, -81, 27}
, {-147, -54, -108, -143, -20, -115, -61}
, {7, 18, 7, 62, 31, -45, 3}
, {35, 2, -62, -86, 29, -77, 96}
, {27, -87, -45, 88, 111, 63, 28}
, {10, 102, 70, 122, 132, 134, 88}
, {46, -45, 88, 110, -32, -26, 73}
, {103, 54, -49, -63, -67, -64, 73}
, {34, -66, -12, -40, -55, -57, -21}
, {-58, -72, 73, 60, -105, -14, 68}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE
/**
  ******************************************************************************
  * @file    maxpool.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_CHANNELS  8
#define INPUT_SAMPLES   161
#define POOL_SIZE       4
#define POOL_STRIDE     3
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_27_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_27(
  const number_t input[INPUT_CHANNELS][INPUT_SAMPLES], 	    // IN
  number_t output[INPUT_CHANNELS][POOL_LENGTH]) {	// OUT

  unsigned short pos_x, k; 	// loop indexes for output volume
  unsigned int x;
  number_t max, tmp; 

  for (k = 0; k < INPUT_CHANNELS; k++) 
    for (pos_x = 0; pos_x < POOL_LENGTH; pos_x++) {
#ifdef ACTIVATION_LINEAR
      max = input[k][pos_x*POOL_STRIDE];
      x = 1;
#elif defined(ACTIVATION_RELU)
      max = 0;
      x = 0;
#endif
      for (; x < POOL_SIZE; x++) {
        tmp = input[k][(pos_x*POOL_STRIDE)+x]; 
        if (max < tmp)
          max = tmp;
      }
      output[k][pos_x] = max; 
    }
}

#undef INPUT_CHANNELS  
#undef INPUT_SAMPLES
#undef POOL_SIZE
#undef POOL_STRIDE
#undef POOL_PAD
#undef POOL_LENGTH
#undef ACTIVATION_LINEAR
/**
  ******************************************************************************
  * @file    averagepool.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_CHANNELS  8
#define INPUT_SAMPLES   53
#define POOL_SIZE       2
#define POOL_STRIDE     2
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t average_pooling1d_6_output_type[INPUT_CHANNELS][POOL_LENGTH];

void average_pooling1d_6(
  const number_t input[INPUT_CHANNELS][INPUT_SAMPLES], 	    // IN
  number_t output[INPUT_CHANNELS][POOL_LENGTH]) {	// OUT

  unsigned short pos_x, k; 	// loop indexes for output volume
  unsigned short x;
  long_number_t avg, tmp; 

  for (k = 0; k < INPUT_CHANNELS; k++) 
    for (pos_x = 0; pos_x < POOL_LENGTH; pos_x++) {
      tmp = 0;
      for (x = 0; x < POOL_SIZE; x++) {
        tmp += input[k][(pos_x*POOL_STRIDE)+x];
      }
#ifdef ACTIVATION_RELU
      if (tmp < 0) {
        tmp = 0;
      }
#endif
      avg = tmp / POOL_SIZE;
      output[k][pos_x] = clamp_to_number_t(avg);
    }
}

#undef INPUT_CHANNELS  
#undef INPUT_SAMPLES
#undef POOL_SIZE
#undef POOL_STRIDE
#undef POOL_PAD
#undef POOL_LENGTH
#undef ACTIVATION_LINEAR
/**
  ******************************************************************************
  * @file    flatten.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_DIM [26][8]
#define OUTPUT_DIM 208

//typedef number_t *flatten_6_output_type;
typedef number_t flatten_6_output_type[OUTPUT_DIM];

#define flatten_6 //noop (IN, OUT)  OUT = (number_t*)IN

#undef INPUT_DIM
#undef OUTPUT_DIM

/**
  ******************************************************************************
  * @file    fc.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_SAMPLES 208
#define FC_UNITS 3
#define ACTIVATION_LINEAR

typedef number_t dense_6_output_type[FC_UNITS];

static inline void dense_6(
  const number_t input[INPUT_SAMPLES], 			      // IN
	const number_t kernel[FC_UNITS][INPUT_SAMPLES],  // IN

	const number_t bias[FC_UNITS],			              // IN

	number_t output[FC_UNITS]) {			                // OUT

  unsigned short k, z; 
  long_number_t output_acc; 

  for (k = 0; k < FC_UNITS; k++) { 
    output_acc = 0; 
    for (z = 0; z < INPUT_SAMPLES; z++) 
      output_acc = output_acc + ( kernel[k][z] * input[z] ); 

    output_acc = scale_number_t(output_acc);

    output_acc = output_acc + bias[k]; 


    // Activation function
#ifdef ACTIVATION_LINEAR
    // Linear (MEANS NONE)
    output[k] = clamp_to_number_t(output_acc);
#elif defined(ACTIVATION_RELU)
    // ReLU
    if (output_acc < 0)
      output[k] = 0;
    else
      output[k] = clamp_to_number_t(output_acc);
#endif
  }
}

#undef INPUT_SAMPLES
#undef FC_UNITS
#undef ACTIVATION_LINEAR
/**
  ******************************************************************************
  * @file    weights/fc.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#define INPUT_SAMPLES 208
#define FC_UNITS 3


const int16_t dense_6_bias[FC_UNITS] = {-13, -19, 37}
;

const int16_t dense_6_kernel[FC_UNITS][INPUT_SAMPLES] = {{-10, -70, -80, 16, -53, -63, 7, 66, 12, -51, 74, -52, -52, 37, 0, -6, -68, 71, 28, -1, -57, -63, -17, -33, -72, 49, -62, 56, 16, -64, -4, -69, -57, 84, 19, -72, 8, -73, -48, -31, 35, -23, 68, -54, 48, -12, -19, -40, 43, -85, 36, -11, 31, -20, 21, -14, -53, -20, -56, 85, 71, -15, -59, -31, 0, -30, 59, -4, 15, -39, 79, 57, 11, 1, 41, -31, -20, -68, 105, 44, 17, 68, 84, 86, 25, 21, 40, 38, -14, 84, 97, 78, -17, 102, -17, -3, 38, 75, 77, -44, 95, -30, 112, -4, -4, 33, 21, -57, 16, 12, 103, -34, 52, -31, -31, -24, -24, -64, -18, 27, 56, 47, -84, 54, -70, -44, 26, 65, 83, 78, -38, -76, -96, -73, -72, -117, -10, 56, -46, -52, -20, 10, 6, -55, -10, -16, -64, -60, -110, -60, -74, -25, -19, -105, -110, 27, 15, 94, 13, 20, 50, 112, 96, 52, 20, 26, 77, -41, -41, -38, 84, -10, -40, 113, 101, 90, 4, 60, 85, 84, 24, -7, 148, 82, 84, 43, 107, 22, -17, 46, 121, 122, 12, -24, -7, 123, 74, 74, -9, -29, 78, 74, 105, 38, 75, -28, 71, 96}
, {-36, 21, -45, -21, -27, 83, 43, -59, -62, -53, 71, 72, 38, -55, -45, 84, -9, -82, 39, -77, 41, 86, -79, 30, -86, -20, 70, -22, -90, 51, 79, 26, 71, 53, 46, 26, 0, 28, 74, -11, 28, -27, 11, -40, 85, 0, 4, 31, -82, -14, -66, -20, 7, 60, 43, 21, -84, -19, -16, -60, -75, 15, -76, -39, -76, -24, -78, -88, -30, 24, -77, 22, 83, -48, 56, 78, 10, -22, -86, 34, 7, -72, -82, -12, -6, -3, -68, 29, -39, -76, -17, 24, -18, -86, 26, -24, 42, -93, 43, -3, -90, 2, -11, 50, -50, -25, 1, -27, 41, -66, -79, 28, -33, -43, -70, -101, -25, -15, 2, -101, -87, -116, -49, -59, -108, -112, -13, -60, 21, -91, 94, 30, -11, 75, 53, 32, -5, -54, 80, -52, 40, 92, 45, 63, 3, -8, 60, 15, -18, 7, -5, 80, 96, 1, 108, -12, 4, 20, 32, -51, -137, -80, -22, -27, -73, -20, -79, -30, -47, -44, -21, -35, -69, -70, -33, -60, -49, -111, -7, -13, -62, -109, 84, -35, 29, 61, -71, -14, -77, 63, 14, -46, -33, 48, -26, -37, -30, -65, 59, -54, -24, -5, 64, 5, -59, 17, 34, 126}
, {53, -40, -22, -34, 17, -33, 56, -80, 33, 1, 60, 60, -21, -5, 61, 62, 34, 26, 6, -36, -7, -58, -85, -85, -71, 4, -1, -85, 85, -77, -82, 39, -2, 76, -33, 2, -41, -72, -19, -70, 36, -53, 1, 0, 62, 15, 45, -11, -47, 64, 22, 7, -79, -71, 32, -10, -72, -8, -60, 12, 46, 49, -19, 72, 72, 70, -69, -84, -63, -16, -78, -19, -22, -47, 12, -30, -58, 11, -41, 0, -4, -88, 58, -23, 8, -98, 28, -42, 39, -21, -13, -69, 4, 17, -22, -27, -15, 31, 18, 33, -91, -43, 6, 20, 112, 16, 81, 37, -50, 53, -21, 36, 42, 38, -61, -29, -52, 79, -17, -3, 62, -41, -48, -57, -9, 56, -10, -5, 85, 27, -73, -5, 3, 94, 34, -32, -57, -35, -22, 83, 60, 24, -52, -17, 73, -40, -16, 50, 51, -36, -49, 61, 3, -3, 29, 59, -19, 27, 16, 50, 37, -59, -10, 67, 10, 7, 90, 64, 63, 95, 92, -56, 49, 108, 77, -27, -85, 35, -39, 63, -48, -2, -84, -102, -55, -83, -120, -27, 5, -13, -13, -93, -89, -79, -47, -49, -100, -61, -84, -126, -72, -105, -45, -20, -141, -101, -14, -116}
}
;

#undef INPUT_SAMPLES
#undef FC_UNITS
/**
  ******************************************************************************
  * @file    model.hh
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    08 july 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef __MODEL_H__
#define __MODEL_H__

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define MODEL_OUTPUT_SAMPLES 3
#define MODEL_INPUT_SAMPLES 16000 // node 0 is InputLayer so use its output shape as input shape of the model
#define MODEL_INPUT_CHANNELS 1

void cnn(
  const number_t input[MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES],
  //dense_6_output_type dense_6_output);
  number_t output[MODEL_OUTPUT_SAMPLES]);

#endif//__MODEL_H__
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
