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
#define POOL_STRIDE     3
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_8_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_8(
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
#define INPUT_SAMPLES       5331
#define CONV_FILTERS        32
#define CONV_KERNEL_SIZE    80
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_6_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_6(
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
#define INPUT_SAMPLES   5252
#define POOL_SIZE       4
#define POOL_STRIDE     3
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_9_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_9(
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
#define INPUT_SAMPLES       1750
#define CONV_FILTERS        16
#define CONV_KERNEL_SIZE    3
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_7_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_7(
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


const int16_t conv1d_7_bias[CONV_FILTERS] = {-13, 0, 10, 15, 0, 15, -9, -3, 22, -3, 3, 0, 1, 18, -6, 11}
;

const int16_t conv1d_7_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{71, 42, 51}
, {-6, 58, 59}
, {36, 68, 88}
, {72, -33, -91}
, {28, 5, 64}
, {-34, 52, -37}
, {-12, -56, 45}
, {3, -93, -103}
, {-77, 32, -57}
, {18, -9, -70}
, {-101, 33, -99}
, {7, -45, 4}
, {1, 67, -21}
, {84, 42, 109}
, {10, 103, 5}
, {6, -1, -31}
, {-38, -90, 91}
, {-41, 64, 45}
, {35, 0, -92}
, {-65, 18, -74}
, {-30, -96, 89}
, {74, -50, -54}
, {-60, -77, 12}
, {68, -37, 75}
, {42, -97, 102}
, {-2, 76, 81}
, {25, -52, 41}
, {89, 30, -72}
, {13, -20, -85}
, {11, -86, 52}
, {-11, -18, 46}
, {-87, 69, 106}
}
, {{-43, -7, 66}
, {49, -57, -110}
, {-61, -66, -35}
, {82, -75, 64}
, {-8, 94, 78}
, {-57, -83, -23}
, {-23, -12, 95}
, {-29, 27, 103}
, {-24, -26, 95}
, {-76, 40, -103}
, {-78, 22, 16}
, {41, -13, -118}
, {-21, -49, -54}
, {89, 49, -31}
, {-41, -70, -28}
, {54, -7, -6}
, {-3, 2, -38}
, {64, -55, -59}
, {-15, -98, -34}
, {-113, 38, -89}
, {2, 17, -96}
, {3, 24, 25}
, {64, -26, 89}
, {58, -68, 42}
, {43, -42, -42}
, {55, -63, -72}
, {64, -27, 15}
, {67, -44, -54}
, {19, -43, 97}
, {-70, -69, -110}
, {68, -82, -13}
, {-28, -56, 4}
}
, {{-13, -7, -81}
, {-98, 26, -34}
, {-89, 95, 59}
, {62, 8, -6}
, {-53, 93, -98}
, {26, 68, 97}
, {27, 77, 15}
, {-71, -29, -51}
, {91, 115, -93}
, {79, -33, 9}
, {-90, 37, -92}
, {91, 56, -39}
, {-6, 33, -22}
, {75, -22, -23}
, {10, 41, -12}
, {-17, 73, 66}
, {70, 86, -69}
, {62, 18, 86}
, {14, 104, -26}
, {34, 1, -81}
, {104, 63, 1}
, {49, 16, -35}
, {78, 53, -15}
, {52, 87, 81}
, {-36, -1, -28}
, {-33, -82, 100}
, {-71, -20, 29}
, {75, 21, -12}
, {-23, 62, 49}
, {101, 107, -27}
, {17, -57, -104}
, {-74, -60, -66}
}
, {{88, 51, -122}
, {83, -10, 90}
, {-14, -89, 9}
, {9, 15, 3}
, {88, -30, -63}
, {-44, -49, 29}
, {79, 0, -69}
, {29, 76, 96}
, {58, 70, 54}
, {-62, -52, 103}
, {10, 101, 88}
, {-27, 75, 102}
, {113, 9, -41}
, {89, 84, 42}
, {-56, -27, -78}
, {1, 13, 40}
, {78, -77, -87}
, {-80, -85, -44}
, {78, -6, 92}
, {-53, 102, 18}
, {25, -43, -31}
, {-34, -26, -38}
, {-107, -18, 50}
, {-39, -9, 45}
, {15, 5, 20}
, {-33, -145, -147}
, {26, -39, -64}
, {30, 61, 70}
, {5, -105, 95}
, {61, -4, -81}
, {-63, 34, -44}
, {1, -133, -108}
}
, {{-49, 106, 74}
, {61, -46, -93}
, {-76, 90, -24}
, {-7, 102, 58}
, {0, -19, 90}
, {114, 50, 0}
, {-43, -21, -32}
, {89, -95, 13}
, {38, 77, -8}
, {-27, -76, -67}
, {38, 109, -19}
, {64, -67, 54}
, {-82, 57, 120}
, {64, -5, 83}
, {22, 76, -34}
, {18, -103, -53}
, {51, -88, 71}
, {-37, 55, 95}
, {-47, 50, 16}
, {-85, 51, -43}
, {-5, -4, -74}
, {-3, -64, -85}
, {-54, 23, 80}
, {-62, 114, 80}
, {-46, -69, 53}
, {-54, 49, 38}
, {-100, -39, -34}
, {88, 55, -50}
, {68, -14, -54}
, {-71, -99, 74}
, {73, -63, 45}
, {69, 28, -88}
}
, {{-71, -57, 104}
, {89, 47, -12}
, {36, -98, 50}
, {88, -16, -84}
, {-46, 110, 61}
, {66, -24, -113}
, {-29, 6, -16}
, {-96, -125, -95}
, {-67, 99, 66}
, {-43, -9, 47}
, {-104, 44, -11}
, {94, 93, 48}
, {85, 72, 0}
, {-22, -84, -52}
, {-106, -107, -37}
, {-93, 36, 61}
, {-92, -89, 31}
, {-101, -115, -159}
, {-43, -66, 5}
, {5, -99, -21}
, {-45, 17, -22}
, {-33, -91, 115}
, {-13, 49, 52}
, {114, 111, -65}
, {-28, 9, 123}
, {-3, 7, 60}
, {135, -47, 53}
, {-21, 46, 20}
, {-77, 44, -67}
, {56, -1, -126}
, {-98, -64, -2}
, {-85, -28, -84}
}
, {{101, 26, 72}
, {-85, -45, 32}
, {-71, -38, 85}
, {3, 74, 111}
, {74, 104, 44}
, {108, -56, 79}
, {-75, -23, -121}
, {-5, -3, -68}
, {45, -61, -58}
, {91, 58, -104}
, {47, -66, -55}
, {23, 111, 20}
, {-42, 107, 63}
, {-78, 23, -106}
, {0, 49, 25}
, {-28, 15, -27}
, {-106, 14, 109}
, {112, 24, 43}
, {-72, 84, -89}
, {49, -11, 20}
, {47, -42, -71}
, {92, 75, 60}
, {50, 112, -8}
, {80, 87, 102}
, {-115, 0, -109}
, {33, -70, -25}
, {-103, -21, -87}
, {58, 102, 77}
, {-14, -92, 51}
, {-84, 34, -65}
, {99, 66, -95}
, {99, 103, -36}
}
, {{-34, 76, 38}
, {61, 63, 25}
, {58, -27, 45}
, {-6, 31, 46}
, {97, -1, -32}
, {-1, -14, -4}
, {-72, 11, 4}
, {73, -90, 63}
, {86, -56, 110}
, {0, 10, -31}
, {-18, 47, -64}
, {11, -40, 47}
, {125, -82, 55}
, {55, -29, -73}
, {49, 64, 20}
, {34, 36, -32}
, {-59, -29, -58}
, {-25, 44, 70}
, {-92, 17, 112}
, {-79, 103, 4}
, {-97, 72, 66}
, {-30, 105, -5}
, {58, 41, 71}
, {93, 65, -31}
, {-15, -33, 40}
, {-51, -87, 1}
, {-94, -97, -87}
, {89, 60, -90}
, {25, -33, 40}
, {70, 23, 68}
, {8, -43, 103}
, {-16, -6, -67}
}
, {{-84, -35, 51}
, {-73, 40, 17}
, {69, 69, 54}
, {-101, 82, 6}
, {15, -88, 76}
, {61, -70, -56}
, {-12, -39, 43}
, {-36, 53, -106}
, {-92, -63, 20}
, {104, -2, -83}
, {-13, 28, 11}
, {35, 60, -19}
, {-27, -37, -22}
, {108, 41, -30}
, {-54, 57, 28}
, {-81, -67, -91}
, {31, -93, -14}
, {-65, -39, 56}
, {37, 88, 18}
, {54, 56, 49}
, {-13, -5, -78}
, {-88, -66, 13}
, {76, -86, 11}
, {72, -50, -26}
, {-41, -22, 103}
, {-9, 69, 85}
, {57, 118, -26}
, {-32, -56, -39}
, {-6, -31, -70}
, {-74, -38, -25}
, {65, 12, 7}
, {0, -106, -41}
}
, {{-82, 103, -59}
, {57, -19, 67}
, {2, 13, 41}
, {63, 30, -83}
, {-48, 67, 33}
, {-13, -87, 62}
, {-32, -67, -80}
, {30, 62, -52}
, {110, -56, -46}
, {116, -51, -26}
, {-6, 38, 13}
, {52, 26, 15}
, {117, 52, -54}
, {-47, 35, 87}
, {2, 27, -74}
, {33, -88, 92}
, {-78, -73, 43}
, {90, 70, -10}
, {31, 37, 103}
, {79, -1, -71}
, {90, -24, 84}
, {-95, 57, -22}
, {20, -35, 116}
, {-18, 8, -45}
, {-1, -64, 24}
, {11, 34, -102}
, {-39, 8, -65}
, {41, 23, 65}
, {-16, -40, -31}
, {85, 63, -80}
, {58, -76, 74}
, {-2, -62, -30}
}
, {{40, -73, -8}
, {73, 82, 1}
, {-1, 57, 27}
, {80, -66, -18}
, {-62, -51, 25}
, {-49, -22, 95}
, {11, -6, -52}
, {-66, 73, 4}
, {50, 57, -15}
, {-63, 42, -82}
, {-68, 0, -54}
, {-2, -60, -72}
, {64, 69, 73}
, {-35, 34, -78}
, {-122, 33, -11}
, {46, 25, -93}
, {90, 44, -30}
, {-46, 9, -31}
, {-64, 104, -5}
, {104, -76, 13}
, {-74, 48, 76}
, {-2, 24, 15}
, {107, 40, 38}
, {62, 47, -9}
, {80, -61, -119}
, {-55, -38, 5}
, {-58, -68, 30}
, {-60, -50, -78}
, {-63, -93, 29}
, {6, 12, 24}
, {37, -83, 58}
, {28, 9, 57}
}
, {{37, 116, 26}
, {65, -9, 32}
, {-71, -25, -88}
, {-26, 81, -10}
, {77, -39, 71}
, {97, 104, 43}
, {-9, 80, -76}
, {80, 59, -64}
, {112, 20, 107}
, {87, 84, -52}
, {82, 30, -55}
, {14, 94, 12}
, {-26, 127, -68}
, {-57, -97, -88}
, {-48, -29, -70}
, {-59, -21, 56}
, {70, -49, 84}
, {-24, -42, -93}
, {-14, 45, -19}
, {28, -24, 18}
, {92, 13, -78}
, {-43, 68, 83}
, {72, -83, -31}
, {99, 37, 69}
, {95, -62, -101}
, {-72, -97, 84}
, {84, 0, -78}
, {120, -5, -81}
, {46, 100, -29}
, {-37, -4, 89}
, {36, 10, 81}
, {-114, -85, 57}
}
, {{8, 99, 57}
, {-67, -79, -21}
, {3, 35, -33}
, {82, -45, 111}
, {7, 0, 100}
, {108, 79, -24}
, {-141, -104, 10}
, {-44, -14, 97}
, {-51, -7, -41}
, {82, 45, 42}
, {-14, 95, -55}
, {-37, 10, -35}
, {-49, 88, 3}
, {-50, 50, 97}
, {-88, -57, -121}
, {26, -26, -18}
, {-71, 72, 42}
, {55, 110, -52}
, {-49, -27, 104}
, {-40, 10, -38}
, {-24, -32, -109}
, {1, 45, -74}
, {-66, 42, -69}
, {18, 7, 4}
, {111, -53, 21}
, {7, -75, -83}
, {50, 19, -87}
, {77, -21, 0}
, {-77, 28, 92}
, {-37, 50, -41}
, {-4, -83, -37}
, {-120, 0, -34}
}
, {{-19, -21, 53}
, {15, -60, 94}
, {89, 78, 48}
, {-80, -49, 64}
, {3, -109, 68}
, {-20, -51, 59}
, {65, -67, 6}
, {-16, -18, 2}
, {-122, -5, -98}
, {-61, -99, 48}
, {5, -11, 0}
, {81, -58, -37}
, {-51, 59, -77}
, {-51, 10, -83}
, {-67, -25, 100}
, {40, -20, 6}
, {56, -7, -19}
, {56, 53, 65}
, {0, -111, -112}
, {-14, 18, -65}
, {73, -76, -32}
, {-70, 64, 4}
, {72, 8, -81}
, {15, 49, -101}
, {4, -64, -69}
, {33, -26, 56}
, {-22, 36, 12}
, {-22, -77, -37}
, {-97, -77, 50}
, {-95, -22, 38}
, {94, 78, 77}
, {39, -74, 76}
}
, {{9, 60, -51}
, {-73, 104, -48}
, {12, -53, 15}
, {69, 1, 57}
, {55, -88, -4}
, {54, 34, -50}
, {-12, -60, -83}
, {-66, 44, -73}
, {32, -74, 128}
, {103, 46, 1}
, {54, -28, 21}
, {0, -21, 93}
, {4, -86, 61}
, {-72, -110, -45}
, {-21, -6, 101}
, {73, -63, -65}
, {25, 131, -60}
, {17, -14, 95}
, {109, 95, -9}
, {46, 78, 91}
, {-44, -77, 10}
, {36, 89, -97}
, {53, -3, -26}
, {38, 99, 79}
, {28, -96, -117}
, {-14, -97, 99}
, {-87, -11, -35}
, {102, 43, 85}
, {-14, -33, -87}
, {102, 84, 3}
, {-99, 16, 41}
, {46, -85, 65}
}
, {{-38, 85, 90}
, {91, 65, 11}
, {-84, -103, 21}
, {16, -73, 72}
, {19, -36, 40}
, {-9, -17, -85}
, {115, 96, 96}
, {-58, -102, -48}
, {-44, 1, 37}
, {62, 48, 10}
, {5, -70, 97}
, {49, 44, 85}
, {-55, -48, -52}
, {-81, -23, -66}
, {24, 65, 11}
, {-41, -42, -36}
, {-28, -21, 36}
, {7, 64, -18}
, {86, -78, 70}
, {-66, 83, -7}
, {60, -35, -59}
, {28, 20, 67}
, {-67, 39, 41}
, {51, 74, 72}
, {67, 54, 87}
, {15, 0, 24}
, {-37, -67, 39}
, {-101, 11, 16}
, {-11, -40, -35}
, {66, 17, 2}
, {66, -91, 27}
, {39, 85, -10}
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
#define INPUT_SAMPLES   1748
#define POOL_SIZE       4
#define POOL_STRIDE     3
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_10_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_10(
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
#define INPUT_SAMPLES       582
#define CONV_FILTERS        8
#define CONV_KERNEL_SIZE    7
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_8_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_8(
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


const int16_t conv1d_8_bias[CONV_FILTERS] = {-10, 40, 0, 16, 19, 3, 3, 20}
;

const int16_t conv1d_8_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{-76, -60, -61, -56, 35, -6, -58}
, {58, 87, 109, -22, -24, -72, 60}
, {-73, 67, 62, -29, -33, 1, -60}
, {-52, 102, -62, 66, 15, 77, -95}
, {-35, 61, -30, 79, 4, 40, -26}
, {101, 150, 32, -35, 36, 28, 52}
, {-32, -20, -39, -22, 113, 32, -39}
, {137, 141, -29, 82, 89, 90, -13}
, {50, -53, 52, -56, 84, 51, -40}
, {-70, 59, 50, -5, -66, 118, 115}
, {80, -53, -76, -2, 11, 44, -13}
, {10, 119, 104, 26, 107, -6, 49}
, {-17, -5, 11, 89, 2, 47, 38}
, {13, -92, -47, -90, 70, -52, -46}
, {36, 21, -24, 50, -58, 27, 58}
, {-36, -34, -88, -20, 72, 43, 43}
}
, {{-82, 30, 51, -71, -19, -87, -54}
, {-44, 72, 46, 61, -18, 29, 82}
, {-29, -84, 53, 32, 31, 80, 41}
, {36, -106, 53, -75, -54, -32, -40}
, {-82, -107, -91, -43, -14, -90, 54}
, {69, -7, 14, -81, 45, -45, 15}
, {1, -64, 66, 7, -81, -64, -79}
, {-60, -24, -40, 71, -57, -31, -3}
, {-27, 6, 95, -19, 93, -17, 17}
, {2, 13, -86, -61, -21, 40, -86}
, {33, -45, -29, 13, 24, 48, -35}
, {-2, -50, 8, -82, -69, -83, 28}
, {41, -129, 58, -76, -65, 64, 22}
, {-41, 44, 5, 71, 75, 44, 85}
, {-3, -94, -90, -37, 46, -92, 54}
, {84, -6, 77, 11, -78, 6, -83}
}
, {{19, 27, -33, -15, -65, 6, -15}
, {-70, -55, 28, 31, -86, 32, 64}
, {108, 54, 51, -3, 65, -38, -2}
, {91, 41, -4, 53, 123, 102, 13}
, {-10, 104, -8, 46, 107, 29, -61}
, {-76, -91, 43, 13, -118, -66, -75}
, {85, 83, -79, 4, 10, -81, 66}
, {29, 86, -58, -5, 74, 37, 68}
, {-64, -21, 59, -53, 22, -6, -12}
, {-50, 3, 36, 66, 58, 37, -9}
, {-83, 47, 51, 12, -29, 14, 59}
, {30, -62, -26, 85, 11, 14, 28}
, {104, -41, 86, 51, -15, -38, 101}
, {-18, -66, 35, 0, -77, 110, -67}
, {15, 50, 65, 29, -83, 75, 60}
, {-44, -31, 76, 67, -68, -24, -28}
}
, {{-25, -61, 82, -5, 53, 23, 45}
, {7, -88, 1, -64, -111, 75, 57}
, {12, -61, -23, -34, 16, -72, -54}
, {-104, -42, 55, 60, 28, -79, -49}
, {-27, -86, -86, -19, 64, 22, 50}
, {28, 54, 92, 73, 128, 98, 92}
, {-5, 0, -62, -79, 75, 84, 42}
, {36, -79, -70, -104, 24, -78, 55}
, {-34, -32, 50, 16, 40, 93, -24}
, {-55, 80, 29, 72, -13, -25, -59}
, {-88, 8, -69, -40, -45, -60, 10}
, {-101, 63, 11, -85, -78, 3, 47}
, {66, -37, -70, 39, 28, -19, 31}
, {42, 4, 41, -70, 38, -58, -53}
, {50, -77, 51, -66, -37, -89, -27}
, {69, -100, 13, -75, -63, 31, -14}
}
, {{-19, -78, 58, 66, 61, 40, 29}
, {-46, -3, -40, -1, -73, 76, 57}
, {29, 17, 8, 56, -68, -28, -48}
, {25, -62, -86, -78, 13, -116, -136}
, {-101, -23, -13, 35, 17, 8, 38}
, {5, 37, 74, 86, 150, 76, 15}
, {-113, -18, -68, -48, -76, -72, -44}
, {28, 0, 76, -77, 43, -64, -94}
, {86, 88, 26, 38, 70, -91, 4}
, {27, 61, -63, 61, 25, -41, 79}
, {-66, -85, 77, 46, -28, 82, 76}
, {80, 65, 100, 24, 8, 2, 12}
, {-65, -60, -16, -70, -77, -10, -57}
, {-51, -99, -32, -76, -54, -5, 49}
, {-91, 43, 62, -51, -71, 32, -37}
, {-57, 81, -52, -39, -22, -6, -69}
}
, {{-61, -6, 50, -66, -17, -75, 71}
, {27, -18, 0, 52, -79, -39, 61}
, {-25, 98, -74, 17, -77, -24, 24}
, {-80, 107, 18, 58, 34, -12, -13}
, {32, -41, 62, 23, 85, -56, 44}
, {13, -8, -49, 0, 118, 6, 46}
, {20, 73, 34, 41, 13, -35, 10}
, {-22, -49, 112, 36, 19, 38, -42}
, {50, -99, 46, -29, -86, -90, 43}
, {11, 91, 25, 66, 113, 16, 39}
, {-46, 80, 20, 54, 10, -80, 38}
, {74, 29, 66, 89, 71, -69, 18}
, {81, -36, 77, 101, 39, 39, -70}
, {71, -38, -86, 33, -101, 16, -48}
, {70, -56, 33, -79, -70, -2, -36}
, {-38, -24, -80, 58, 74, -68, 62}
}
, {{-55, 1, 12, -37, -39, -6, -22}
, {-29, -35, -85, 18, 29, -57, -78}
, {94, -84, -95, -92, 95, -79, 34}
, {84, -51, -28, -63, 107, 90, 0}
, {67, 2, -3, 97, 50, -38, 87}
, {-77, -92, -37, -26, -19, 74, 19}
, {-31, 6, -10, -32, 33, 43, -3}
, {-4, 50, -14, 51, 81, 70, -79}
, {-34, 60, 54, -26, 18, -22, 13}
, {-54, 87, 79, -47, 66, 89, -36}
, {-55, -68, 27, -14, 36, 79, 62}
, {-49, 76, -18, -20, 89, -39, 105}
, {29, 105, 114, -43, -13, -37, 98}
, {33, -96, -87, -68, 44, 80, 88}
, {-36, 74, 98, -41, -42, -42, -17}
, {-35, -82, -57, -8, -7, 92, -1}
}
, {{41, 44, 39, 44, 0, -4, -98}
, {-37, 27, 56, 53, 23, -72, -77}
, {-74, -80, 2, 85, -36, 2, 75}
, {-15, 31, 54, 32, 47, -62, -116}
, {-57, 14, 24, -79, 16, -63, -5}
, {33, 0, 9, 98, 125, 97, 147}
, {-98, -96, 46, -6, 15, -85, -40}
, {-99, -47, -15, 49, -102, -74, -103}
, {56, -47, 14, 75, 25, -60, 45}
, {-94, -48, -38, -23, 8, -3, 65}
, {56, 10, 25, 3, -93, -35, -85}
, {83, 2, 43, -60, 0, -6, -82}
, {9, 27, -43, -48, -27, 55, -33}
, {57, 34, -33, -73, 19, -69, -50}
, {-48, 5, 79, -37, 39, 70, -89}
, {82, 27, 64, 41, 83, -12, 48}
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
#define INPUT_SAMPLES   576
#define POOL_SIZE       4
#define POOL_STRIDE     3
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_11_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_11(
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
#define INPUT_SAMPLES   191
#define POOL_SIZE       2
#define POOL_STRIDE     2
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t average_pooling1d_2_output_type[INPUT_CHANNELS][POOL_LENGTH];

void average_pooling1d_2(
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

#define INPUT_DIM [95][8]
#define OUTPUT_DIM 760

//typedef number_t *flatten_2_output_type;
typedef number_t flatten_2_output_type[OUTPUT_DIM];

#define flatten_2 //noop (IN, OUT)  OUT = (number_t*)IN

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

#define INPUT_SAMPLES 760
#define FC_UNITS 3
#define ACTIVATION_LINEAR

typedef number_t dense_2_output_type[FC_UNITS];

static inline void dense_2(
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

#define INPUT_SAMPLES 760
#define FC_UNITS 3


const int16_t dense_2_bias[FC_UNITS] = {-14, -14, 26}
;

const int16_t dense_2_kernel[FC_UNITS][INPUT_SAMPLES] = {{-2, 11, 44, -1, 3, 31, 27, 70, 13, 51, 35, 4, 65, 18, 2, -20, 37, 10, -1, 1, 0, 55, 80, 31, -17, 73, 32, 26, 80, -23, 47, -7, -7, 42, 1, 10, 49, 52, 18, 64, 48, 73, 37, 29, 37, 14, 24, -2, 46, -9, 29, -2, -13, 4, -8, 45, 49, 38, -28, 35, 17, 14, 40, 9, 48, 9, 36, 8, -12, 36, -9, 37, -23, -7, -4, 25, 32, 17, 0, 28, -3, 0, -29, -28, 25, 28, -40, 34, 14, 24, 37, 3, -22, -1, -33, 19, 12, 23, -20, -36, 7, -3, 7, 5, -42, 18, -8, -20, 28, -31, -41, -37, 11, -13, 14, -23, -31, 61, -5, -11, 44, 8, 32, 24, 0, -47, -17, -34, -49, 6, 6, 2, -30, 7, -33, -21, -12, -1, 16, 0, -21, -33, -17, 12, -43, 14, -34, -23, 9, 15, 5, -26, -3, -23, 37, -26, -11, 13, -42, -4, 28, -31, -35, -48, 25, -33, -6, 13, -43, 24, 30, 10, -13, -24, -13, -17, -11, 21, -6, -47, -48, -37, 11, -1, -5, -7, 29, -37, -19, 43, 10, 2, -45, -35, -39, 16, -28, -22, -42, -36, -47, -24, 13, 13, -8, -53, 11, 0, -60, 0, 19, -34, -8, -45, -41, 25, 41, 6, -28, -15, -50, 24, -8, 24, -27, -20, -9, 10, 0, -6, -21, -19, 5, 9, -35, 15, 20, 37, -4, 31, 26, -5, -46, 24, -21, 21, 21, -30, -8, -22, -31, 16, -34, -57, -19, 29, 0, -23, -39, -46, -47, 17, -46, -25, -59, -3, 13, -4, -37, -49, 8, -43, -13, 1, -56, -36, -4, -1, 6, 17, -56, -24, -49, -48, 5, -10, 31, 22, -20, 4, -41, 18, 0, -45, -55, -51, -9, -47, -19, 21, -4, 19, -25, 32, 1, 22, -60, 2, -6, -30, 25, -15, 34, 6, -13, 3, 29, -12, -24, 30, -33, 3, -34, -23, -36, 36, 7, 25, -21, -7, 23, -23, -21, -62, -5, -67, -43, -27, -24, -8, -13, 22, 18, 26, 25, -10, 19, 12, 48, 69, -38, -35, 3, 21, 34, 8, 15, -15, 10, -27, -34, -44, -42, 37, -6, 12, 21, -17, 23, -20, 11, -39, -22, -25, 18, 19, -1, -8, -27, -23, -25, -15, -16, 20, -30, -11, 12, 33, -18, -27, -35, 43, -14, 54, 0, -11, 29, -22, -22, 5, 41, 0, -12, 43, 3, -16, 35, 0, 56, -5, -16, 7, 27, -9, -19, 5, -3, -16, 21, 50, -8, -7, 37, -12, 52, -10, 2, -25, 14, 36, 10, -27, -8, 6, 28, -9, 35, 29, 55, -23, 43, 43, 46, 3, 39, 24, 4, 17, -5, 9, 0, 45, 18, -28, 13, -29, 41, 36, 37, 22, 41, 1, 23, 37, 14, -28, 21, 25, -9, 14, 45, 5, 34, 15, -33, -22, 10, -39, 29, 45, -14, 7, -2, -28, 15, 22, -45, 30, 18, -27, -3, 15, -32, -8, 17, -34, -29, 10, 43, -12, -27, -15, 47, -18, 7, 1, 31, 37, 6, 38, -24, 9, 21, 29, -24, -29, 5, -10, -37, -8, 10, -36, -22, 4, -21, 21, -17, -3, 2, -39, -16, 6, -25, 8, 11, 36, 1, 41, -9, 43, 35, 0, 11, 21, -40, -1, -32, 28, 30, 11, -36, -23, -32, 39, 3, 6, -18, 17, 21, 11, 11, -44, 4, -18, 7, 2, -4, -34, 36, 26, 27, -39, -16, 16, -27, -16, 33, 18, 5, -29, -10, -47, -14, -1, -32, -38, -33, -12, -39, -42, -27, 17, -8, -11, 11, 20, -26, -9, -41, 0, -23, 14, -42, -11, -2, -14, -20, 21, 29, -14, 43, -15, 39, -46, 10, -24, 12, -19, 32, -6, -25, 1, -7, 13, 45, -35, 16, -7, -27, -28, -27, 12, -34, 36, -11, -4, 17, -4, -12, 28, -8, -17, 41, -5, 13, -26, -31, 18, 34, -23, 5, -12, 17, -41, -44, 19, -1, -31, 2, -8, -35, 32, 5, 30, 48, -13, 57, -18, 14, 50, 34, -1, 4, -38, 15, -8, 31, 15, 15, 45, -8, -4, -14, 11, -32, -34, -26, 1, 35, 65, -1, -13, -7, 62, 39, -18, 11, 11, -2, 0, -31, -24, 40, 29, -28, 51, 49, 5, 30, 25, 8, -20, 20, 12, 32, -46, -38, 34, 0, -7, -5, 29, 23, 7, 47, -10, 59, 6, -33, 21, 35, -3, 43, -17, 31, 33, 28, 10, -20, 39, -2, 0, 26, 0, -9, 13, 34, -32, 36, 18, -16, 1, -1, 15, -20, 54, 50, 2, 40, -32}
, {3, 21, 35, 58, -6, -2, -29, 7, 23, 22, 19, -16, 5, 17, 21, 23, -22, -18, 1, -4, -28, -23, -67, -14, 39, -40, -30, 2, -57, 32, 6, -25, 27, -51, -19, -11, 10, -13, 49, -21, -4, 4, -22, 4, 42, 29, -1, -13, -6, 25, -6, 13, -19, -1, -47, -6, -34, 21, -19, 25, 39, 12, 43, -11, 21, 55, 54, 34, 18, 14, -6, 31, -39, 10, 57, 20, -20, 17, 19, 30, -28, 29, 47, -38, 16, -32, -1, 11, 24, -12, 29, 67, 71, 9, 48, 3, -58, -51, 5, 0, -41, -39, -34, -25, -29, 11, -63, -67, -79, -13, -39, 15, 3, -19, -21, -62, -65, -20, -68, -52, 17, 9, 1, 31, 36, -36, -1, 0, -57, 6, -10, 5, -42, -43, 12, 25, 13, 16, -12, -34, -2, -20, -38, 2, 0, -1, -59, 1, -24, 0, -54, -65, -46, -15, -55, 9, 4, -34, 0, -37, -53, -6, -40, 32, 42, 0, -3, -38, -26, -15, -12, -39, 20, 2, 20, 3, 3, -52, -63, 3, 2, -10, -17, 16, -57, -20, -8, -13, 12, -19, 85, 27, 51, 57, 16, -3, 60, 46, 14, -22, 54, 46, -2, -22, -1, 0, 11, -7, 21, 43, 26, 6, 24, 24, 3, 7, 23, 36, 31, 1, 37, -7, 14, 19, -11, 5, 39, 8, 44, 7, 50, -14, 45, -17, -9, 54, 48, 2, 33, -1, -24, -1, 0, -42, 14, 21, -9, 36, 8, 27, -32, 9, 33, 18, -37, 46, -2, 34, 23, -24, 24, -30, 21, 7, -11, -19, -16, 30, 19, 33, 2, -3, 46, 34, 40, 3, 8, 34, 52, 61, -8, 73, -14, 51, 0, -50, -5, -13, -14, -14, 18, -30, 5, -37, 10, -42, 16, -7, -51, -19, -42, -45, 12, -52, -33, -30, -8, 3, 2, 27, 28, -1, -45, -21, 0, 17, -37, -47, -5, -15, -5, -64, 1, 13, 9, 7, 19, -58, 32, -23, -15, -2, 33, 36, -8, 45, 8, -49, 17, -19, 20, -7, 5, 2, 22, 21, 30, -28, -44, -37, -20, 2, 7, -51, -23, 10, -23, -30, -5, -15, -55, 40, 42, -5, -17, 29, 6, -28, -10, 30, -56, -41, -1, -55, -29, -39, -6, -33, -35, -39, -7, -48, -61, -69, -28, -38, -20, -42, -1, 9, -27, -65, -64, -40, -31, -20, -70, -10, -56, -10, 15, 5, 25, -10, 2, -12, -24, 12, -17, 3, -58, -1, -6, -26, -40, 14, -50, -12, -19, -54, -61, -6, -16, -14, 18, 3, 8, -10, -23, -39, -13, -13, -40, -20, 0, 10, -39, -30, -70, -7, -33, -29, -46, -42, -34, -39, 10, -9, 2, 17, -11, 5, -52, -38, -7, -5, -48, -22, 19, -30, -9, -26, -56, -36, -41, -53, -47, -26, -12, -37, -58, -20, -5, -34, -16, -1, 66, -8, -12, 45, 43, 22, 10, -9, 38, 48, 43, 0, -10, -25, 12, -5, -14, -6, 27, -51, -22, -17, -26, -2, 6, -4, -8, 8, 45, -20, -10, -27, -27, -38, 2, 9, 28, 30, 39, -24, 28, -3, -29, -3, 13, -6, -11, 28, -27, 19, 10, -9, -25, -14, -19, -37, 17, 37, -27, 21, -1, -26, -37, 14, 31, 14, -30, -2, -1, -19, -7, -37, -1, -4, 42, 20, -28, 49, -36, 42, -32, -7, 20, 46, -14, 44, 9, -35, -7, 31, -27, 40, -12, 43, -8, 44, -3, -21, 40, -7, 27, -21, 25, 52, 36, 10, -23, -18, -14, 22, 50, 41, -16, -19, 7, -46, 44, 45, 17, -9, 13, 19, -20, 42, -12, 12, 11, 39, 37, 57, 32, -8, -31, 25, 10, 43, 16, -11, 14, -2, -7, 28, -24, 45, -36, -9, 38, 32, -11, 2, 13, -7, 33, -22, -22, -27, -15, -5, 4, 60, 37, 10, 45, -8, 14, -13, 24, -21, 12, 41, 25, 58, -8, 25, 18, 33, -13, 16, 21, -8, 32, 2, 27, 22, 42, -27, -16, 2, 37, -19, 2, -41, -29, -19, -64, 2, 6, 9, -31, -43, -8, 27, -7, -33, -23, -31, -36, 14, 15, -37, -46, -21, 13, -7, -54, -41, 16, -36, -29, 25, -1, -53, -57, -45, 34, 27, -35, 23, 20, -36, -24, 32, -35, -53, -3, -10, -21, 8, 3, -19, 13, -16, -49, -30, -7, 28, -41, 2, -7, -55, -50, 0, 2, -60, -5, 15, 12, 35, -9, -42, -12, 28, 23, -5, -4, -53, -17, -5, -4, -44, -36, 0, -5, 20, 3, -9, -4, 0, -3, -21, -19, -37, -47, -45}
, {-53, -70, -35, -71, -50, -62, -90, 7, -7, -44, -40, -55, -2, 1, -72, -63, -20, -40, -6, -20, -55, -8, -20, 27, -64, -23, -14, -82, 18, -66, -2, -48, -11, 5, -48, -61, -59, 25, -26, -6, -67, -59, -28, -50, -52, -72, -37, -38, -41, -43, -47, 21, -57, 33, -14, -7, -59, -9, 8, -53, 10, -60, -86, -70, -10, -42, -77, -40, -41, -26, 19, -22, -13, 9, -54, 16, 6, 27, -63, -51, -48, -32, -58, -4, -53, -46, -58, -64, -13, -13, -40, -14, -25, -14, -66, -24, 4, -28, -32, 52, 26, 22, 55, 45, 47, 37, 61, 47, 45, 23, 36, -11, -10, 29, -6, 27, 23, 39, 13, -40, -11, -48, -13, 38, -24, 11, 35, -6, 16, 20, 19, 15, -12, 27, 22, 43, 1, 18, 14, -1, -23, 21, 3, 0, -5, 40, -9, 63, 40, -4, 30, 40, -1, 5, -4, -10, 63, 41, -29, 24, 43, 52, 20, 45, 23, 25, 57, 26, 51, -4, 50, 56, 31, 22, 35, 26, 12, -4, 15, 12, 8, 39, 28, 32, -6, 3, 44, 9, 14, 3, -45, 22, -57, 7, 24, -12, 0, -10, -34, -35, 37, 41, 29, 9, 1, 37, 18, 52, -26, 29, 35, 34, -24, -11, -13, -14, 23, 10, -28, -5, 43, 13, -4, 26, 44, 9, 39, 5, -40, 11, 26, 27, 36, 20, -17, 28, 42, -33, 12, 38, -10, -26, -2, 2, 25, -11, -3, 21, 47, -9, 19, 3, 43, -30, -2, -12, -6, -33, -26, -24, 15, -9, 44, 6, 7, -27, 17, 48, 26, 35, 24, 37, 14, 35, 13, 16, 41, 27, -32, -31, 28, 0, -5, 1, -8, 19, 69, 44, -23, 23, 39, -13, 1, 44, 29, 12, 60, -20, 47, 51, 4, -4, -28, 25, -13, 58, 12, 57, -5, -18, -5, 28, 0, 12, -2, -35, 27, 54, -24, 72, 65, -10, 66, 50, -26, 41, -20, 40, -9, 0, 59, 54, 67, 46, 66, 0, -14, 30, -24, -22, 22, -18, -26, 9, -10, 12, 30, 25, 12, 42, -16, 3, 19, -19, 36, 12, 33, 26, 13, -26, 9, 41, -9, 8, -3, 14, -1, 15, 37, -33, -18, -7, -13, -4, -11, 30, 35, -30, 17, 11, 34, 44, -32, 21, -1, -4, -10, 8, -13, -29, -31, 29, 26, 37, 4, 5, -42, 20, 35, -27, 17, 22, 34, 25, -45, -24, 0, 24, 11, -45, 18, 30, 31, 26, 9, -10, -9, 5, 0, -38, -35, -32, 29, 37, 39, 30, 35, -18, 43, 26, -36, 43, -11, 34, -13, 25, -14, 18, 44, -40, -8, 34, -14, 3, 23, 0, 12, -2, -14, -29, -11, 24, 32, -37, -35, 23, 0, -18, 4, -27, -17, 9, -8, 16, 31, 44, -36, -20, 25, 46, -21, 35, 17, 22, -23, -42, -18, -55, -17, -61, -23, 21, -14, 20, -27, 8, 28, 27, -9, -1, 25, -1, -6, 30, 43, -17, -14, 9, 23, 28, -18, 38, -41, 35, 36, -43, 32, -36, -6, 6, -47, -20, -16, -37, 25, -30, 23, -21, -18, -43, 9, 12, 11, 10, -8, -45, -45, -19, 33, 9, -17, 4, -1, -37, -2, -24, -35, 5, 34, -39, 0, -44, 10, -16, -30, -41, 20, -2, -37, -38, -16, -17, 25, 0, 26, -38, 18, 21, -3, 3, 15, -1, -22, 34, -18, -8, -26, 7, -51, -60, -55, -5, 32, -20, 15, 11, 7, -6, 35, -35, -40, -27, -15, -32, 10, -33, -48, -29, 20, 23, -49, 38, 25, -11, -26, -46, -3, -36, 13, -12, 0, 24, 18, -25, -34, 18, 36, 10, -21, 10, -35, 27, 26, -35, -24, -23, -34, -9, 25, -37, 37, -1, -27, 30, 21, 19, 13, 0, 38, 22, 13, -35, -33, 35, -47, -52, -52, 33, -40, -11, 35, -8, 1, 2, 19, 28, 11, 16, -30, -32, 27, -14, -6, 25, 31, 26, -43, -32, -3, -44, 24, 9, 17, 35, -15, 15, 24, 10, -22, 54, -20, -5, -10, -11, -23, 35, 55, -11, 30, 34, 14, -37, -11, -18, 5, 39, -29, 3, -37, -9, -33, -16, -4, -18, -26, 11, 13, 39, -39, -23, 17, -19, 34, -23, 21, 35, 25, 6, -24, 12, 41, -17, 33, -20, 18, 0, -7, -26, 4, 16, 13, -34, 6, 25, -27, -1, 19, -6, 24, -2, 34, 29, -6, -46, -40, 14, 18, 18, 22, 41, 19, -12, -7, -25, 14, 49, -1, -30, -20, -24, 29, 36, -13, -9, 34, -2, -26, 14, -30, 15}
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
  //dense_2_output_type dense_2_output);
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
#include "max_pooling1d_8.c" // InputLayer is excluded
#include "conv1d_6.c"
#include "weights/conv1d_6.c" // InputLayer is excluded
#include "max_pooling1d_9.c" // InputLayer is excluded
#include "conv1d_7.c"
#include "weights/conv1d_7.c" // InputLayer is excluded
#include "max_pooling1d_10.c" // InputLayer is excluded
#include "conv1d_8.c"
#include "weights/conv1d_8.c" // InputLayer is excluded
#include "max_pooling1d_11.c" // InputLayer is excluded
#include "average_pooling1d_2.c" // InputLayer is excluded
#include "flatten_2.c" // InputLayer is excluded
#include "dense_2.c"
#include "weights/dense_2.c"
#endif

void cnn(
  const number_t input[MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES],
  dense_2_output_type dense_2_output) {

  // Output array allocation
  static union {
    max_pooling1d_8_output_type max_pooling1d_8_output;
    max_pooling1d_9_output_type max_pooling1d_9_output;
    max_pooling1d_10_output_type max_pooling1d_10_output;
    max_pooling1d_11_output_type max_pooling1d_11_output;
  } activations1;

  static union {
    conv1d_6_output_type conv1d_6_output;
    conv1d_7_output_type conv1d_7_output;
    conv1d_8_output_type conv1d_8_output;
    average_pooling1d_2_output_type average_pooling1d_2_output;
    flatten_2_output_type flatten_2_output;
  } activations2;


  //static union {
//
//    static input_3_output_type input_3_output;
//
//    static max_pooling1d_8_output_type max_pooling1d_8_output;
//
//    static conv1d_6_output_type conv1d_6_output;
//
//    static max_pooling1d_9_output_type max_pooling1d_9_output;
//
//    static conv1d_7_output_type conv1d_7_output;
//
//    static max_pooling1d_10_output_type max_pooling1d_10_output;
//
//    static conv1d_8_output_type conv1d_8_output;
//
//    static max_pooling1d_11_output_type max_pooling1d_11_output;
//
//    static average_pooling1d_2_output_type average_pooling1d_2_output;
//
//    static flatten_2_output_type flatten_2_output;
//
  //} activations;

  // Model layers call chain
 // InputLayer is excluded 
  max_pooling1d_8(
     // First layer uses input passed as model parameter
    input,
    activations1.max_pooling1d_8_output
  );
 // InputLayer is excluded 
  conv1d_6(
    
    activations1.max_pooling1d_8_output,
    conv1d_6_kernel,
    conv1d_6_bias,
    activations2.conv1d_6_output
  );
 // InputLayer is excluded 
  max_pooling1d_9(
    
    activations2.conv1d_6_output,
    activations1.max_pooling1d_9_output
  );
 // InputLayer is excluded 
  conv1d_7(
    
    activations1.max_pooling1d_9_output,
    conv1d_7_kernel,
    conv1d_7_bias,
    activations2.conv1d_7_output
  );
 // InputLayer is excluded 
  max_pooling1d_10(
    
    activations2.conv1d_7_output,
    activations1.max_pooling1d_10_output
  );
 // InputLayer is excluded 
  conv1d_8(
    
    activations1.max_pooling1d_10_output,
    conv1d_8_kernel,
    conv1d_8_bias,
    activations2.conv1d_8_output
  );
 // InputLayer is excluded 
  max_pooling1d_11(
    
    activations2.conv1d_8_output,
    activations1.max_pooling1d_11_output
  );
 // InputLayer is excluded 
  average_pooling1d_2(
    
    activations1.max_pooling1d_11_output,
    activations2.average_pooling1d_2_output
  );
 // InputLayer is excluded 
  flatten_2(
    
    activations2.average_pooling1d_2_output,
    activations2.flatten_2_output
  );
 // InputLayer is excluded 
  dense_2(
    
    activations2.flatten_2_output,
    dense_2_kernel,
    dense_2_bias, // Last layer uses output passed as model parameter
    dense_2_output
  );

}
