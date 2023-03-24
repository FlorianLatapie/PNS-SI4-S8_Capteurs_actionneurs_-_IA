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
  * @file    conv.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_CHANNELS      9
#define INPUT_SAMPLES       128
#define CONV_FILTERS        8
#define CONV_KERNEL_SIZE    3
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_3_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_3(
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

#define INPUT_CHANNELS    9
#define CONV_FILTERS      8
#define CONV_KERNEL_SIZE  3


const int16_t conv1d_3_bias[CONV_FILTERS] = {-330, -2, 5, -419, -134, -178, -217, -40}
;

const int16_t conv1d_3_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{155, 125, 3}
, {215, 116, 100}
, {74, 62, -236}
, {62, -162, -128}
, {-13, -136, -70}
, {138, 159, -260}
, {164, 191, -306}
, {351, 171, -271}
, {33, 133, -466}
}
, {{-247, 268, 100}
, {126, -46, 200}
, {139, 158, 9}
, {-144, -16, 153}
, {148, -95, -75}
, {16, 132, 238}
, {-38, 76, 111}
, {215, 160, 66}
, {130, 234, 138}
}
, {{219, -187, 38}
, {135, -212, 17}
, {30, -102, -7}
, {-53, -374, 119}
, {19, 41, 23}
, {-67, 56, 180}
, {25, 179, -151}
, {-63, 117, -122}
, {-249, 160, -328}
}
, {{-140, 245, -272}
, {-294, 68, -253}
, {32, 189, -280}
, {-111, 217, -205}
, {-215, 13, -202}
, {7, -60, -40}
, {69, 156, -105}
, {170, -208, 231}
, {277, -42, 34}
}
, {{-126, -285, -15}
, {36, 52, -222}
, {-164, -68, -58}
, {81, -66, 150}
, {-27, 143, 196}
, {-193, -111, 1}
, {-184, 15, -44}
, {-215, -29, 31}
, {-116, -135, -198}
}
, {{172, -192, 114}
, {137, -258, 159}
, {36, -244, 76}
, {128, -117, 150}
, {-46, -138, 1}
, {60, -220, 82}
, {-175, 3, 28}
, {-69, 18, -202}
, {2, 115, -422}
}
, {{-177, -163, -256}
, {-146, -288, -185}
, {-90, -88, -132}
, {-65, -212, -211}
, {-163, -436, -250}
, {-65, -404, -345}
, {-89, -172, -85}
, {-70, -206, -36}
, {-246, -164, -176}
}
, {{-171, -41, 117}
, {2, -56, 124}
, {-55, 56, -210}
, {-142, 290, -103}
, {-99, 57, -260}
, {-21, 331, -194}
, {-213, 215, -275}
, {-330, 398, -151}
, {-24, 227, -291}
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
#define INPUT_SAMPLES   126
#define POOL_SIZE       2
#define POOL_STRIDE     2
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_2_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_2(
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
#define INPUT_SAMPLES       63
#define CONV_FILTERS        8
#define CONV_KERNEL_SIZE    5
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_4_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_4(
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
#define CONV_FILTERS      8
#define CONV_KERNEL_SIZE  5


const int16_t conv1d_4_bias[CONV_FILTERS] = {43, 388, -44, -38, 27, 168, -20, 223}
;

const int16_t conv1d_4_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{39, -32, -408, -400, -310}
, {128, -49, -110, -413, -251}
, {27, -33, -20, 99, -87}
, {-214, 129, -118, 445, -142}
, {-147, -45, -119, -36, -18}
, {34, -191, -128, -82, 190}
, {-414, -181, 199, 234, 151}
, {142, -56, 18, 354, -135}
}
, {{205, -385, -615, 102, -288}
, {-204, -527, -338, -135, -98}
, {-297, 91, -315, -239, 414}
, {-205, 93, 201, -7, -31}
, {-66, 151, -145, -55, 210}
, {-64, 368, -342, -53, 235}
, {-288, -52, -298, -206, -153}
, {-202, 88, -269, 152, 220}
}
, {{76, 40, 101, 55, -280}
, {24, -18, -252, -173, 116}
, {70, -56, -212, -76, 29}
, {146, -32, -338, 44, -119}
, {17, 33, 161, 174, 56}
, {-101, -153, -185, -108, -8}
, {166, 199, 326, 200, -75}
, {-182, 95, -500, 117, 181}
}
, {{-189, -269, -365, -360, -135}
, {159, -334, -130, 123, 40}
, {-146, 70, 186, 106, 2}
, {-56, 217, 71, 224, 30}
, {-211, -29, -23, 17, -47}
, {101, -17, 85, 71, -48}
, {-92, -29, 90, 116, -50}
, {337, -87, 43, -46, -193}
}
, {{94, -104, -179, 152, -23}
, {-51, 97, 161, 173, 32}
, {-231, -49, 22, -39, 108}
, {58, -140, -44, -128, -26}
, {-23, 45, 27, -65, -38}
, {-73, 0, 33, 11, -214}
, {-174, 323, 117, -180, -469}
, {30, -120, -23, -289, -27}
}
, {{-14, -123, -535, -86, -310}
, {90, 45, -100, -44, 49}
, {159, 160, -272, 80, 64}
, {-213, -132, 60, -242, -4}
, {195, 134, -442, -549, -335}
, {177, 90, -330, 209, 60}
, {40, -156, -516, -639, -551}
, {-49, 115, 13, -126, 261}
}
, {{-38, -73, -159, -23, -146}
, {48, -95, -34, -143, -94}
, {94, 9, 111, 89, -15}
, {451, 129, 10, 108, -55}
, {-237, -73, -27, -13, 65}
, {130, 105, 180, 0, 119}
, {-47, -46, -271, -15, -225}
, {351, 194, 200, 230, 100}
}
, {{87, -56, 346, 285, -76}
, {-441, -176, 57, -65, 332}
, {-414, -338, -71, -173, -4}
, {-677, 197, 207, -163, 50}
, {165, -58, -189, 177, -5}
, {-241, -97, -34, -21, 91}
, {234, -385, 48, 207, -209}
, {-549, -336, 13, -72, 153}
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
#define INPUT_SAMPLES   59
#define POOL_SIZE       2
#define POOL_STRIDE     2
#define POOL_PAD        0 // Unsupported
#define POOL_LENGTH	    ( ( (INPUT_SAMPLES - POOL_SIZE + (2*POOL_PAD) ) / POOL_STRIDE ) + 1 )

#define ACTIVATION_LINEAR

typedef number_t max_pooling1d_3_output_type[INPUT_CHANNELS][POOL_LENGTH];

static inline void max_pooling1d_3(
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
#define INPUT_SAMPLES       29
#define CONV_FILTERS        8
#define CONV_KERNEL_SIZE    7
#define CONV_STRIDE         1

#define ZEROPADDING_LEFT    0
#define ZEROPADDING_RIGHT   0

#define CONV_OUTSAMPLES     ( ( (INPUT_SAMPLES - CONV_KERNEL_SIZE + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE ) + 1 )

#define ACTIVATION_RELU

typedef number_t conv1d_5_output_type[CONV_FILTERS][CONV_OUTSAMPLES];

static inline void conv1d_5(
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
#define CONV_FILTERS      8
#define CONV_KERNEL_SIZE  7


const int16_t conv1d_5_bias[CONV_FILTERS] = {-69, -99, 206, 24, 72, -47, -177, -5}
;

const int16_t conv1d_5_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{226, -1, -367, -307, 48, -128, -280}
, {-508, -600, -204, -168, -228, 127, 234}
, {-62, -9, -173, 57, -27, -418, -454}
, {74, -104, -2, -131, -98, -78, 61}
, {-22, 66, -13, 192, 241, 260, 187}
, {-89, -110, 13, 9, 222, 97, 317}
, {122, 95, 59, -219, 4, -81, 150}
, {20, -297, -128, -221, 38, -140, -167}
}
, {{-26, 103, -347, -312, 169, 89, 125}
, {-70, 24, -9, 260, 209, -121, 7}
, {6, 206, -163, -511, -111, 87, 280}
, {-21, -61, -37, -77, 11, -15, 42}
, {146, 77, 47, -105, -57, -103, -228}
, {-166, -6, 135, 30, -164, -630, -244}
, {-3, -17, 159, -94, -174, 5, 102}
, {84, 41, -270, -338, -31, -257, -444}
}
, {{-415, -363, -145, 197, 9, 205, -90}
, {536, 456, 99, -86, 26, -588, -8}
, {-297, -38, -131, -157, -98, -417, -60}
, {-359, -277, -139, 95, 247, 116, 185}
, {-120, -313, -54, 72, 59, 74, 169}
, {-250, -96, -110, 87, 76, 53, 214}
, {-317, -314, -62, -121, 63, -26, 114}
, {395, 303, 98, -158, -444, -244, -109}
}
, {{-102, -87, -351, -84, -102, 199, 89}
, {-37, 39, -201, -82, -242, -293, -186}
, {145, 59, -79, -87, 104, 164, 220}
, {-154, -105, -28, -26, -30, 0, 62}
, {-182, -80, 73, 12, 117, 38, 269}
, {-95, 25, -59, -257, -104, 11, 169}
, {-251, -54, -22, -18, -52, -124, -135}
, {-118, 76, 20, 1, -78, -24, 175}
}
, {{17, -34, -37, 93, -226, -367, 116}
, {74, 50, -144, 17, 4, -462, -480}
, {-456, -191, -239, -232, -77, -237, -292}
, {114, 32, -206, -46, 267, -54, 76}
, {-95, -187, -144, -163, 15, 232, -28}
, {74, -66, 201, -33, 48, -5, -23}
, {-180, -335, -220, -291, -112, -358, -246}
, {-119, -246, -378, -22, 335, 116, -436}
}
, {{-33, -228, 8, -183, 127, -9, 21}
, {-466, -221, -163, 294, -10, 0, -261}
, {22, -4, -113, -203, -169, 22, 25}
, {188, 44, 40, 100, 150, 32, 8}
, {-171, -88, -218, 95, 43, 5, 73}
, {-221, -156, -208, -59, 34, 9, -225}
, {374, 251, 92, 154, -127, -96, -196}
, {-283, -159, -91, -407, -18, -149, -2}
}
, {{119, 73, 78, 55, -86, 129, -76}
, {-496, -185, 405, 273, 193, -24, 199}
, {81, 41, -122, -37, -186, -238, -99}
, {61, -50, 15, -3, -207, -39, -24}
, {79, -141, -34, -144, -259, -250, -308}
, {-377, 6, -179, -51, -158, 106, 350}
, {180, 116, -1, 184, -17, -91, -12}
, {-83, -161, -174, -403, -509, -577, -372}
}
, {{-8, 37, -61, -138, 164, -268, -216}
, {-308, -229, 242, 358, 382, 137, 173}
, {-110, 112, -1, 50, -47, -40, 20}
, {218, -16, 76, -258, -75, 35, -255}
, {5, 59, -122, -45, -131, -100, -225}
, {-190, 180, 66, 222, -87, 15, -540}
, {390, 175, -38, -144, -46, 43, -122}
, {-291, -149, -347, -176, 107, 51, 185}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE
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

#define INPUT_DIM [23][8]
#define OUTPUT_DIM 184

//typedef number_t *flatten_1_output_type;
typedef number_t flatten_1_output_type[OUTPUT_DIM];

#define flatten_1 //noop (IN, OUT)  OUT = (number_t*)IN

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

#define INPUT_SAMPLES 184
#define FC_UNITS 6
#define ACTIVATION_LINEAR

typedef number_t dense_1_output_type[FC_UNITS];

static inline void dense_1(
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

#define INPUT_SAMPLES 184
#define FC_UNITS 6


const int16_t dense_1_bias[FC_UNITS] = {85, -70, -201, 84, 4, 167}
;

const int16_t dense_1_kernel[FC_UNITS][INPUT_SAMPLES] = {{-340, -195, 226, -96, -47, -88, -133, -143, 105, 16, 77, -20, 234, 89, -12, -26, 137, 127, 84, 26, -179, 8, -18, -378, -351, -29, -21, -4, -74, -220, -155, -328, -220, -45, -13, 33, -115, -19, 130, -360, 152, -11, 122, 69, -31, -65, 8, -50, -236, -230, -105, -80, -54, -165, 4, 174, 230, -46, 15, 82, 22, -98, 42, -68, -85, -140, -49, 62, 83, -362, -404, 25, 54, 83, -67, -35, 23, -77, -55, 128, 56, -174, -79, -170, -65, 42, -97, -87, -168, -106, -173, 47, -412, -560, -162, -291, -282, 47, 20, 77, -45, -90, 76, -30, -6, 52, -519, -465, -125, 46, -16, -134, -37, -107, -172, -495, -262, -100, -158, 27, -53, -231, -69, 84, 125, -6, -29, 4, 78, 80, 149, 70, 163, 121, 40, -156, -39, -237, -266, -108, 45, -91, -158, 9, 185, 48, 96, 14, 222, 167, 73, 47, 154, 128, -124, 56, 22, 56, 32, 80, -20, -169, -285, -444, 125, 332, 228, 8, -101, 159, -22, -44, 217, -182, 55, -31, 120, -128, 3, 143, 73, -1, -98, 14}
, {-146, -116, 116, 89, 11, -37, 99, 154, 78, 51, 1, -31, -38, 62, 76, 105, 19, -28, -77, -40, -92, -140, -258, 100, 287, 185, -35, 94, 30, 13, -67, -110, -194, -31, -78, -33, 14, 274, -9, 65, -324, 205, 175, 101, 140, 282, -290, -365, -179, -72, -61, -10, -8, -53, -111, -152, -239, -130, -104, 37, 14, -10, -83, 2, -31, -92, -118, -131, -65, -241, -116, -32, -4, -24, 38, 40, 60, 9, 248, -6, 72, 101, 93, 8, 88, -33, -178, -78, 2, -5, -51, 65, -244, -499, -248, -52, 162, 57, 21, -14, -123, -69, -121, -293, -36, 115, 149, -578, -380, 30, -30, -96, 148, 9, -40, 92, 217, 149, 244, 121, 51, 130, -106, -108, -34, 82, -36, 42, -77, -3, -253, -37, -101, 13, -87, -32, -58, 6, 126, 89, -130, 200, 128, 22, 18, -62, 68, -41, 25, -39, -112, -14, -187, -135, -129, 66, -33, 108, 16, -12, 125, -78, 8, 94, 248, 13, 27, 19, 18, 22, 131, 109, 37, 123, -127, -4, -96, -5, 137, -126, -130, -29, -155, 90}
, {371, 229, -108, 83, 251, 124, 99, -17, -34, -50, 20, 63, 79, 72, 11, -14, -91, 24, 93, -4, 76, 12, -12, 34, -275, -5, 146, 7, 106, 179, 114, 382, 369, 103, -50, 22, -74, -222, -233, -251, 188, -296, -161, -175, -26, -305, -295, -193, 176, 52, 100, 108, 55, 23, 100, 6, -151, 40, 101, -199, -65, -34, -76, -75, -147, -89, -364, -455, -492, -475, -368, -235, -89, 86, 90, 175, 119, 84, -110, -90, -149, -163, -274, -92, -120, -84, -14, -191, -311, -298, -268, -498, 45, -119, -359, -75, 140, 24, -19, -166, -45, -150, 41, -19, -41, -21, -145, -433, -157, 54, -324, -294, -153, -43, -214, 234, 154, 76, -16, -28, 27, 103, -19, -9, -130, -34, 20, -51, 77, 9, -3, 64, -116, -65, -80, -70, 135, 55, 250, 198, 125, 0, 168, -48, -183, -4, -83, -19, 49, -7, 142, 78, 95, 11, 106, 158, 30, 69, 38, -4, -72, 225, 316, 113, -213, -265, -188, -152, 21, -78, 5, 35, 2, -66, 0, -2, -71, 138, -117, -17, -3, -76, 54, 52}
, {-492, -301, -372, -236, -397, -21, -222, -162, -5, 170, -201, -228, -29, -81, -19, -36, 81, 69, -24, 19, 170, 252, 449, -68, 90, -348, -160, 343, -9, -51, -194, 204, -193, 161, 27, -25, -109, -97, -312, -387, -340, 26, -91, -89, 13, 77, 278, 125, -14, -33, -86, -50, -288, -631, -319, 40, 348, 480, 276, -257, 15, -27, 79, 39, 112, -118, -233, -323, -267, -48, 18, -130, -186, -189, -129, -176, -236, -59, -143, 52, 214, 148, 183, 58, 28, 26, 51, 43, -43, -83, -420, 67, -214, -235, -145, 121, -199, -10, -350, -127, -1, 235, 291, 333, 223, -91, 0, 140, 68, 32, -454, -595, 119, 179, -264, -690, -663, -457, -636, -168, -257, -253, -358, -366, -52, -42, 30, -257, -307, -530, -155, -1, 16, -42, 70, -111, -165, -52, -487, -459, -202, 81, -15, 18, 92, -93, 60, -174, -75, -374, -137, -113, -109, -10, -34, 53, 15, 45, -58, 97, -35, -445, -112, -26, -145, 44, 79, 19, -67, 83, -47, -366, -331, -435, -183, 145, -81, -272, -97, -112, -19, -52, 72, 131}
, {-295, -398, -250, -178, -129, -157, -80, 105, -46, -99, 31, -60, -102, -72, -113, 34, 36, 50, -1, 11, -109, -170, -678, -46, 28, 65, 47, -336, -139, -40, -89, -200, -121, -33, 48, 241, -193, -75, -62, -79, -205, -130, 96, 153, -70, -133, 115, 140, 60, 24, 52, 24, 33, 450, 84, 82, -484, -611, -454, -66, -63, 88, 52, 123, 112, 1, -144, -69, -265, 502, 338, 211, 192, 129, 9, -181, -159, -363, -151, -44, -73, -208, 80, 70, -24, 42, 100, 48, 62, -79, 143, -370, 179, 537, 272, -159, 13, -240, 39, 28, 142, -100, -264, -421, -239, 11, -5, 106, 96, 15, -225, -76, 9, 28, -739, -585, -534, -563, -424, -375, -457, -231, -181, -14, -61, 139, -236, -204, -360, -67, -76, -25, -94, 101, 18, -75, -45, -105, -497, -502, -486, -573, -144, -110, -246, 21, -81, 120, 44, 67, -105, -255, -254, -46, -11, -153, -86, 55, 108, 111, -88, -279, -381, -577, -379, 61, -247, -453, -250, -237, -145, 122, 91, 172, -239, 67, 191, -87, -156, 35, -16, -26, -30, 114}
, {-90, 5, 130, 122, 198, 128, 63, 89, -61, -73, 9, 100, 14, -24, -267, -306, -379, -335, 86, 189, 127, 212, 385, 210, -87, -292, -115, 67, 155, 109, -150, 80, 145, 4, -67, 52, -133, 86, 379, 333, 218, 89, 39, -649, -191, -19, 4, 3, -49, 54, -27, 90, 150, 48, 194, -51, -140, -84, 28, 116, 64, -96, -145, -205, -158, 112, 126, 338, 203, 277, 248, 61, -100, 59, 93, 136, 28, 5, -48, -91, -64, -186, -364, -440, -297, -47, 131, 1, 21, 542, 565, 400, -67, 177, 54, -20, 200, -130, 59, -11, -34, -21, -3, -53, -19, 63, -47, -466, -470, -327, 536, 785, 143, -184, 875, -427, -348, -95, -134, -127, -29, -213, 40, -15, 128, 23, 13, 97, -180, -26, -91, -332, -156, -203, -55, 40, 116, 212, -394, -84, -17, 38, 17, -74, -71, -39, 111, -52, -66, 26, -184, 28, 110, 87, -8, -54, -108, -79, -94, -123, -115, -261, -101, -69, -72, -210, -137, -103, -125, 2, 142, -58, 61, -52, -73, -187, -115, -151, -5, -36, -42, -245, -238, -157}
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

#define MODEL_OUTPUT_SAMPLES 6
#define MODEL_INPUT_SAMPLES 128 // node 0 is InputLayer so use its output shape as input shape of the model
#define MODEL_INPUT_CHANNELS 9

void cnn(
  const number_t input[MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES],
  //dense_1_output_type dense_1_output);
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
#include "conv1d_3.c"
#include "weights/conv1d_3.c" // InputLayer is excluded
#include "max_pooling1d_2.c" // InputLayer is excluded
#include "conv1d_4.c"
#include "weights/conv1d_4.c" // InputLayer is excluded
#include "max_pooling1d_3.c" // InputLayer is excluded
#include "conv1d_5.c"
#include "weights/conv1d_5.c" // InputLayer is excluded
#include "flatten_1.c" // InputLayer is excluded
#include "dense_1.c"
#include "weights/dense_1.c"
#endif

void cnn(
  const number_t input[MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES],
  dense_1_output_type dense_1_output) {

  // Output array allocation
  static union {
    conv1d_3_output_type conv1d_3_output;
    conv1d_4_output_type conv1d_4_output;
    conv1d_5_output_type conv1d_5_output;
    flatten_1_output_type flatten_1_output;
  } activations1;

  static union {
    max_pooling1d_2_output_type max_pooling1d_2_output;
    max_pooling1d_3_output_type max_pooling1d_3_output;
  } activations2;


  //static union {
//
//    static input_2_output_type input_2_output;
//
//    static conv1d_3_output_type conv1d_3_output;
//
//    static max_pooling1d_2_output_type max_pooling1d_2_output;
//
//    static conv1d_4_output_type conv1d_4_output;
//
//    static max_pooling1d_3_output_type max_pooling1d_3_output;
//
//    static conv1d_5_output_type conv1d_5_output;
//
//    static flatten_1_output_type flatten_1_output;
//
  //} activations;

  // Model layers call chain
 // InputLayer is excluded 
  conv1d_3(
     // First layer uses input passed as model parameter
    input,
    conv1d_3_kernel,
    conv1d_3_bias,
    activations1.conv1d_3_output
  );
 // InputLayer is excluded 
  max_pooling1d_2(
    
    activations1.conv1d_3_output,
    activations2.max_pooling1d_2_output
  );
 // InputLayer is excluded 
  conv1d_4(
    
    activations2.max_pooling1d_2_output,
    conv1d_4_kernel,
    conv1d_4_bias,
    activations1.conv1d_4_output
  );
 // InputLayer is excluded 
  max_pooling1d_3(
    
    activations1.conv1d_4_output,
    activations2.max_pooling1d_3_output
  );
 // InputLayer is excluded 
  conv1d_5(
    
    activations2.max_pooling1d_3_output,
    conv1d_5_kernel,
    conv1d_5_bias,
    activations1.conv1d_5_output
  );
 // InputLayer is excluded 
  flatten_1(
    
    activations1.conv1d_5_output,
    activations1.flatten_1_output
  );
 // InputLayer is excluded 
  dense_1(
    
    activations1.flatten_1_output,
    dense_1_kernel,
    dense_1_bias, // Last layer uses output passed as model parameter
    dense_1_output
  );

}
