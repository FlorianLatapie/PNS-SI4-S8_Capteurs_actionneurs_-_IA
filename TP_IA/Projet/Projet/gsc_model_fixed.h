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
#define POOL_SIZE       20
#define POOL_STRIDE     20
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_60_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_60(
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
#define INPUT_SAMPLES       800
#define CONV_FILTERS        8
#define CONV_KERNEL_SIZE    40
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_45_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_45(
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
#define CONV_FILTERS      8
#define CONV_KERNEL_SIZE  40


const int16_t conv1d_45_bias[CONV_FILTERS] = {6, -34, -16, -14, -14, 6, 24, -11}
;

const int16_t conv1d_45_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{46, -24, 33, -12, -38, -39, 27, -43, 17, 25, 58, 60, 13, 33, -25, 2, 58, -45, 32, 22, -48, 16, 54, 64, 48, 35, 22, 12, 42, -33, 64, 33, 59, 55, 79, -16, 48, 47, -28, -38}
}
, {{-53, 24, -78, -10, -55, 16, -80, 5, 42, -81, 36, -13, -47, -3, 28, -83, -73, 38, 17, 18, -68, 58, 16, -31, 69, 64, 0, -21, 54, 66, 41, -30, -48, -39, -38, 45, 4, 48, 40, 54}
}
, {{-54, -90, -60, -37, 10, 24, 26, -38, -68, -79, 46, -33, -9, -53, -13, -26, -68, 38, 28, -51, 48, -9, -26, 65, 79, -19, -54, -23, 61, -31, 37, 45, -17, 36, -40, 85, 12, 62, 42, 44}
}
, {{-70, -9, -12, 3, -24, 9, 19, -33, -30, 19, 33, -14, 13, 65, -61, -41, 56, -26, 52, 16, 20, 62, 60, 46, -69, 56, -69, -61, 36, 42, -63, -50, -36, -9, 5, 30, 8, -19, -81, 52}
}
, {{8, -62, 41, 59, -7, -10, 45, 0, 27, 17, 19, 54, -62, -58, -57, -20, -45, -53, 11, -62, -50, 58, 32, 20, 60, 22, -3, 29, 53, 24, 46, -46, 1, 0, -18, 24, -11, -77, -47, 9}
}
, {{19, -7, -72, -90, -85, -41, -79, -55, -49, 60, 48, 5, 37, -30, -2, 61, -17, 48, -12, 49, 55, 44, -32, 47, -44, -1, 85, 38, -25, -41, -71, 12, 42, 11, -35, -25, -28, -79, -60, -73}
}
, {{-66, -59, -48, -30, 44, 26, -44, -22, 43, -36, -19, -6, -73, 9, -2, 0, -60, -4, -67, -47, -16, 21, -34, -42, 1, -41, -16, 11, -13, 10, -64, -52, -34, -55, -44, -16, 11, 41, 16, -16}
}
, {{-83, -98, -64, -22, 2, -70, 34, -16, 2, -23, -68, 44, -2, -70, 36, 31, 1, -31, 62, 71, -49, -56, 65, 46, 3, -45, -37, 83, 87, -37, -4, -48, 21, 72, -33, 53, -24, 16, 67, -9}
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
#define INPUT_SAMPLES   761
#define POOL_SIZE       4
#define POOL_STRIDE     4
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_61_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_61(
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

#define INPUT_CHANNELS      8
#define INPUT_SAMPLES       190
#define CONV_FILTERS        16
#define CONV_KERNEL_SIZE    3
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_46_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_46(
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

#define INPUT_CHANNELS    8
#define CONV_FILTERS      16
#define CONV_KERNEL_SIZE  3


const int16_t conv1d_46_bias[CONV_FILTERS] = {-4, -10, 47, 10, 2, -23, -39, -19, -4, 64, 18, -17, -13, 2, 16, 13}
;

const int16_t conv1d_46_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{2, 85, -92}
, {-97, -61, 133}
, {-135, 34, 171}
, {107, -112, 15}
, {103, -19, 8}
, {-54, 28, 80}
, {67, -13, -23}
, {-139, -74, -121}
}
, {{-94, -47, -112}
, {47, 31, 145}
, {167, 150, 61}
, {131, 98, 65}
, {124, -62, -36}
, {-34, 176, 133}
, {7, -137, -53}
, {126, 87, 19}
}
, {{-95, -91, -4}
, {-130, 28, 159}
, {-42, -8, 87}
, {-96, -165, 41}
, {-171, -13, -179}
, {-38, 27, 48}
, {-101, 118, 39}
, {50, -94, -94}
}
, {{25, -1, 120}
, {-154, 20, -135}
, {-101, 99, -145}
, {-97, 92, -49}
, {-162, -134, 78}
, {-96, 84, -59}
, {128, -39, -131}
, {32, -110, -17}
}
, {{54, 61, 41}
, {-88, -70, 80}
, {28, -9, -40}
, {73, 104, -42}
, {-83, 95, 149}
, {4, -117, 54}
, {-64, 108, 38}
, {-81, -63, -163}
}
, {{107, 59, -114}
, {134, 70, 50}
, {-126, 93, -22}
, {-7, -58, 146}
, {207, 233, 203}
, {147, 156, -20}
, {120, -115, 17}
, {0, -91, -84}
}
, {{28, 64, -71}
, {101, 155, 140}
, {69, 135, 96}
, {-2, 43, 65}
, {0, 110, 124}
, {-137, -49, -76}
, {-116, -6, 1}
, {78, 72, 35}
}
, {{50, -23, -82}
, {84, -123, -76}
, {-15, 41, 40}
, {138, -23, 59}
, {-81, -109, 33}
, {13, 128, -140}
, {33, 51, -36}
, {-70, 52, 112}
}
, {{-64, 57, 132}
, {11, -110, 5}
, {20, 164, 166}
, {87, 114, -54}
, {-113, 30, 8}
, {-97, -31, 95}
, {21, 149, 130}
, {124, -31, 145}
}
, {{-91, -96, -76}
, {42, -103, 124}
, {-178, -94, -123}
, {15, 55, -91}
, {-81, -67, 63}
, {-36, 75, 4}
, {38, 86, 48}
, {-179, -109, 63}
}
, {{-5, 3, 103}
, {-15, 29, 39}
, {-109, -79, -147}
, {-65, -28, -63}
, {-134, -64, -113}
, {-66, -192, 107}
, {-82, -32, -14}
, {-211, 54, -138}
}
, {{-7, -13, -39}
, {144, -103, 18}
, {-5, 59, 41}
, {114, -89, 47}
, {31, -9, -117}
, {-19, 52, 72}
, {15, -73, 129}
, {133, 91, 139}
}
, {{148, -40, -73}
, {-61, -136, -123}
, {99, 96, 79}
, {51, 177, -61}
, {-62, 40, -136}
, {29, -15, -107}
, {-39, 99, 113}
, {-93, 38, -98}
}
, {{126, 47, 79}
, {34, -138, 22}
, {101, -109, 46}
, {-23, 43, -57}
, {-133, -3, -2}
, {84, 140, 72}
, {36, -23, -3}
, {-176, 66, 74}
}
, {{13, 124, -54}
, {-45, 156, -130}
, {110, 135, 24}
, {-75, -93, 36}
, {77, 24, -51}
, {54, 2, 38}
, {-95, -17, -6}
, {-78, -68, -72}
}
, {{61, 11, 93}
, {-58, 47, -66}
, {-40, 2, 15}
, {59, -21, 92}
, {11, -49, -62}
, {-101, -30, -117}
, {-152, -108, -128}
, {-27, -57, -131}
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
#define INPUT_SAMPLES   188
#define POOL_SIZE       4
#define POOL_STRIDE     4
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_62_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_62(
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
#define INPUT_SAMPLES       47
#define CONV_FILTERS        32
#define CONV_KERNEL_SIZE    3
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_47_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_47(
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
#define CONV_FILTERS      32
#define CONV_KERNEL_SIZE  3


const int16_t conv1d_47_bias[CONV_FILTERS] = {-5, -3, 10, 6, -3, 0, 8, -2, 8, 5, 4, 1, -11, -8, 22, -1, -5, -5, 17, 4, -3, -2, -10, -25, -3, 34, -1, -1, -7, 13, -8, 7}
;

const int16_t conv1d_47_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{-14, -58, 42}
, {108, 6, -41}
, {-8, -29, -10}
, {-114, -113, 76}
, {-116, -119, -89}
, {23, 4, 42}
, {103, -20, 76}
, {94, 61, -48}
, {64, -25, 108}
, {-80, 21, 62}
, {13, -75, 67}
, {12, -34, 104}
, {-122, -106, -42}
, {-88, 15, -13}
, {47, -90, -11}
, {-18, 9, 79}
}
, {{16, 68, -55}
, {60, -51, -94}
, {165, 171, 164}
, {-104, -2, 63}
, {-44, -5, 57}
, {-109, 67, 52}
, {86, 121, -4}
, {36, 21, -102}
, {-47, -19, 66}
, {132, -5, 137}
, {70, -98, 18}
, {-117, 95, -80}
, {13, -110, 28}
, {-12, 17, -61}
, {15, 88, 75}
, {-80, 41, 26}
}
, {{74, -68, 23}
, {80, 20, 73}
, {-26, 33, -85}
, {-58, 78, -69}
, {60, 64, 84}
, {-5, 54, -57}
, {-77, 50, 78}
, {-6, 112, 36}
, {76, 92, -85}
, {112, -65, -73}
, {-12, -41, 66}
, {-119, 10, -2}
, {71, 74, 16}
, {-59, 66, -78}
, {60, -50, 99}
, {-55, 70, -2}
}
, {{108, 7, 37}
, {-92, -85, -131}
, {92, 169, 104}
, {-42, 79, -68}
, {15, -84, -71}
, {99, -6, 38}
, {99, -40, 56}
, {75, -58, -7}
, {121, -15, -65}
, {131, 140, 78}
, {-42, -74, 29}
, {-72, -91, -24}
, {102, 32, 50}
, {115, 28, 12}
, {-67, -49, 37}
, {-60, 6, -69}
}
, {{-86, -81, 75}
, {-79, -87, 89}
, {-81, 1, -94}
, {-29, -18, 14}
, {35, -35, -60}
, {-91, -49, 24}
, {-29, 99, -82}
, {65, 59, 63}
, {-50, 19, -50}
, {46, 72, 14}
, {-83, -41, -6}
, {-76, 67, -58}
, {-85, -27, 38}
, {-52, -9, 82}
, {-85, -99, -70}
, {-8, 98, 91}
}
, {{-121, -26, 48}
, {83, -109, 64}
, {13, 90, 119}
, {-86, -76, -83}
, {-33, 87, 25}
, {-74, 53, -11}
, {39, 93, -62}
, {-49, 56, -23}
, {59, 24, 6}
, {72, -62, -37}
, {31, -39, 25}
, {-61, -10, -38}
, {-68, 0, 9}
, {26, 86, -94}
, {58, -85, -80}
, {98, 100, -15}
}
, {{26, -125, -63}
, {68, -12, 80}
, {-78, 21, -121}
, {105, 129, -51}
, {23, 27, 16}
, {52, -113, 85}
, {-114, -116, -31}
, {131, -39, -54}
, {-22, 31, -4}
, {-90, 14, -66}
, {135, 36, 71}
, {63, -31, 76}
, {-39, -35, 39}
, {92, 64, 136}
, {-5, 73, -70}
, {93, -78, -36}
}
, {{18, 0, -88}
, {-61, -18, -108}
, {-25, -6, -39}
, {-21, 40, 88}
, {-5, -71, -35}
, {-1, 31, 42}
, {105, 104, 64}
, {74, -11, 49}
, {88, 13, -70}
, {-13, 48, -71}
, {70, -42, -49}
, {-42, -19, -27}
, {-8, 34, 20}
, {104, -9, -6}
, {89, -92, -36}
, {19, -14, 86}
}
, {{112, 76, 92}
, {-133, -145, -109}
, {119, 174, 188}
, {-61, -23, -61}
, {-94, 75, 63}
, {-81, 12, 109}
, {35, -41, 44}
, {-135, 6, -95}
, {-46, -8, 28}
, {28, 149, 173}
, {-110, 36, -15}
, {88, 3, 57}
, {66, -8, 0}
, {-3, -65, 5}
, {4, 107, 89}
, {38, -27, 13}
}
, {{28, -111, 25}
, {4, -10, -71}
, {-114, 30, -21}
, {26, -56, 35}
, {64, 12, 114}
, {-87, 109, 73}
, {-71, 54, -4}
, {-4, -19, -24}
, {-25, -67, 2}
, {103, -88, 51}
, {23, 13, 107}
, {-107, -75, -27}
, {15, -20, 23}
, {59, -12, 86}
, {43, 74, -104}
, {6, 88, 34}
}
, {{62, -57, 37}
, {56, 39, -108}
, {120, 136, 116}
, {-53, 95, 47}
, {57, 25, -55}
, {-56, 83, 35}
, {52, 44, 39}
, {-72, -1, -83}
, {-47, 73, 50}
, {0, 83, 131}
, {-6, -103, -85}
, {-45, -4, 83}
, {-4, 73, 115}
, {-47, -1, 11}
, {-7, 62, -62}
, {-77, -12, 6}
}
, {{-21, -10, 30}
, {98, -38, -55}
, {-2, -114, -92}
, {-88, 82, 80}
, {-45, -5, -90}
, {59, -55, -26}
, {8, 95, -46}
, {-13, 76, -67}
, {-11, -59, 81}
, {56, 40, -81}
, {41, 0, 26}
, {48, -53, 43}
, {-1, 69, 80}
, {65, 93, 103}
, {-100, -84, 51}
, {104, 7, -21}
}
, {{129, 45, 81}
, {25, 34, 68}
, {54, -99, 44}
, {-10, -22, -70}
, {71, 66, 115}
, {-49, -32, 46}
, {-12, 113, -13}
, {36, 10, 43}
, {-4, -38, -91}
, {-25, -88, -47}
, {28, -73, -61}
, {-2, 18, 12}
, {-64, 58, 2}
, {86, -70, 26}
, {84, 25, -84}
, {-85, 82, 14}
}
, {{-62, 61, 23}
, {119, -65, 125}
, {-67, -49, 31}
, {-104, -73, -57}
, {7, -22, -53}
, {84, 83, 90}
, {-30, -21, 53}
, {79, 54, 127}
, {-2, 19, -71}
, {2, -58, -13}
, {-128, -108, -91}
, {83, 80, 114}
, {73, 46, 50}
, {3, -7, 6}
, {-27, 18, 57}
, {-100, -117, -65}
}
, {{75, 5, -5}
, {35, 30, -2}
, {-19, -21, -118}
, {1, -72, -47}
, {47, -75, 17}
, {-83, 42, -61}
, {-62, -19, 62}
, {-7, -81, 54}
, {53, 18, 36}
, {-101, -122, 38}
, {108, -15, 84}
, {0, -68, 1}
, {-17, 117, 28}
, {-26, 27, 15}
, {-49, 57, -72}
, {87, 22, -86}
}
, {{-9, -57, -9}
, {-127, -107, -28}
, {136, 156, 184}
, {47, 0, -75}
, {-29, -72, 44}
, {-97, 65, 54}
, {42, -38, 121}
, {-53, -148, -86}
, {-88, -51, 119}
, {101, 88, 90}
, {-31, -27, -24}
, {-22, -66, -50}
, {20, -90, -24}
, {-64, 91, 58}
, {19, -87, 97}
, {0, 0, -20}
}
, {{72, -8, 8}
, {-84, 11, 0}
, {-88, 61, 50}
, {-40, -59, 92}
, {68, 83, 33}
, {53, 118, -79}
, {72, 22, -93}
, {-3, 74, 41}
, {-3, 108, -89}
, {-45, 58, -14}
, {-2, -22, -81}
, {-64, 61, 1}
, {-24, -38, 52}
, {8, 71, 21}
, {-73, -19, 49}
, {-92, -39, -66}
}
, {{-81, 92, 17}
, {40, -80, 52}
, {26, -44, -4}
, {-27, 36, -96}
, {65, -33, -5}
, {-74, 7, 43}
, {56, 64, -60}
, {15, -18, 19}
, {-35, -62, -35}
, {-108, -41, 36}
, {-96, 46, -45}
, {-88, -75, 78}
, {34, 54, -77}
, {-107, -93, -90}
, {38, 8, -65}
, {59, -41, -82}
}
, {{-63, 64, -130}
, {47, -71, 37}
, {-50, -136, -101}
, {-58, 40, 68}
, {91, 32, -10}
, {16, 41, -97}
, {32, 71, 6}
, {-79, 14, -87}
, {-16, -31, 48}
, {-41, 68, -94}
, {-42, 19, 92}
, {59, 20, 67}
, {84, -61, -57}
, {-51, 1, 50}
, {-2, -76, -47}
, {122, 37, 88}
}
, {{61, 114, 91}
, {79, 82, -62}
, {-104, 13, -63}
, {59, -50, -9}
, {0, -27, 20}
, {69, 19, 27}
, {17, 0, -16}
, {-11, 116, 113}
, {-29, -2, -75}
, {42, -9, 12}
, {7, -44, -77}
, {-10, 104, -40}
, {-83, 42, -80}
, {-28, 88, 62}
, {18, -80, -18}
, {-98, -115, -86}
}
, {{-47, -88, 3}
, {2, -53, -64}
, {141, 16, 53}
, {-56, 3, 64}
, {-36, -123, 55}
, {-119, -57, 87}
, {46, 69, -26}
, {-15, -68, -105}
, {32, 90, 102}
, {126, -84, -6}
, {-130, 53, -63}
, {-24, 45, -64}
, {42, -51, 79}
, {-58, 56, -83}
, {32, -66, 91}
, {-13, -110, -50}
}
, {{3, 57, -28}
, {30, -19, -47}
, {99, -80, -48}
, {-65, -28, 69}
, {46, -71, -58}
, {-73, -27, -1}
, {-10, -10, -85}
, {99, -86, 77}
, {-5, -89, -1}
, {-78, 28, -44}
, {61, -21, -87}
, {-13, -36, -20}
, {-91, 98, -44}
, {-11, -10, 81}
, {-51, -26, 22}
, {-26, -33, -82}
}
, {{-84, 24, 79}
, {-29, -14, 62}
, {70, 124, -11}
, {-13, 62, -83}
, {-56, -12, 24}
, {-78, -34, -65}
, {49, 34, 38}
, {53, -17, 62}
, {71, 50, 91}
, {91, 116, -78}
, {48, -50, 47}
, {41, 65, 38}
, {-54, 91, -20}
, {-60, -22, 10}
, {-71, -107, -9}
, {4, -90, -15}
}
, {{-67, 29, 70}
, {18, -93, 97}
, {-49, -26, -152}
, {49, -32, 73}
, {79, 26, 18}
, {-30, -94, -2}
, {-77, 84, -85}
, {11, 115, 10}
, {-66, -17, 30}
, {-34, 5, -121}
, {-6, 37, -43}
, {-132, 76, -6}
, {-1, 48, 33}
, {87, -46, 82}
, {25, -25, 19}
, {1, 75, -72}
}
, {{85, 140, 24}
, {-117, -6, -26}
, {25, 9, 184}
, {-37, 19, 68}
, {85, -10, 96}
, {8, -28, 30}
, {52, 50, 28}
, {-42, -45, -41}
, {-98, -25, 106}
, {89, 87, 112}
, {61, -127, -79}
, {-1, 56, -29}
, {38, 8, 58}
, {-112, -74, 85}
, {-94, -10, 44}
, {74, 66, -39}
}
, {{-24, -74, 110}
, {-2, 123, 13}
, {28, 34, 86}
, {-28, -30, 110}
, {-81, -110, 68}
, {-12, -125, -71}
, {-64, 63, 4}
, {77, 17, 15}
, {56, -38, 57}
, {127, -16, -14}
, {-22, -62, 8}
, {62, 72, 6}
, {86, -85, -62}
, {-67, -70, -8}
, {67, 106, 136}
, {-112, -83, 139}
}
, {{35, 2, -109}
, {-54, 77, -62}
, {22, -78, 37}
, {-92, -16, 70}
, {-84, -61, -79}
, {109, -3, -112}
, {-64, -8, 10}
, {33, 2, 19}
, {14, -33, -93}
, {-108, 39, -10}
, {27, -9, 25}
, {100, -44, -54}
, {84, -21, 0}
, {11, -62, -59}
, {-74, 78, -22}
, {-11, 92, 12}
}
, {{51, 22, 28}
, {-50, 121, 76}
, {-51, -79, -17}
, {5, -122, -30}
, {-38, 89, -60}
, {-9, 31, -7}
, {44, 77, -2}
, {108, 32, 41}
, {-53, -2, -25}
, {44, 28, -37}
, {77, -7, 51}
, {69, -4, -7}
, {99, 4, 60}
, {-40, -17, 31}
, {-100, -46, 15}
, {-59, 0, 82}
}
, {{26, 85, -85}
, {67, 31, -88}
, {-42, -35, 113}
, {35, -84, 16}
, {-123, 80, -5}
, {31, -49, 92}
, {-6, 101, 91}
, {-91, 56, 8}
, {118, 39, -34}
, {-65, -80, -69}
, {-124, -27, 74}
, {22, 77, 78}
, {4, 72, 64}
, {-40, -84, -19}
, {-43, 62, -93}
, {-74, 76, -11}
}
, {{-58, 19, -108}
, {48, 107, 8}
, {-99, -109, -27}
, {-75, 21, 5}
, {-7, 10, 86}
, {13, 45, 16}
, {-36, 59, -76}
, {-69, 34, 70}
, {91, -86, 81}
, {-137, 5, -92}
, {56, 109, 3}
, {105, 0, 31}
, {62, -90, -66}
, {-22, -30, 98}
, {88, -44, 88}
, {110, 62, -55}
}
, {{30, 32, 120}
, {62, 92, -29}
, {57, -97, -120}
, {30, -47, 81}
, {40, -35, 96}
, {74, 46, 124}
, {98, 19, -69}
, {106, 6, -42}
, {90, -100, 30}
, {22, -31, -108}
, {58, -53, 39}
, {-58, -67, 52}
, {55, 94, -22}
, {-50, 31, -3}
, {71, -22, -102}
, {-73, -2, -82}
}
, {{-26, -66, 34}
, {25, 21, 88}
, {-157, 7, -93}
, {-62, 54, 89}
, {-30, 115, -11}
, {-88, -45, -81}
, {-40, -89, 37}
, {27, 27, 88}
, {-26, 33, -14}
, {-112, -28, 25}
, {-7, 73, 24}
, {-74, 82, 88}
, {26, 5, 92}
, {101, 2, 61}
, {88, -56, -11}
, {121, 67, 17}
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
#define INPUT_SAMPLES   45
#define POOL_SIZE       4
#define POOL_STRIDE     4
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_63_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_63(
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

#define INPUT_CHANNELS  32
#define INPUT_SAMPLES   11
#define POOL_SIZE       8
#define POOL_STRIDE     8
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t average_pooling1d_15_output_type[INPUT_CHANNELS][POOL_LENGTH];

void average_pooling1d_15(
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

#define INPUT_DIM [1][32]
#define OUTPUT_DIM 32

//typedef number_t *flatten_15_output_type;
typedef number_t flatten_15_output_type[OUTPUT_DIM];

#define flatten_15 //noop (IN, OUT)  OUT = (number_t*)IN

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

#define INPUT_SAMPLES 32
#define FC_UNITS 3
#define ACTIVATION_LINEAR

typedef number_t dense_15_output_type[FC_UNITS];

static inline void dense_15(
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
  //dense_15_output_type dense_15_output);
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
