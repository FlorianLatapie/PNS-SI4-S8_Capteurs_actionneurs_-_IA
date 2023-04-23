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

typedef number_t max_pooling1d_32_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_32(
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

typedef number_t conv1d_24_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_24(
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


const int16_t conv1d_24_bias[CONV_FILTERS] = {-2, 2, 2, 0, 13, -26, 2, -21, -8, -12, 25, 11, 0, -1, 13, 53, -1, -17, -44, -5, 34, 1, -2, -2, 58, -17, 52, -15, 0, 18, 1, -20}
;

const int16_t conv1d_24_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{-18, 2, -8, -11, 6, -31, -37, -42, -42, 1, 19, 29, 19, -35, -41, -1, -15, -43, -11, 31, 23, -29, -22, 11, -6, -19, -29, -8, 25, 7, -12, -13, -4, 3, -2, 1, -19, -21, 5, -11, -18, 6, 2, 15, 31, 20, 8, -14, -23, -4, -1, -16, -11, 10, 36, 23, -18, -12, 8, 28, 31, -1, -1, -3, -1, 21, 7, -14, -7, 0, 0, 19, 35, 29, 9, 1, 9, 4, 22, 28}
}
, {{-8, -20, 0, 26, -34, 23, -31, 12, -22, -8, 10, -17, 8, 1, 10, -24, 3, -29, 8, -38, 6, -29, 19, -11, -25, 13, 4, 15, -6, -23, -2, -10, -17, 30, -11, 25, 17, -16, 16, -15, 0, -17, 22, 12, 21, 28, 16, 21, 31, -19, 7, -6, 25, -13, 4, 22, 17, 14, -3, 0, 27, 15, 7, 21, 5, -13, -17, -23, 25, 0, -13, -18, -13, 0, -24, -4, 13, -6, -20, -1}
}
, {{24, -19, 14, 0, 23, 1, -12, -33, 15, 0, 31, -41, -20, -24, -19, -19, -26, -19, 32, -21, 0, -43, -31, 19, 25, -6, -35, -1, 11, 16, 0, -26, 29, 31, -17, 30, -34, 37, -10, 13, -22, -13, -16, 25, 5, 16, 2, 13, 17, -3, 7, 2, 29, -16, 25, -2, 33, -14, 9, -3, 3, 1, -3, 23, 11, -36, 19, 4, 9, 0, 3, -21, -23, -8, 1, -25, 17, 14, 12, 0}
}
, {{0, 9, 7, -2, -13, -17, -12, 2, 19, -19, -7, 15, -1, 1, -19, -3, -13, -6, -1, 20, -21, 24, -44, 18, 16, -30, 21, -7, -17, 0, 19, -9, 4, 4, 15, 9, 25, -19, 39, -40, 22, -32, 40, -27, 9, -15, 20, -16, 26, -37, 30, -7, 20, -16, 9, 12, 5, 9, -14, 20, -5, -2, 21, -34, 25, -26, 5, -20, -4, -5, 24, -18, 13, -18, 3, -6, 2, 12, -3, 20}
}
, {{11, 17, -11, 4, 11, -1, 5, 4, -3, 10, 8, 11, -9, 2, -20, 3, -11, -30, 0, -30, -33, -16, -36, -5, -13, 3, -12, -2, -1, -32, -20, -39, -41, -38, -22, -26, -12, -23, -40, -16, -38, -24, -33, -27, -43, -35, -47, -38, -32, -19, -27, -11, -7, -9, -24, -28, -30, -34, -21, -24, -13, -34, -19, -10, -27, -32, -27, -23, -18, 0, -25, -13, -27, 0, 0, -10, 1, 0, -5, 1}
}
, {{30, 11, 29, 7, -6, 24, -14, 17, 15, 22, 0, 34, -12, 23, -15, -3, -18, 10, -11, 3, 11, -13, 26, 8, 7, 14, 28, -14, 24, -18, 23, -1, 25, -22, 3, -1, 29, -18, -23, 10, 15, -15, 20, -27, 25, 12, 13, 1, -14, 22, -15, 12, 1, 12, 0, -10, 14, -3, 9, -23, 8, 3, -18, 20, -27, 5, -4, -33, 12, -13, 10, -2, 16, 25, -12, 30, 12, -6, 27, -8}
}
, {{-1, 8, -8, 15, 11, 0, 9, 6, -5, 11, 6, 11, 1, 19, 4, -4, 14, -1, 6, 0, 0, 9, 10, 8, -4, 13, 4, 10, 19, 16, 6, 25, -10, 2, -16, 16, -13, 4, 26, -14, 27, -9, 19, 16, 44, 13, 36, 25, 18, 25, 1, 0, -6, -5, -1, 23, -8, 6, -13, 5, 20, 14, 6, -10, -4, 3, 23, -18, 10, -1, -2, 16, 1, 0, -15, 17, -24, -8, 8, -16}
}
, {{-9, 2, -18, 0, 12, 16, 2, -16, 8, 10, 14, -3, 10, -7, -12, 13, 6, -4, -27, -28, -3, 4, 24, 0, 8, -23, -1, -23, -11, -16, -18, 17, -18, -9, -24, -23, 8, 5, -10, 12, -22, -11, -1, 1, -6, -1, -21, 16, -18, 1, 16, 7, 6, -10, -30, -28, 18, 22, -10, 25, 7, -21, 0, -6, -13, 24, -2, -2, -15, 15, 3, -26, -24, 8, 7, -1, 19, -17, 0, 7}
}
, {{29, 14, -28, -45, -27, 16, 1, -26, 8, 14, -27, -11, 19, 14, -19, -42, -21, 17, -3, -4, -11, -24, 15, -3, -18, 7, 26, 16, -24, -28, -29, -33, 19, 33, 21, 13, -27, -49, -5, 22, -1, 0, 20, 13, -13, -10, 11, 8, 9, 2, 3, -11, -14, 14, 8, 36, 21, -9, 14, -2, -4, 29, 8, 0, 22, -18, -46, 0, 32, 45, 31, -14, -26, 4, 17, -19, -13, 30, 16, -13}
}
, {{22, 4, 7, 15, 10, 9, 4, 10, 14, -3, -3, 9, 13, 12, 14, 17, 14, -5, 14, 27, 4, 20, 27, 8, 6, 26, 20, 2, 16, 22, -4, -4, -10, -5, 19, 7, -7, 2, 3, 13, 11, -4, -10, -7, 5, 34, 23, 5, 6, 3, 16, -5, -16, 7, -10, -22, -5, 0, -9, 0, -1, 8, 13, 5, 24, 3, -8, 0, 13, 3, 0, 0, 8, -8, 5, 9, 1, 0, 26, 6}
}
, {{17, 6, -10, 22, 11, 19, 17, 3, 29, -17, 21, 3, 13, 20, 4, 5, -5, 13, 0, -29, 5, -10, -17, 0, -17, 8, -4, 3, -18, 7, -6, -16, 13, 5, 6, 10, 6, -5, 10, 14, 12, 22, -11, -9, 18, -20, 19, -28, 19, 0, -11, 18, -4, 26, -37, 13, -31, 19, 7, -9, 11, -2, 12, -22, 3, -7, -12, -7, 32, -25, 10, -18, -4, -24, 15, -23, -11, -3, -14, 19}
}
, {{12, 5, 7, 4, -21, -1, -4, -3, 0, 1, 4, 7, -31, 14, -17, -4, -1, -12, 5, 13, -8, -1, 20, -2, 11, 6, 7, -22, 15, -2, 10, -9, 3, 3, -12, 26, -31, 31, -20, -3, 0, -7, -22, 23, -24, 2, 11, -11, 4, 22, -29, 28, -13, 35, -24, 36, -22, 25, 11, -11, 15, -35, 35, -48, 47, -31, 25, 9, -9, -8, 4, -13, -6, -10, -4, 14, -23, 20, -16, 13}
}
, {{-35, 35, -26, 14, -29, 23, 11, -6, -2, 14, 1, -4, 24, -11, -1, 7, 18, 12, -4, 30, -5, -18, -14, 16, -18, 12, 0, 20, -16, -16, 11, -16, 35, -50, 30, -30, 16, 11, -4, -14, -10, 22, 2, 25, 7, 20, 10, -10, 10, 12, -12, -3, 9, -17, 26, -12, -8, -32, 13, -32, 20, 4, -12, -4, -10, 1, -21, -23, -21, 0, 4, 20, -24, 21, -1, -6, 31, -34, 23, 7}
}
, {{-2, -13, -6, -11, -4, 12, -11, 23, 2, -25, 28, -35, 7, -14, 12, -23, 26, 22, 1, 7, 22, 3, 25, 5, -6, 13, 6, -14, -3, -4, -16, 0, 11, 13, 10, -17, 1, 13, 31, -29, 11, 6, 1, -17, 13, 29, -11, -12, 33, -25, 32, -13, 17, -11, 35, 0, 2, 20, -20, 16, -14, 0, -13, -14, 3, 20, -26, -15, 16, -16, -10, 3, 17, -9, 0, -17, 15, 19, -19, 2}
}
, {{-21, -14, 17, 27, 15, -8, -32, -15, 25, 14, -23, -11, 18, -34, -25, 12, 9, -11, -14, 16, 20, -22, -4, -3, 32, -13, 11, 3, 23, 17, -43, 14, 0, 17, -4, -8, 22, -19, -1, 1, 20, -8, 5, -16, -18, 16, -17, -15, 23, 17, -14, -17, -8, 6, 22, -5, -30, -26, 33, 4, -28, 5, 19, 9, -7, 0, -11, -28, -22, 15, 14, -42, 23, 35, -44, -23, 26, 36, 9, -41}
}
, {{-28, 21, 10, 1, 10, 9, 16, -16, -1, -28, -17, 0, -17, 11, 6, -17, -22, -26, 7, -11, -38, -3, -12, 7, -20, -25, 8, -22, -17, -29, -22, -37, -20, -21, -24, 7, -4, -13, 2, -7, 17, -2, 7, 8, 13, 20, -11, 12, -12, -21, -23, 10, -37, -37, -4, -41, -8, -22, -15, -39, -38, -28, 15, -20, -3, -36, -10, 3, -11, -14, 9, -26, -23, -8, -3, 3, -50, -26, -17, -19}
}
, {{11, 15, 8, 9, -12, -7, -18, 22, -1, 20, -13, 7, 11, -22, 30, -26, -4, -6, -10, -2, 1, -6, 12, 2, -18, 16, -13, 35, -33, 15, -6, -2, 3, -24, 21, -21, 22, -5, -2, 22, -20, 12, -7, -24, 35, -25, 15, 0, 0, 0, 25, -31, 23, -16, -2, 18, 19, -18, 16, 2, -13, 8, -27, 31, -24, 14, 8, -3, -10, 0, 15, -22, -1, 4, -24, 26, 3, 8, -9, 8}
}
, {{15, 31, 25, 36, 17, 28, 14, -4, 21, 32, 26, 7, 30, 9, -3, 22, 17, 11, 16, 25, 22, -16, -7, 9, 30, -24, 21, 1, 30, 2, -17, -20, 16, 6, 26, -16, 8, 14, -16, 29, -23, 12, -23, 22, 13, -1, 32, -20, 22, -1, -15, 13, 21, -20, 4, -19, 12, 1, 4, -6, 12, 15, -5, 20, -26, 0, 10, 1, -15, -16, -9, -25, 15, -1, -9, 3, -13, -11, -15, 27}
}
, {{-19, 21, -12, 4, 0, -15, -26, -2, 6, -1, 8, -15, -1, -21, 1, 16, -21, 1, -3, -6, -1, 19, -8, -11, 12, 10, 1, -18, -23, 0, 15, -11, 8, -18, 9, 1, 4, 9, 3, -16, 7, -29, 12, -34, -21, -28, 0, -10, 18, -30, -12, 12, 12, 12, -20, 2, 1, -21, 0, -20, -29, 15, -5, 15, -1, -28, -6, -3, -5, -17, -9, -13, -4, -10, -30, -5, -8, -3, -5, 6}
}
, {{-19, 14, -6, 5, 8, 18, -27, -12, -8, 13, 17, -1, 11, -18, -27, 19, 0, 14, -2, 5, -12, -21, 30, 10, -18, 13, -21, 15, -3, -10, 7, -23, 13, -5, -23, 16, -14, -4, 16, -18, -19, -17, 4, 14, 6, -13, -20, -1, -29, 7, 17, -27, -2, -24, -8, 17, -19, -23, -21, 18, 18, 9, -25, 0, 6, -23, 10, -15, 22, 25, -17, -16, -28, -4, 32, 20, 2, 18, -32, -8}
}
, {{-27, -8, -36, 0, -20, 6, 10, -5, -22, -35, -18, -16, 21, 1, 19, 9, -16, -12, 12, -19, 9, 14, 28, 23, 26, 0, -5, -14, -18, -2, -34, 4, -27, 17, 4, 2, -5, -11, -1, -22, -35, -31, -39, -14, 4, -5, -21, -32, -40, -25, -10, 1, 0, 6, -18, -25, -5, -29, -4, 1, 4, 21, -2, 16, -19, 3, 0, 7, -13, -7, -15, -33, -1, 0, -4, -1, -21, 1, 0, -6}
}
, {{-7, 9, -19, -23, 24, 12, -7, 17, -5, 11, 1, -16, -12, 19, -19, 15, -7, -9, 31, -15, 4, -20, 12, -40, 22, -36, -6, -30, 12, -17, 31, -25, -7, 29, -33, -4, -6, -14, 2, -6, 11, 5, 21, -21, -7, 11, 12, 0, 25, -18, -3, -1, 4, -10, 23, 9, 27, -19, 10, -16, -13, 14, -11, -2, -27, 29, -2, -17, 7, 12, 26, 10, 12, -25, -11, 2, 12, -4, -16, -6}
}
, {{18, -12, 14, -28, 22, 19, 26, -1, 0, 15, 7, 7, 0, 8, 23, -25, -3, -24, -11, 19, -10, -2, -20, 26, -14, 28, -14, 24, -26, 29, -41, 32, -23, -20, 4, -10, -1, -15, 7, -3, -20, -11, 17, -24, 1, 15, -9, 6, -10, -9, 0, -20, -8, 15, 10, -14, 11, -10, 11, -28, 23, 6, 19, 13, 7, 6, 1, -19, -24, 20, 15, -14, -13, 1, 17, -3, 17, 10, -6, 18}
}
, {{-5, -12, -5, 13, 27, -42, 18, 4, -18, -23, 17, -10, -27, 9, 1, 0, -10, -22, 10, 15, 9, -8, -6, -23, 40, -35, 38, -28, 25, -6, 24, 13, -26, 17, -14, 14, -24, 14, 2, 13, -2, -1, -16, 21, -23, 16, -7, 18, 3, 18, -3, -18, 7, 12, -26, -12, 15, -18, -13, 9, -1, -9, 14, 9, 8, -12, -18, 32, -36, 21, -10, 26, 0, 13, 13, 20, 1, 15, 12, -21}
}
, {{-2, 11, -11, 5, 19, -6, 34, -8, -18, 9, 13, -21, -4, -20, 30, 25, 0, -12, -25, 0, -2, -1, -4, -2, -12, -12, -6, -22, -9, 3, -3, 29, 18, 6, -22, 9, 12, 11, 26, -11, 39, -11, 10, 18, 27, -6, 3, 11, 24, 30, -40, 25, 0, 26, -25, 15, -17, 21, 21, -27, 16, -40, 2, -34, 11, 0, -19, 24, 11, -13, -15, 10, -16, 32, -41, 14, -28, 6, 27, -19}
}
, {{-2, -1, 7, -21, -26, -7, 4, 33, -12, -14, -7, -20, 8, 0, 2, -2, 3, -13, -19, -40, 3, 21, 18, 1, -26, -26, -33, 7, 35, 20, -1, -32, -18, 0, -10, 0, -15, -6, 12, -1, -10, 2, -3, -9, -2, -20, 19, 0, -30, -21, -25, -7, -16, 0, -19, 9, 28, 3, 6, -30, -26, 2, 0, 14, -18, -31, -10, -9, -17, -4, -8, 7, 35, 3, -38, -18, 6, 28, 20, -3}
}
, {{11, 10, -3, -27, 0, 26, 1, 2, -5, -6, -11, 0, 28, 33, -6, -27, -26, -9, 14, -26, -41, -39, -7, 20, -6, -44, -23, -13, 9, -16, -34, -59, -30, 15, 9, -7, 7, 8, -29, -35, -28, -33, -1, 15, -22, -10, 16, 20, 3, -16, 26, 29, -6, -1, 7, 32, 24, -14, -17, 2, 4, 8, 37, 23, -12, 9, 17, -7, 13, 23, 14, 6, 1, -31, -22, -17, -4, 34, 20, -7}
}
, {{20, 6, 16, 23, -4, 8, 24, 0, -1, 17, 0, 7, 13, 3, -6, 20, 4, -15, 21, 23, 9, 22, 14, 11, 21, 26, 10, -10, -4, 20, 23, 1, 9, 15, -6, -3, 0, -1, 18, 21, 0, 2, 17, -2, -5, 14, 22, 14, 14, 10, 4, 22, 3, -12, -10, -1, 18, 8, 15, 26, 34, 30, -1, 10, 14, 18, 28, 6, -14, 3, 0, 0, 22, 5, 12, 14, -11, -4, 8, 3}
}
, {{-19, -8, -15, 32, 14, 15, -9, -15, -10, -24, 25, 0, -21, 33, -22, 20, 12, 8, -27, 15, -3, 24, -2, 6, 15, -17, 31, -36, 17, -4, 3, 4, 23, -26, 22, -45, 35, -33, 5, 9, -31, -8, 1, 19, -2, -2, 30, -35, 0, -22, 15, 1, 18, -11, 1, -18, -19, 17, 20, 13, 19, -8, -5, 16, 20, -5, -8, -23, 13, -25, 19, 15, 19, 14, -24, -17, 15, -33, 27, -25}
}
, {{3, -3, -16, -26, 19, -33, 2, -8, -10, -16, 8, -9, 20, -16, 6, -16, -28, 6, -5, -7, -10, 17, -22, -4, -23, 29, -22, -14, 20, -8, 16, 14, 9, -19, 8, -12, -8, 14, 16, 25, -25, -9, 13, -2, -10, 7, 19, -7, -4, 7, -1, 24, 2, -10, 0, -16, 15, 28, -27, 23, -17, 31, 6, 13, -1, 13, 21, 12, 25, -7, 10, 0, 29, 7, 12, 33, -2, 29, 6, 24}
}
, {{22, -28, 44, -45, 42, -36, -8, -16, 11, -16, 27, -10, -8, 14, 8, 4, -27, 19, 13, 13, 20, -26, 0, -14, 20, 0, -7, 0, -18, 8, -13, -5, 14, 21, -5, 18, -6, -15, 14, 0, -4, -19, -20, -14, 26, -4, -6, -26, 24, 3, 15, -19, -2, -14, 19, -24, 27, 10, 20, 8, 4, 26, -23, 6, -15, 0, -8, 12, -1, 21, -5, 13, 13, -3, -15, 19, -32, -22, 0, 5}
}
, {{27, 7, 34, -4, 22, 14, 9, 27, 18, 26, 25, 0, 21, -19, -16, -13, -13, -30, 11, -29, 11, -12, 21, -8, 14, -5, 30, -26, 10, 14, -13, -12, 21, -6, -10, 28, -24, 18, 16, -15, 14, 8, -13, -18, -13, -4, -7, -14, 2, -3, 5, -6, 0, -14, -6, -4, 15, 8, 14, 4, 25, 9, 12, 30, -31, 11, -6, -20, 16, 0, -6, -16, 4, 3, 21, -13, 32, -6, 14, -14}
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
#define POOL_STRIDE     4
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_33_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_33(
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
#define INPUT_SAMPLES       380
#define CONV_FILTERS        16
#define CONV_KERNEL_SIZE    3
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_25_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_25(
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


const int16_t conv1d_25_bias[CONV_FILTERS] = {13, -15, -23, 17, 27, 12, 20, 50, -10, 11, 12, -11, 22, 3, 20, -18}
;

const int16_t conv1d_25_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{-107, 0, -14}
, {-119, -115, 48}
, {42, -93, -91}
, {73, 39, 23}
, {-64, 60, 69}
, {20, -49, 93}
, {-29, 40, -2}
, {-101, 89, -100}
, {-108, 59, 53}
, {-75, 94, 63}
, {-11, -4, 67}
, {-28, -57, 86}
, {-100, 11, -121}
, {8, -40, -45}
, {4, -49, 94}
, {-69, -13, 64}
, {80, -55, -38}
, {14, 35, 82}
, {-90, -103, 88}
, {-96, 76, -27}
, {83, -19, 30}
, {14, -45, 83}
, {4, 19, 40}
, {-75, -60, 72}
, {99, 49, -101}
, {0, -109, -104}
, {-63, -30, -71}
, {85, -70, -78}
, {65, -54, -67}
, {72, 66, -54}
, {-69, -52, -65}
, {101, -14, 92}
}
, {{-65, 103, 74}
, {107, 8, 31}
, {124, -42, -34}
, {121, -56, 65}
, {58, 96, 17}
, {-43, 100, -55}
, {22, -19, 77}
, {84, -13, 34}
, {21, -20, 62}
, {0, -79, 0}
, {-5, 15, -33}
, {2, -3, 89}
, {105, 128, 9}
, {9, -19, 45}
, {67, 81, -15}
, {-55, 73, 121}
, {-66, 107, 68}
, {-8, -101, -28}
, {84, 73, 87}
, {-70, -50, 9}
, {-1, 112, 72}
, {-21, 40, 12}
, {31, 80, -54}
, {39, 102, -64}
, {0, -20, 116}
, {-43, -23, -94}
, {-17, 86, -1}
, {51, 52, -28}
, {118, 116, 67}
, {-57, 5, -14}
, {85, 73, 126}
, {-22, 3, 33}
}
, {{14, -52, 41}
, {87, -10, -118}
, {-17, -101, 18}
, {-14, 104, 102}
, {-62, 34, 14}
, {-1, -42, -81}
, {-107, -61, 62}
, {11, 0, -65}
, {-27, 58, 91}
, {-19, 59, -83}
, {-60, 6, -51}
, {-12, -87, -31}
, {-82, 50, -50}
, {58, -93, -50}
, {102, 15, 3}
, {82, -3, 0}
, {52, 16, 95}
, {-41, 80, -21}
, {36, -22, -7}
, {104, -44, 79}
, {85, -90, 105}
, {75, 78, 83}
, {60, 63, -80}
, {-17, 17, -6}
, {21, -70, -88}
, {-54, 108, -84}
, {-100, -97, -44}
, {-21, 14, 54}
, {88, 107, 91}
, {-62, -22, -106}
, {9, -90, -27}
, {-46, -99, 41}
}
, {{-125, -72, -7}
, {-22, 65, -89}
, {81, -34, -55}
, {55, -82, 43}
, {77, 3, -62}
, {-36, 3, -67}
, {53, -48, 39}
, {79, 42, -42}
, {-92, -49, 66}
, {-17, -10, -31}
, {-58, -74, -43}
, {-65, -83, -46}
, {-47, 8, -116}
, {-82, 33, -71}
, {-18, -84, -89}
, {76, -70, 16}
, {46, 63, -43}
, {-62, -37, 3}
, {86, -30, 43}
, {89, -49, -114}
, {28, -23, 77}
, {-99, -10, -99}
, {-130, -81, -8}
, {-77, -133, -112}
, {34, -34, 92}
, {88, 93, -99}
, {6, 103, -73}
, {-19, -28, 5}
, {-88, -46, -38}
, {-14, 92, 23}
, {-83, -100, 49}
, {64, 6, 1}
}
, {{-72, 98, 0}
, {-15, 110, -71}
, {-26, 70, 27}
, {-55, 21, -61}
, {84, 115, -62}
, {44, -17, -99}
, {34, 14, -10}
, {85, -33, 28}
, {50, 125, 44}
, {19, -96, -70}
, {-74, -5, -26}
, {55, -98, 34}
, {-14, -67, -120}
, {4, -40, 19}
, {36, -86, 27}
, {-44, 39, -46}
, {-10, -88, 63}
, {57, 60, -53}
, {73, -68, -39}
, {43, -97, 75}
, {-45, -31, -3}
, {2, 35, 59}
, {-43, -15, 79}
, {12, 7, -97}
, {81, -37, 63}
, {-10, 6, 28}
, {42, -28, -17}
, {38, 90, -54}
, {85, -38, -23}
, {-53, -88, 54}
, {69, 51, -99}
, {39, -76, -107}
}
, {{-61, -58, -97}
, {70, -17, -104}
, {-113, -120, -25}
, {24, 29, -17}
, {35, -50, -16}
, {99, 6, 71}
, {112, 71, -7}
, {-59, -28, -61}
, {-102, -132, -38}
, {-27, 62, 8}
, {-2, 66, 92}
, {-81, 61, 65}
, {-20, 64, -45}
, {58, -43, 81}
, {-104, -59, -135}
, {117, 109, 98}
, {27, 59, 37}
, {55, 6, 95}
, {-99, -51, -70}
, {-54, -87, -18}
, {-42, 87, 24}
, {-35, -78, -30}
, {18, -58, 35}
, {-79, -31, -13}
, {38, -1, 56}
, {42, -118, -98}
, {-13, -78, 9}
, {113, 25, -15}
, {-24, -130, 70}
, {19, -42, -116}
, {-127, -13, -43}
, {29, -97, 55}
}
, {{12, 33, 46}
, {-75, -22, 39}
, {-56, 82, -53}
, {49, -16, -124}
, {66, 50, -23}
, {-58, -33, -62}
, {-19, 48, 77}
, {44, -71, -50}
, {47, 51, 16}
, {79, 88, 7}
, {88, -94, -76}
, {-132, -70, -70}
, {26, -103, 57}
, {72, 31, -62}
, {-56, 74, 116}
, {-37, -84, -14}
, {71, 2, -99}
, {-68, 18, -27}
, {-76, -82, -45}
, {-7, 101, -1}
, {27, 82, -67}
, {92, 88, -97}
, {1, -42, 0}
, {-88, 24, 8}
, {43, -13, -24}
, {41, -7, 109}
, {77, 93, 25}
, {-2, 58, 105}
, {-85, 3, 33}
, {-30, -26, -45}
, {-96, 48, -14}
, {-67, -18, -77}
}
, {{-14, 79, -9}
, {-44, -13, 90}
, {-100, -89, -40}
, {-98, -53, -13}
, {9, -23, 54}
, {-156, -150, -66}
, {-72, 17, 44}
, {-96, -6, 42}
, {-86, 12, -80}
, {-53, -22, -80}
, {-36, 87, 95}
, {85, -10, 89}
, {-97, -59, -65}
, {-39, -109, -8}
, {34, 57, -84}
, {50, 55, -65}
, {-148, -152, 12}
, {-124, -66, -31}
, {-42, -79, 7}
, {90, -18, 12}
, {82, 30, 70}
, {50, -69, -70}
, {-83, -36, -93}
, {-85, -114, -139}
, {95, 16, 5}
, {-97, -47, 95}
, {-57, 112, 121}
, {-67, -17, 67}
, {-116, 39, 0}
, {15, -50, 90}
, {-71, 68, 22}
, {-121, -36, 83}
}
, {{-57, -66, -46}
, {-55, 50, 74}
, {18, 9, 5}
, {26, -13, 121}
, {-140, 66, -76}
, {40, 84, -87}
, {-49, 81, -52}
, {-63, -22, 86}
, {-20, 25, -39}
, {-28, 2, 7}
, {25, 49, -31}
, {-46, 20, 78}
, {94, 110, -22}
, {-4, 104, 22}
, {-73, -35, 29}
, {-76, 55, 111}
, {100, 49, 106}
, {34, 25, 98}
, {-2, -96, 66}
, {-35, 0, -25}
, {36, -34, 38}
, {25, -17, 115}
, {56, -40, 61}
, {-45, 46, 87}
, {-70, -15, 117}
, {22, -12, 50}
, {25, 52, -10}
, {-54, -45, -87}
, {125, 46, 115}
, {-34, 38, 79}
, {-9, 83, 101}
, {-23, -72, 91}
}
, {{23, -25, -50}
, {102, -30, 109}
, {-80, 4, 67}
, {115, 47, 80}
, {-105, 18, 72}
, {-35, -25, -56}
, {-28, 98, 14}
, {82, 41, -93}
, {-49, -42, -17}
, {-25, 79, -42}
, {67, -101, -6}
, {27, 105, 119}
, {-14, 113, -1}
, {-84, 66, -50}
, {-2, -25, -20}
, {-81, -14, 137}
, {-20, 78, 77}
, {-30, -6, -6}
, {-61, -24, -76}
, {-96, 21, 20}
, {-54, -108, 7}
, {72, -87, 43}
, {29, -92, 21}
, {0, -38, -39}
, {12, 60, 85}
, {-23, -105, 14}
, {80, -26, -112}
, {2, -83, 98}
, {10, -15, -68}
, {81, -102, 63}
, {51, -2, 65}
, {-66, -36, -45}
}
, {{-51, -34, 102}
, {84, -42, 31}
, {15, -31, 81}
, {17, -79, -5}
, {-109, 63, -97}
, {14, -83, 29}
, {20, 33, -85}
, {39, 18, 91}
, {40, -89, 5}
, {9, 53, 71}
, {-80, 31, 10}
, {-4, -12, 34}
, {115, 73, 30}
, {115, -71, -66}
, {62, -58, -31}
, {-26, -82, -159}
, {-5, -75, 44}
, {43, 26, 90}
, {-4, -49, 44}
, {-67, -73, 1}
, {-90, -33, 61}
, {75, 58, -109}
, {-76, -31, 96}
, {11, -80, -71}
, {51, 124, -51}
, {80, -21, 70}
, {17, 0, 5}
, {-6, -91, 23}
, {36, -6, 27}
, {-110, -115, 10}
, {-8, 58, -11}
, {-52, -87, 115}
}
, {{-29, -107, -52}
, {-99, 56, 85}
, {7, 53, -80}
, {-87, 36, -47}
, {19, -39, -79}
, {22, -74, 6}
, {-73, -70, -107}
, {-27, -46, -110}
, {-28, 2, 19}
, {41, 72, 61}
, {46, 4, 92}
, {50, -30, 71}
, {30, -70, 4}
, {12, -47, -96}
, {76, -96, -89}
, {-19, -3, 29}
, {-36, -36, 51}
, {-98, -108, 13}
, {-107, -81, -108}
, {-31, -79, 33}
, {34, -90, -75}
, {36, 34, -49}
, {45, -49, -71}
, {42, 84, 94}
, {-71, -101, -82}
, {-16, 76, -67}
, {-51, -12, -74}
, {-23, -2, 23}
, {37, -104, 71}
, {-55, 3, -103}
, {-106, -12, 44}
, {11, 10, 47}
}
, {{42, 96, -84}
, {-11, -76, 65}
, {-3, 75, 47}
, {-22, 87, 47}
, {-58, 37, 72}
, {83, 47, -64}
, {29, -95, -49}
, {-55, 109, 56}
, {89, -84, 29}
, {-75, -7, -24}
, {-30, 69, 94}
, {-64, 119, -31}
, {-5, 32, 8}
, {41, 12, -59}
, {81, 35, -66}
, {-39, -92, -39}
, {-9, -55, -7}
, {-75, 89, -94}
, {-26, 50, 64}
, {73, 66, 53}
, {-91, 65, 64}
, {-19, -44, 29}
, {23, -10, 92}
, {103, -52, 27}
, {71, 88, 102}
, {42, -92, 99}
, {-16, 104, 104}
, {-82, 83, -114}
, {51, 90, 89}
, {83, 66, -3}
, {100, -18, 74}
, {20, -17, -75}
}
, {{30, -17, 25}
, {-30, -10, 117}
, {86, 93, 105}
, {122, -49, 60}
, {18, -90, -9}
, {-34, -50, -51}
, {36, -43, 27}
, {16, 59, -41}
, {-39, -16, -11}
, {-14, -96, 68}
, {115, -20, 71}
, {130, -2, 77}
, {121, 58, 89}
, {108, -17, -52}
, {56, 121, 78}
, {54, 98, 13}
, {68, -47, -30}
, {-3, 52, -106}
, {93, -97, 111}
, {-94, 61, 66}
, {-35, -44, -102}
, {85, -1, -1}
, {-57, -23, 58}
, {96, 69, -58}
, {-60, 40, 35}
, {30, -39, 68}
, {50, -6, -55}
, {-43, -96, -93}
, {50, 156, 79}
, {1, 18, 62}
, {9, 100, 16}
, {70, 0, 69}
}
, {{-28, 16, 1}
, {-43, 56, -106}
, {-82, -66, -69}
, {32, 19, -63}
, {100, 77, 89}
, {0, 34, 89}
, {75, -127, -79}
, {-49, 77, -17}
, {5, -73, 36}
, {25, -45, -31}
, {-39, -52, -14}
, {-84, -41, -7}
, {-65, -13, -15}
, {8, -23, -87}
, {43, -79, 44}
, {106, 41, 40}
, {32, -6, 74}
, {42, 41, 78}
, {-2, -90, 35}
, {-8, 9, 75}
, {68, -36, 21}
, {-37, 52, -105}
, {46, -20, -23}
, {29, -81, 42}
, {-53, -13, -16}
, {-1, -90, 68}
, {-55, 37, 64}
, {0, -26, -85}
, {-39, -101, -142}
, {-121, -14, -48}
, {-54, -77, 73}
, {34, -50, -44}
}
, {{112, 31, 46}
, {-21, 24, 57}
, {74, -74, -22}
, {-46, 112, 90}
, {76, 11, 56}
, {51, 83, -85}
, {38, 20, -79}
, {31, 47, 70}
, {44, -12, 39}
, {62, -87, 96}
, {38, -46, -51}
, {1, 5, 50}
, {-62, 101, 86}
, {2, -3, 37}
, {-71, -80, -48}
, {21, -32, -53}
, {8, -45, 13}
, {-88, -52, -37}
, {-75, -3, -74}
, {7, -3, 43}
, {-69, 47, -9}
, {68, -81, -88}
, {-101, 52, -31}
, {-9, 70, -70}
, {116, -66, -73}
, {59, 21, 61}
, {76, -102, -88}
, {77, -16, -36}
, {48, -75, -7}
, {-72, -5, -94}
, {108, -72, 58}
, {2, -27, -71}
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
#define INPUT_SAMPLES   378
#define POOL_SIZE       4
#define POOL_STRIDE     4
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_34_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_34(
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
#define INPUT_SAMPLES       94
#define CONV_FILTERS        8
#define CONV_KERNEL_SIZE    7
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_26_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_26(
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


const int16_t conv1d_26_bias[CONV_FILTERS] = {-15, -10, 16, 27, 17, 33, 12, -6}
;

const int16_t conv1d_26_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{-1, -26, -8, 28, 77, 30, -8}
, {45, 10, 50, 10, 38, 65, 41}
, {-78, 46, -10, 92, 79, 53, 32}
, {36, -25, -70, -41, 58, -44, -74}
, {-105, -70, -9, -43, 66, 49, -23}
, {-25, 84, -58, -74, -90, 12, 21}
, {-68, 7, -43, -114, -96, -79, 29}
, {94, 29, 71, -36, 79, 58, -80}
, {50, 113, -64, 9, 118, 23, 120}
, {-39, 83, 91, 87, 108, -79, -67}
, {-88, -81, -92, 76, 41, -21, -55}
, {-65, 80, -20, -57, 60, 39, -28}
, {-87, -13, -61, 24, 74, 85, 20}
, {-60, -48, 31, 36, 107, -61, 41}
, {-46, 58, -45, -40, 41, -53, -61}
, {-25, -36, -22, 103, 34, 25, -25}
}
, {{-17, -26, 71, -72, -70, -31, -7}
, {86, 77, 79, 39, -28, 19, -17}
, {30, -85, -63, -42, 75, -38, 36}
, {-4, 17, -135, -48, -37, -50, -61}
, {81, -15, -47, 10, -44, 0, -106}
, {-6, 28, 68, 14, 48, -29, -55}
, {-17, -69, 52, -2, -61, -4, -80}
, {111, 75, 138, 21, 11, -14, 133}
, {-48, 110, 112, 12, 96, 25, 69}
, {82, 50, 19, 0, 10, 42, 34}
, {-73, 5, 25, 33, 90, 25, 43}
, {23, 92, -16, 16, -94, 61, 79}
, {74, -35, 5, -109, -90, -25, -112}
, {-31, 60, -55, -43, 83, -34, 72}
, {99, -79, -67, -100, -113, -55, -20}
, {60, 24, 41, -44, 45, 30, -90}
}
, {{62, 57, 64, -1, -38, -78, -88}
, {-53, 7, -6, -64, -6, 76, 39}
, {-51, 3, -4, 28, -8, -65, 69}
, {52, 12, 45, 81, -47, -13, -30}
, {-68, 109, 92, -81, 41, 26, -48}
, {-42, -81, -62, -65, 14, -114, -113}
, {10, 16, 30, 48, 54, 87, -77}
, {-82, -91, -90, -179, -138, 4, -55}
, {-20, 97, 63, 111, 46, -58, -23}
, {64, -11, -57, 55, 38, 36, 61}
, {-109, 8, 20, 105, -43, 108, 86}
, {85, -15, -3, -70, 0, 79, 60}
, {-49, 95, 16, 104, 109, 53, -33}
, {54, 95, -80, 1, -58, 58, 95}
, {-6, -10, -27, 91, 37, 35, -30}
, {24, 43, 58, -48, 5, -54, 14}
}
, {{-41, -83, -40, 36, 20, 52, -20}
, {62, 80, -75, -27, 59, -22, -9}
, {-79, -86, 85, -75, 20, -19, -5}
, {56, 40, 75, 19, 67, -63, -97}
, {-1, 93, 103, 45, 104, 11, 26}
, {-81, -29, -60, 43, 95, -38, -44}
, {71, 33, 92, -40, 33, -38, 61}
, {121, -48, -16, -47, -57, -22, -60}
, {-5, -101, -22, -100, 67, 45, -14}
, {66, -5, 6, -51, 17, -37, -91}
, {-36, -15, -70, 96, -40, 94, 79}
, {15, -21, 43, -39, -60, -81, 28}
, {109, -3, -54, -47, 68, -42, -40}
, {21, -99, -36, 40, -7, -33, 4}
, {11, 113, -52, -61, 54, -2, -36}
, {-102, -97, -88, -34, 0, -83, -6}
}
, {{38, -77, -62, 18, 48, 38, 18}
, {74, 75, 79, -49, -80, -52, 43}
, {31, 26, 56, -59, -13, -22, -85}
, {2, 14, -11, 67, -2, -61, 73}
, {46, 98, -19, 99, -82, -56, 100}
, {-52, 84, 65, 79, 41, -17, -48}
, {62, -56, 8, -15, -67, -20, -62}
, {71, 119, 110, -14, 116, 45, 90}
, {-68, 68, -93, -30, -75, 83, -93}
, {62, 97, 49, -84, -20, -75, -24}
, {3, 6, 84, 80, 49, 54, 6}
, {86, -73, 16, 47, 47, 90, -77}
, {-21, -81, 8, -71, 19, 91, 52}
, {45, -85, 2, -109, 21, -96, -91}
, {-38, 9, -14, 37, 79, 56, 95}
, {-80, 70, -1, -25, -6, 5, -32}
}
, {{80, 25, 82, -5, 29, -28, -52}
, {-58, -53, -69, 5, -32, -80, -109}
, {21, -63, -35, -40, -71, 44, -27}
, {-49, 18, 101, -15, -57, 91, -3}
, {-38, -70, -76, -18, 56, 19, 37}
, {-64, 42, 45, 21, 16, 31, 50}
, {61, -53, 35, 87, 39, -12, -31}
, {89, -6, 66, 29, 118, -79, 61}
, {-89, -95, -44, -99, -120, -26, -22}
, {37, 65, 26, -37, 33, -2, 71}
, {80, -14, -33, -67, 58, 31, 9}
, {-8, 81, -79, -54, -12, -35, 72}
, {88, -70, -95, -86, -42, 91, 80}
, {64, -75, -117, -1, 22, -67, 31}
, {-28, 90, 68, 176, 115, -8, 97}
, {85, 97, -1, -62, 64, 34, -71}
}
, {{-1, 40, 83, 51, 33, -100, -65}
, {-25, -13, 87, -13, -86, -65, 36}
, {9, -52, -81, 20, 96, 38, 48}
, {-64, 69, -32, 51, 54, -64, 63}
, {-4, 13, 48, 3, -62, -93, -98}
, {7, 6, 47, -49, 90, 20, 48}
, {-41, -4, -1, -16, 34, 96, -4}
, {-69, -93, -110, -42, -124, -113, 10}
, {-5, 80, -76, -11, -30, 18, 22}
, {19, -78, 62, -9, 0, 73, 30}
, {-22, 10, -46, 6, -14, -72, -97}
, {86, 8, -2, 57, -52, 57, -68}
, {-71, -10, 103, 98, 107, -65, 120}
, {111, -9, 146, 80, 61, -10, 88}
, {-49, -41, 71, -16, -88, -48, 97}
, {-3, -33, 66, 84, 43, -21, 81}
}
, {{36, 36, 77, 0, -37, 74, -88}
, {-87, -3, -8, -75, 55, -88, -27}
, {-14, -89, 10, 89, 16, -40, -61}
, {17, -98, -43, 67, -17, -35, -14}
, {15, -97, 6, -94, 40, -14, 61}
, {-35, -97, -46, 35, -20, -22, -9}
, {-93, -76, -94, 81, 64, -68, 29}
, {84, -56, -65, -34, -73, 33, -9}
, {66, 51, -26, 78, -94, -17, -46}
, {-34, -41, 39, -76, -16, 79, 26}
, {43, 27, 74, -2, 27, 25, 84}
, {-68, -80, 49, -29, 79, 79, -54}
, {-43, -11, 68, -63, -77, -26, -60}
, {-49, 27, 3, 84, 20, -30, 17}
, {-3, -94, 11, 66, 46, -74, -63}
, {-29, 83, -76, 48, 80, 19, -63}
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
#define INPUT_SAMPLES   88
#define POOL_SIZE       4
#define POOL_STRIDE     4
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_35_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_35(
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
#define INPUT_SAMPLES   22
#define POOL_SIZE       2
#define POOL_STRIDE     2
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t average_pooling1d_8_output_type[INPUT_CHANNELS][POOL_LENGTH];

void average_pooling1d_8(
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

#define INPUT_DIM [11][8]
#define OUTPUT_DIM 88

//typedef number_t *flatten_8_output_type;
typedef number_t flatten_8_output_type[OUTPUT_DIM];

#define flatten_8 //noop (IN, OUT)  OUT = (number_t*)IN

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

#define INPUT_SAMPLES 88
#define FC_UNITS 3
#define ACTIVATION_LINEAR

typedef number_t dense_8_output_type[FC_UNITS];

static inline void dense_8(
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

#define INPUT_SAMPLES 88
#define FC_UNITS 3


const int16_t dense_8_bias[FC_UNITS] = {-3, -16, 16}
;

const int16_t dense_8_kernel[FC_UNITS][INPUT_SAMPLES] = {{-40, -16, 105, -58, 31, 2, -29, 20, 25, -144, -150, -33, -178, -113, -94, -178, -83, -105, -86, -32, -140, -50, 109, 111, 126, 125, 29, 156, 22, 77, 49, -50, 150, 16, -123, -11, -51, -21, -37, 114, -123, -102, 37, 85, 75, 55, 75, 49, 106, 40, -110, 93, 82, -19, -111, 129, 10, -60, -53, -72, 117, 52, 65, -59, 94, 150, 65, -29, 55, -61, 140, 116, 5, 53, 109, 125, 33, 53, -90, 63, -68, 68, -90, 4, 71, -83, 108, 126}
, {45, 140, 75, 4, -35, 75, 129, 104, -27, -20, 103, -8, 150, 105, 69, 49, 70, 16, -67, 140, -41, 50, -86, -86, -146, -17, 27, -16, -65, -92, -77, 20, -138, -141, -165, 46, -125, -30, 78, -133, -154, -19, 48, -9, 90, 69, -81, -72, 74, -51, -52, -50, 95, 74, 52, -182, 4, -140, -119, -126, 41, 1, -48, -146, -90, -165, 94, 92, -20, -86, 113, 23, 36, 106, 28, -63, 125, 64, 107, 36, -3, 102, -68, -50, -36, -15, 87, 19}
, {47, -114, 11, 89, -133, -70, -107, 7, -59, -25, -61, 70, 102, 65, -109, -115, 23, 3, -104, 64, -65, -95, -73, -111, 27, 92, -140, 7, -78, 25, -25, -11, -144, 94, -54, 108, 47, 154, -2, 133, 87, -78, 10, 86, 120, -43, 112, -11, 87, 118, 105, 147, 33, 139, 115, 22, 22, -12, -78, -109, 67, -110, 12, -20, 73, -34, 36, -81, -82, 13, 52, -117, -52, 6, -85, -48, -121, -15, 9, 66, 38, -29, 95, 48, 26, -83, 59, -43}
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
  //dense_8_output_type dense_8_output);
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
