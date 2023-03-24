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

#define FIXED_POINT	0	// Fixed point scaling factor, set to 0 when using floating point
#define NUMBER_MIN	-inf	// Max value for this numeric type
#define NUMBER_MAX	inf	// Min value for this numeric type
typedef float number_t;		// Standard size numeric type used for weights and activations
typedef float long_number_t;	// Long numeric type used for intermediate results

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


const float conv1d_3_bias[CONV_FILTERS] = {-0x1.4a59620000000p-1, -0x1.5e557a0000000p-8, 0x1.76f4440000000p-7, -0x1.a3aea40000000p-1, -0x1.0cd6900000000p-2, -0x1.6461140000000p-2, -0x1.b35e700000000p-2, -0x1.40f2d80000000p-4}
;

const float conv1d_3_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{0x1.36f4500000000p-2, 0x1.f484660000000p-3, 0x1.cebd7a0000000p-8}
, {0x1.af119e0000000p-2, 0x1.d1062a0000000p-3, 0x1.919e260000000p-3}
, {0x1.2918700000000p-3, 0x1.f52f8a0000000p-4, -0x1.d89e040000000p-2}
, {0x1.f1df480000000p-4, -0x1.442e400000000p-2, -0x1.0072ce0000000p-2}
, {-0x1.ab18f00000000p-6, -0x1.11c48c0000000p-2, -0x1.18bbd60000000p-3}
, {0x1.15538e0000000p-2, 0x1.3f8b9a0000000p-2, -0x1.04cba00000000p-1}
, {0x1.4919900000000p-2, 0x1.7e53940000000p-2, -0x1.3283860000000p-1}
, {0x1.5f8f0c0000000p-1, 0x1.5747f20000000p-2, -0x1.0f49d20000000p-1}
, {0x1.08995c0000000p-4, 0x1.0b983c0000000p-2, -0x1.d2e8600000000p-1}
}
, {{-0x1.ee4a960000000p-2, 0x1.0c6b480000000p-1, 0x1.90f73e0000000p-3}
, {0x1.fa6f380000000p-3, -0x1.7794480000000p-4, 0x1.91c0420000000p-2}
, {0x1.170cb20000000p-2, 0x1.3dfc6e0000000p-2, 0x1.2432c80000000p-6}
, {-0x1.209ab00000000p-2, -0x1.0760220000000p-5, 0x1.333e280000000p-2}
, {0x1.28c09a0000000p-2, -0x1.7cb0c80000000p-3, -0x1.2c2e640000000p-3}
, {0x1.024b120000000p-5, 0x1.08dd000000000p-2, 0x1.dca0b00000000p-2}
, {-0x1.331cc00000000p-4, 0x1.31e88c0000000p-3, 0x1.bdd6cc0000000p-3}
, {0x1.ae4df40000000p-2, 0x1.4026200000000p-2, 0x1.0bb4700000000p-3}
, {0x1.04cef20000000p-2, 0x1.d4e2cc0000000p-2, 0x1.1508aa0000000p-2}
}
, {{0x1.b716a40000000p-2, -0x1.7642100000000p-2, 0x1.32212c0000000p-4}
, {0x1.0e897a0000000p-2, -0x1.a864440000000p-2, 0x1.1809980000000p-5}
, {0x1.e9bfaa0000000p-5, -0x1.9b1e940000000p-3, -0x1.d236900000000p-7}
, {-0x1.a97bee0000000p-4, -0x1.7687040000000p-1, 0x1.dfe4d80000000p-3}
, {0x1.39c90c0000000p-5, 0x1.4b71920000000p-4, 0x1.7d7b4e0000000p-5}
, {-0x1.0cca5a0000000p-3, 0x1.c2aa2a0000000p-4, 0x1.68f52c0000000p-2}
, {0x1.96888c0000000p-5, 0x1.667c380000000p-2, -0x1.2fcc2a0000000p-2}
, {-0x1.fb31c20000000p-4, 0x1.d651b80000000p-3, -0x1.eb38380000000p-3}
, {-0x1.f3076c0000000p-2, 0x1.410f6a0000000p-2, -0x1.48bfa60000000p-1}
}
, {{-0x1.185fe60000000p-2, 0x1.eba2a60000000p-2, -0x1.10339e0000000p-1}
, {-0x1.26f09c0000000p-1, 0x1.134c000000000p-3, -0x1.fbc4900000000p-2}
, {0x1.01e73c0000000p-4, 0x1.7a2a400000000p-2, -0x1.1814100000000p-1}
, {-0x1.be39440000000p-3, 0x1.b37c640000000p-2, -0x1.9b91760000000p-2}
, {-0x1.aed24a0000000p-2, 0x1.a2e8ac0000000p-6, -0x1.9580840000000p-2}
, {0x1.fd8cc40000000p-7, -0x1.e7104c0000000p-4, -0x1.400c080000000p-4}
, {0x1.1756940000000p-3, 0x1.392b720000000p-2, -0x1.a4947a0000000p-3}
, {0x1.5471260000000p-2, -0x1.a1d7f60000000p-2, 0x1.ce8db40000000p-2}
, {0x1.1506720000000p-1, -0x1.5364dc0000000p-4, 0x1.12514a0000000p-4}
}
, {{-0x1.fad8560000000p-3, -0x1.1d7bfa0000000p-1, -0x1.eb6f780000000p-6}
, {0x1.25b1180000000p-4, 0x1.a273be0000000p-4, -0x1.bcb9080000000p-2}
, {-0x1.48ee7c0000000p-2, -0x1.1350c60000000p-3, -0x1.d221d80000000p-4}
, {0x1.47c2540000000p-3, -0x1.084f060000000p-3, 0x1.2d64a40000000p-2}
, {-0x1.b59e360000000p-5, 0x1.1f378c0000000p-2, 0x1.88e9fa0000000p-2}
, {-0x1.83e5c80000000p-2, -0x1.bce9180000000p-3, 0x1.fbb7d00000000p-9}
, {-0x1.70127a0000000p-2, 0x1.e6b2e20000000p-6, -0x1.61ddc80000000p-4}
, {-0x1.af3aaa0000000p-2, -0x1.dd98760000000p-5, 0x1.f43e9e0000000p-5}
, {-0x1.d234660000000p-3, -0x1.0ee7240000000p-2, -0x1.8d4ea00000000p-2}
}
, {{0x1.58a5f80000000p-2, -0x1.814ad40000000p-2, 0x1.cba5720000000p-3}
, {0x1.13e7740000000p-2, -0x1.021dc20000000p-1, 0x1.3f40760000000p-2}
, {0x1.23e4480000000p-4, -0x1.e89ebc0000000p-2, 0x1.3260d80000000p-3}
, {0x1.01bb940000000p-2, -0x1.d4448a0000000p-3, 0x1.2c59800000000p-2}
, {-0x1.716ec20000000p-4, -0x1.15eec00000000p-2, 0x1.b6ea880000000p-9}
, {0x1.e4b3a40000000p-4, -0x1.b93c740000000p-2, 0x1.4b79c40000000p-3}
, {-0x1.5e34060000000p-2, 0x1.98a5320000000p-8, 0x1.c5d8760000000p-5}
, {-0x1.15aa7e0000000p-3, 0x1.2b2fe00000000p-5, -0x1.9440200000000p-2}
, {0x1.6170040000000p-8, 0x1.ce046a0000000p-3, -0x1.a6f9de0000000p-1}
}
, {{-0x1.6386b60000000p-2, -0x1.47e3840000000p-2, -0x1.004d200000000p-1}
, {-0x1.257ff60000000p-2, -0x1.20104a0000000p-1, -0x1.73c9be0000000p-2}
, {-0x1.6af84e0000000p-3, -0x1.6084480000000p-3, -0x1.096cce0000000p-2}
, {-0x1.06a4a80000000p-3, -0x1.a89ea40000000p-2, -0x1.a6c3280000000p-2}
, {-0x1.473f9c0000000p-2, -0x1.b4fb780000000p-1, -0x1.f527600000000p-2}
, {-0x1.04da800000000p-3, -0x1.94eed20000000p-1, -0x1.596e560000000p-1}
, {-0x1.6542520000000p-3, -0x1.59d5e00000000p-2, -0x1.5603c60000000p-3}
, {-0x1.1b96400000000p-3, -0x1.9d58100000000p-2, -0x1.214ae20000000p-4}
, {-0x1.ed157e0000000p-2, -0x1.4890da0000000p-2, -0x1.6142420000000p-2}
}
, {{-0x1.57981c0000000p-2, -0x1.4bf2360000000p-4, 0x1.d662a60000000p-3}
, {0x1.5a33160000000p-8, -0x1.c7e4ce0000000p-4, 0x1.f179440000000p-3}
, {-0x1.ba5a7a0000000p-4, 0x1.c16c760000000p-4, -0x1.a41d160000000p-2}
, {-0x1.1c0c6a0000000p-2, 0x1.2254ec0000000p-1, -0x1.9e6ff60000000p-3}
, {-0x1.8d0a4a0000000p-3, 0x1.c8e0160000000p-4, -0x1.04dc420000000p-1}
, {-0x1.5fbd840000000p-5, 0x1.4b403a0000000p-1, -0x1.8548d00000000p-2}
, {-0x1.aa01820000000p-2, 0x1.af7fe00000000p-2, -0x1.13fa5e0000000p-1}
, {-0x1.4a6c580000000p-1, 0x1.8ea5040000000p-1, -0x1.2f04660000000p-2}
, {-0x1.8c58c20000000p-5, 0x1.c645740000000p-2, -0x1.23a0e40000000p-1}
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


const float conv1d_4_bias[CONV_FILTERS] = {0x1.5af3d60000000p-4, 0x1.8408c40000000p-1, -0x1.61e8720000000p-4, -0x1.37b19c0000000p-4, 0x1.b6f7500000000p-5, 0x1.51b8a60000000p-2, -0x1.4a7ffc0000000p-5, 0x1.be092c0000000p-2}
;

const float conv1d_4_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{0x1.3ae5c00000000p-4, -0x1.041a5c0000000p-4, -0x1.984c240000000p-1, -0x1.90db500000000p-1, -0x1.36c53c0000000p-1}
, {0x1.006c4c0000000p-2, -0x1.8c39a00000000p-4, -0x1.baf6880000000p-3, -0x1.9d7f300000000p-1, -0x1.f769ac0000000p-2}
, {0x1.bfb7900000000p-5, -0x1.0df1580000000p-4, -0x1.4d25840000000p-5, 0x1.8f0bb40000000p-3, -0x1.5cb9a60000000p-3}
, {-0x1.aca1980000000p-2, 0x1.0200f20000000p-2, -0x1.d895420000000p-3, 0x1.bdf2b60000000p-1, -0x1.1d689a0000000p-2}
, {-0x1.27d2be0000000p-2, -0x1.6873b40000000p-4, -0x1.df1b620000000p-3, -0x1.244bc60000000p-4, -0x1.2a5c2a0000000p-5}
, {0x1.1111fc0000000p-4, -0x1.7f47b80000000p-2, -0x1.017bc40000000p-2, -0x1.48e3000000000p-3, 0x1.7db6600000000p-2}
, {-0x1.9ee1d40000000p-1, -0x1.6ad7060000000p-2, 0x1.8eb7000000000p-2, 0x1.d530c00000000p-2, 0x1.2e949a0000000p-2}
, {0x1.1db0ba0000000p-2, -0x1.c4d41c0000000p-4, 0x1.2e944a0000000p-5, 0x1.6272980000000p-1, -0x1.0f3fde0000000p-2}
}
, {{0x1.9ad99c0000000p-2, -0x1.8127dc0000000p-1, -0x1.33c8720000000p+0, 0x1.99a46e0000000p-3, -0x1.2013dc0000000p-1}
, {-0x1.99a7be0000000p-2, -0x1.07b1cc0000000p+0, -0x1.520fba0000000p-1, -0x1.0e67980000000p-2, -0x1.8a8f120000000p-3}
, {-0x1.29d41e0000000p-1, 0x1.6d14d80000000p-3, -0x1.3b2d500000000p-1, -0x1.df00900000000p-2, 0x1.9e87e40000000p-1}
, {-0x1.9b7e3a0000000p-2, 0x1.7460100000000p-3, 0x1.92cec80000000p-2, -0x1.e89bce0000000p-7, -0x1.f778920000000p-5}
, {-0x1.085d5a0000000p-3, 0x1.2fc9d60000000p-2, -0x1.2262340000000p-2, -0x1.bda9620000000p-4, 0x1.a4f8ee0000000p-2}
, {-0x1.004b660000000p-3, 0x1.705c5a0000000p-1, -0x1.56b2dc0000000p-1, -0x1.ad53e00000000p-4, 0x1.d7f71a0000000p-2}
, {-0x1.20456a0000000p-1, -0x1.a0c52e0000000p-4, -0x1.2acf7c0000000p-1, -0x1.9d5c0c0000000p-2, -0x1.328d600000000p-2}
, {-0x1.9477e00000000p-2, 0x1.61de900000000p-3, -0x1.0dbf5c0000000p-1, 0x1.312a2c0000000p-2, 0x1.b9b4640000000p-2}
}
, {{0x1.32b7280000000p-3, 0x1.4361320000000p-4, 0x1.95ef640000000p-3, 0x1.bc46680000000p-4, -0x1.18a8500000000p-1}
, {0x1.8082200000000p-5, -0x1.27c1180000000p-5, -0x1.f84de00000000p-2, -0x1.5bf9540000000p-2, 0x1.d212320000000p-3}
, {0x1.196b1c0000000p-3, -0x1.c4fec40000000p-4, -0x1.a8cd8c0000000p-2, -0x1.3108ce0000000p-3, 0x1.d06d7c0000000p-5}
, {0x1.25fdca0000000p-2, -0x1.03272c0000000p-4, -0x1.526cd20000000p-1, 0x1.668b8a0000000p-4, -0x1.dedff80000000p-3}
, {0x1.10f8520000000p-5, 0x1.0d32660000000p-4, 0x1.4273e80000000p-2, 0x1.5cfd240000000p-2, 0x1.c587fc0000000p-4}
, {-0x1.957b140000000p-3, -0x1.3392100000000p-2, -0x1.73e10a0000000p-2, -0x1.b3eafa0000000p-3, -0x1.014cb20000000p-6}
, {0x1.4c80c00000000p-2, 0x1.8f56500000000p-2, 0x1.468a9e0000000p-1, 0x1.914c5a0000000p-2, -0x1.2e32f60000000p-3}
, {-0x1.6d75fe0000000p-2, 0x1.7f3a6a0000000p-3, -0x1.f4f5740000000p-1, 0x1.d7513e0000000p-3, 0x1.6a75e60000000p-2}
}
, {{-0x1.7be0e20000000p-2, -0x1.0da2e80000000p-1, -0x1.6d0ea80000000p-1, -0x1.6895780000000p-1, -0x1.0e94ae0000000p-2}
, {0x1.3fc00e0000000p-2, -0x1.4e16e00000000p-1, -0x1.04d5140000000p-2, 0x1.efdb840000000p-3, 0x1.46ece20000000p-4}
, {-0x1.25ae020000000p-2, 0x1.1850f80000000p-3, 0x1.74473c0000000p-2, 0x1.a986040000000p-3, 0x1.3b228a0000000p-8}
, {-0x1.c241e40000000p-4, 0x1.b2a2440000000p-2, 0x1.1f8a020000000p-3, 0x1.c073260000000p-2, 0x1.e8f3060000000p-5}
, {-0x1.a773460000000p-2, -0x1.d2306e0000000p-5, -0x1.76598a0000000p-5, 0x1.135bd20000000p-5, -0x1.7d4df20000000p-4}
, {0x1.9645000000000p-3, -0x1.1643900000000p-5, 0x1.5608240000000p-3, 0x1.1fe0860000000p-3, -0x1.848f780000000p-4}
, {-0x1.72e2f80000000p-3, -0x1.d9be360000000p-5, 0x1.6b81220000000p-3, 0x1.d1d2c20000000p-3, -0x1.93af2e0000000p-4}
, {0x1.516b340000000p-1, -0x1.5cd7b40000000p-3, 0x1.5a66800000000p-4, -0x1.7705c40000000p-4, -0x1.8393480000000p-2}
}
, {{0x1.7af1880000000p-3, -0x1.a3b2ee0000000p-3, -0x1.67a3b40000000p-2, 0x1.31f9e20000000p-2, -0x1.74e4720000000p-5}
, {-0x1.98e42a0000000p-4, 0x1.85df660000000p-3, 0x1.43f6580000000p-2, 0x1.5ba5300000000p-2, 0x1.02c1c00000000p-4}
, {-0x1.ce31d80000000p-2, -0x1.8a3fe60000000p-4, 0x1.65cd480000000p-5, -0x1.3b76420000000p-4, 0x1.b1d54c0000000p-3}
, {0x1.d255ea0000000p-4, -0x1.1930420000000p-2, -0x1.6748420000000p-4, -0x1.0059ec0000000p-2, -0x1.a35bb60000000p-5}
, {-0x1.7a99f60000000p-5, 0x1.6e8afc0000000p-4, 0x1.ba62620000000p-5, -0x1.063c9a0000000p-3, -0x1.369ad20000000p-4}
, {-0x1.2796ca0000000p-3, -0x1.ca42620000000p-10, 0x1.09f3700000000p-4, 0x1.72f2040000000p-6, -0x1.ac4af60000000p-2}
, {-0x1.5c86ea0000000p-2, 0x1.4392240000000p-1, 0x1.d504660000000p-3, -0x1.6978980000000p-2, -0x1.d5090c0000000p-1}
, {0x1.e17b900000000p-5, -0x1.e310380000000p-3, -0x1.7662ce0000000p-5, -0x1.216c160000000p-1, -0x1.b265980000000p-5}
}
, {{-0x1.c2a1f20000000p-6, -0x1.ecf8f80000000p-3, -0x1.0b91480000000p+0, -0x1.59e4940000000p-3, -0x1.3696bc0000000p-1}
, {0x1.6a87820000000p-3, 0x1.6c3bb80000000p-4, -0x1.931f9e0000000p-3, -0x1.61a0d60000000p-4, 0x1.88de6c0000000p-4}
, {0x1.3fff220000000p-2, 0x1.408e2c0000000p-2, -0x1.10f8ca0000000p-1, 0x1.404e460000000p-3, 0x1.02bdde0000000p-3}
, {-0x1.aa7c940000000p-2, -0x1.094eb40000000p-2, 0x1.e08cec0000000p-4, -0x1.e5969e0000000p-2, -0x1.22f1e80000000p-7}
, {0x1.86c43c0000000p-2, 0x1.0d1d400000000p-2, -0x1.bab97c0000000p-1, -0x1.12ea340000000p+0, -0x1.4ff2360000000p-1}
, {0x1.62c9f40000000p-2, 0x1.69e1060000000p-3, -0x1.4a5c8a0000000p-1, 0x1.a23a420000000p-2, 0x1.e21eaa0000000p-4}
, {0x1.4475780000000p-4, -0x1.3821e60000000p-2, -0x1.027f1e0000000p+0, -0x1.3fc2ce0000000p+0, -0x1.1399c80000000p+0}
, {-0x1.8f22ce0000000p-4, 0x1.cff4e60000000p-3, 0x1.b3af180000000p-6, -0x1.fa72b20000000p-3, 0x1.0514600000000p-1}
}
, {{-0x1.35ef580000000p-4, -0x1.2516a00000000p-3, -0x1.3ee6860000000p-2, -0x1.7fc54c0000000p-5, -0x1.24f33c0000000p-2}
, {0x1.86ce680000000p-4, -0x1.7f1da00000000p-3, -0x1.161f940000000p-4, -0x1.1f2a3e0000000p-2, -0x1.7ba9820000000p-3}
, {0x1.7818f20000000p-3, 0x1.2337ca0000000p-6, 0x1.bdf84a0000000p-3, 0x1.65d2760000000p-3, -0x1.e585700000000p-6}
, {0x1.c335560000000p-1, 0x1.03e5cc0000000p-2, 0x1.56fa340000000p-6, 0x1.b04c4a0000000p-3, -0x1.bb69940000000p-4}
, {-0x1.db94b00000000p-2, -0x1.2592f20000000p-3, -0x1.b52ca00000000p-5, -0x1.bf671c0000000p-6, 0x1.057ddc0000000p-3}
, {0x1.0438b20000000p-2, 0x1.a660f80000000p-3, 0x1.6966c60000000p-2, -0x1.10529e0000000p-11, 0x1.df1ada0000000p-3}
, {-0x1.7e6bf20000000p-4, -0x1.75a9440000000p-4, -0x1.0f7fae0000000p-1, -0x1.ef340a0000000p-6, -0x1.c34efa0000000p-2}
, {0x1.5fe4ca0000000p-1, 0x1.847b800000000p-2, 0x1.9082820000000p-2, 0x1.cd3a740000000p-2, 0x1.93cf560000000p-3}
}
, {{0x1.5f4a3c0000000p-3, -0x1.c400480000000p-4, 0x1.5a5bb80000000p-1, 0x1.1d8ed80000000p-1, -0x1.3058d00000000p-3}
, {-0x1.b96b300000000p-1, -0x1.61f7980000000p-2, 0x1.c8c9a80000000p-4, -0x1.07457e0000000p-3, 0x1.4ce0dc0000000p-1}
, {-0x1.9ea4e60000000p-1, -0x1.52776a0000000p-1, -0x1.1fa2f80000000p-3, -0x1.5b7dee0000000p-2, -0x1.0fa8dc0000000p-7}
, {-0x1.52dac60000000p+0, 0x1.8bc0c40000000p-2, 0x1.9fc05c0000000p-2, -0x1.4739740000000p-2, 0x1.9176020000000p-4}
, {0x1.4a6eca0000000p-2, -0x1.d6f0ce0000000p-4, -0x1.7a86820000000p-2, 0x1.63e6b00000000p-2, -0x1.45e0c60000000p-7}
, {-0x1.e3cb6e0000000p-2, -0x1.85b0900000000p-3, -0x1.1560200000000p-4, -0x1.51ace00000000p-5, 0x1.6c1db00000000p-3}
, {0x1.d5c5b80000000p-2, -0x1.81afaa0000000p-1, 0x1.83771e0000000p-4, 0x1.9e49680000000p-2, -0x1.a33bce0000000p-2}
, {-0x1.12b9100000000p+0, -0x1.50d0a60000000p-1, 0x1.a94e920000000p-6, -0x1.2000580000000p-3, 0x1.337be20000000p-2}
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


const float conv1d_5_bias[CONV_FILTERS] = {-0x1.14439c0000000p-3, -0x1.8e1bae0000000p-3, 0x1.9dd75a0000000p-2, 0x1.8e557c0000000p-5, 0x1.23f4740000000p-3, -0x1.7b15320000000p-4, -0x1.6387660000000p-2, -0x1.4f70aa0000000p-7}
;

const float conv1d_5_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{0x1.c48c560000000p-2, -0x1.25cfe20000000p-9, -0x1.6f5cc20000000p-1, -0x1.33739a0000000p-1, 0x1.8371bc0000000p-4, -0x1.00a5120000000p-2, -0x1.1869f20000000p-1}
, {-0x1.fcc8740000000p-1, -0x1.2c41220000000p+0, -0x1.99563a0000000p-2, -0x1.50638a0000000p-2, -0x1.c8ebde0000000p-2, 0x1.ff28d60000000p-3, 0x1.d50f1c0000000p-2}
, {-0x1.f074be0000000p-4, -0x1.2b7bf20000000p-6, -0x1.5b1d7c0000000p-2, 0x1.c990040000000p-4, -0x1.baf2a00000000p-5, -0x1.a2ff880000000p-1, -0x1.c6c0ea0000000p-1}
, {0x1.2b5aee0000000p-3, -0x1.a357940000000p-3, -0x1.0d59ae0000000p-8, -0x1.06054e0000000p-2, -0x1.8ac8ea0000000p-3, -0x1.3b01ce0000000p-3, 0x1.ec0e840000000p-4}
, {-0x1.610d480000000p-5, 0x1.098ef00000000p-3, -0x1.aa719e0000000p-6, 0x1.80c6d60000000p-2, 0x1.e227d20000000p-2, 0x1.049a0e0000000p-1, 0x1.76fcc20000000p-2}
, {-0x1.66d41a0000000p-3, -0x1.b9a2740000000p-3, 0x1.b865100000000p-6, 0x1.3a64940000000p-6, 0x1.bc3bd60000000p-2, 0x1.8507b00000000p-3, 0x1.3d82400000000p-1}
, {0x1.e8f9e40000000p-3, 0x1.7fd9c80000000p-3, 0x1.d8b6c00000000p-4, -0x1.b7b1880000000p-2, 0x1.39ff460000000p-7, -0x1.44b93c0000000p-3, 0x1.2d7fd40000000p-2}
, {0x1.4ac23a0000000p-5, -0x1.2911c20000000p-1, -0x1.0030900000000p-2, -0x1.bac8740000000p-2, 0x1.37ec0c0000000p-4, -0x1.1907380000000p-2, -0x1.4ecba00000000p-2}
}
, {{-0x1.a6e5ea0000000p-5, 0x1.9e6d720000000p-3, -0x1.5ba7760000000p-1, -0x1.3802c80000000p-1, 0x1.52f3200000000p-2, 0x1.648c0a0000000p-3, 0x1.f49bc00000000p-3}
, {-0x1.1b9f0c0000000p-3, 0x1.85a5ec0000000p-5, -0x1.273a4c0000000p-6, 0x1.047e8a0000000p-1, 0x1.a27c2c0000000p-2, -0x1.e405f40000000p-3, 0x1.da5a2c0000000p-7}
, {0x1.a4879a0000000p-7, 0x1.9dc9280000000p-2, -0x1.47b4740000000p-2, -0x1.ffa4d20000000p-1, -0x1.be5ac80000000p-3, 0x1.5f41ac0000000p-3, 0x1.18ac700000000p-1}
, {-0x1.53a64e0000000p-5, -0x1.ef131a0000000p-4, -0x1.2f18ea0000000p-4, -0x1.3657080000000p-3, 0x1.612a0c0000000p-6, -0x1.e2dc680000000p-6, 0x1.545ca00000000p-4}
, {0x1.24ef0c0000000p-2, 0x1.3555120000000p-3, 0x1.7d00820000000p-4, -0x1.a49fb40000000p-3, -0x1.c977bc0000000p-4, -0x1.9fcb420000000p-3, -0x1.c991000000000p-2}
, {-0x1.4dd0660000000p-2, -0x1.8059c60000000p-7, 0x1.0e8fd20000000p-2, 0x1.e35f480000000p-5, -0x1.48e7240000000p-2, -0x1.3b6afc0000000p+0, -0x1.e8cd2e0000000p-2}
, {-0x1.f355980000000p-8, -0x1.1066d60000000p-5, 0x1.3e6fd40000000p-2, -0x1.7a450e0000000p-3, -0x1.5c12f40000000p-2, 0x1.50eb620000000p-7, 0x1.9a23380000000p-3}
, {0x1.5200620000000p-3, 0x1.4a58380000000p-4, -0x1.0ecaa80000000p-1, -0x1.52797a0000000p-1, -0x1.f757c20000000p-5, -0x1.01ab260000000p-1, -0x1.bc44e40000000p-1}
}
, {{-0x1.9f78460000000p-1, -0x1.6b3d460000000p-1, -0x1.22aa200000000p-2, 0x1.8b426e0000000p-2, 0x1.3185be0000000p-6, 0x1.9b5daa0000000p-2, -0x1.6881f40000000p-3}
, {0x1.0c1c4c0000000p+0, 0x1.c8fa840000000p-1, 0x1.8f89a80000000p-3, -0x1.5ab38e0000000p-3, 0x1.ac88ca0000000p-5, -0x1.26446e0000000p+0, -0x1.07e0920000000p-6}
, {-0x1.29ffd40000000p-1, -0x1.304ff40000000p-4, -0x1.0635720000000p-2, -0x1.3a009e0000000p-2, -0x1.89b25e0000000p-3, -0x1.a1d1020000000p-1, -0x1.e2b6fe0000000p-4}
, {-0x1.674c7c0000000p-1, -0x1.1585c00000000p-1, -0x1.17aa840000000p-2, 0x1.7f0cf20000000p-3, 0x1.ef43500000000p-2, 0x1.d3f9540000000p-3, 0x1.72267e0000000p-2}
, {-0x1.e26e460000000p-3, -0x1.39a5760000000p-1, -0x1.b18fc40000000p-4, 0x1.23920c0000000p-3, 0x1.df8b520000000p-4, 0x1.297b240000000p-3, 0x1.5322180000000p-2}
, {-0x1.f44d2c0000000p-2, -0x1.8061fc0000000p-3, -0x1.b9911c0000000p-3, 0x1.5cb8760000000p-3, 0x1.3103f00000000p-3, 0x1.ae67b20000000p-4, 0x1.ad13440000000p-2}
, {-0x1.3df2780000000p-1, -0x1.3a5a900000000p-1, -0x1.f2405c0000000p-4, -0x1.e62f840000000p-3, 0x1.faef340000000p-4, -0x1.abdf300000000p-5, 0x1.cada760000000p-3}
, {0x1.8b04aa0000000p-1, 0x1.2f06e20000000p-1, 0x1.89c65c0000000p-3, -0x1.3c55d00000000p-2, -0x1.bc2e740000000p-1, -0x1.e936400000000p-2, -0x1.b619ea0000000p-3}
}
, {{-0x1.997f200000000p-3, -0x1.5ff1460000000p-3, -0x1.5f2a9c0000000p-1, -0x1.50fce20000000p-3, -0x1.9aea9c0000000p-3, 0x1.8f2e800000000p-2, 0x1.67a14e0000000p-3}
, {-0x1.2e06480000000p-4, 0x1.3c6de80000000p-4, -0x1.92e78e0000000p-2, -0x1.4b85e80000000p-3, -0x1.e438ee0000000p-2, -0x1.2517f40000000p-1, -0x1.75d1cc0000000p-2}
, {0x1.2203280000000p-2, 0x1.dbb7c60000000p-4, -0x1.3e67480000000p-3, -0x1.5d9a480000000p-3, 0x1.a016320000000p-3, 0x1.496b540000000p-2, 0x1.b979e60000000p-2}
, {-0x1.34f6cc0000000p-2, -0x1.a4623c0000000p-3, -0x1.cf3cbc0000000p-5, -0x1.a70f8e0000000p-5, -0x1.ebd28c0000000p-5, 0x1.b81ee00000000p-10, 0x1.f3a3a80000000p-4}
, {-0x1.6c07f00000000p-2, -0x1.42d0160000000p-3, 0x1.2606240000000p-3, 0x1.90341e0000000p-6, 0x1.d626b20000000p-3, 0x1.3571a60000000p-4, 0x1.0dadbc0000000p-1}
, {-0x1.7f50840000000p-3, 0x1.9e40100000000p-5, -0x1.de7cae0000000p-4, -0x1.01693c0000000p-1, -0x1.a3638c0000000p-3, 0x1.6776160000000p-6, 0x1.52c9520000000p-2}
, {-0x1.f66ae60000000p-2, -0x1.b7dd340000000p-4, -0x1.6533d80000000p-5, -0x1.20d0dc0000000p-5, -0x1.a0af560000000p-4, -0x1.f3c9020000000p-3, -0x1.0f2cd40000000p-2}
, {-0x1.d914820000000p-3, 0x1.33589a0000000p-3, 0x1.4099f80000000p-5, 0x1.c44e840000000p-9, -0x1.38ba1c0000000p-3, -0x1.807ed20000000p-5, 0x1.5efdd60000000p-2}
}
, {{0x1.1debf80000000p-5, -0x1.146f6a0000000p-4, -0x1.285eb00000000p-4, 0x1.7517620000000p-3, -0x1.c5fb360000000p-2, -0x1.6f1b1e0000000p-1, 0x1.d20fd00000000p-3}
, {0x1.2af6140000000p-3, 0x1.922c500000000p-4, -0x1.20c31a0000000p-2, 0x1.11da320000000p-5, 0x1.3097180000000p-7, -0x1.cebbb00000000p-1, -0x1.e0db7a0000000p-1}
, {-0x1.c8e4660000000p-1, -0x1.7edd680000000p-2, -0x1.def4d20000000p-2, -0x1.d1cc7c0000000p-2, -0x1.34bbbc0000000p-3, -0x1.db034c0000000p-2, -0x1.2493a00000000p-1}
, {0x1.c8cd280000000p-3, 0x1.07084e0000000p-4, -0x1.9cec180000000p-2, -0x1.73811a0000000p-4, 0x1.0b820a0000000p-1, -0x1.b5d9680000000p-4, 0x1.31e7aa0000000p-3}
, {-0x1.7d8c460000000p-3, -0x1.77547e0000000p-2, -0x1.2167940000000p-2, -0x1.4680ae0000000p-2, 0x1.eb88780000000p-6, 0x1.d11b680000000p-2, -0x1.c76a260000000p-5}
, {0x1.283b1e0000000p-3, -0x1.0867c00000000p-3, 0x1.9303240000000p-2, -0x1.0ade580000000p-4, 0x1.86b74a0000000p-4, -0x1.606ad40000000p-7, -0x1.7477d80000000p-5}
, {-0x1.68d6e20000000p-2, -0x1.4f83500000000p-1, -0x1.b85a220000000p-2, -0x1.23f36e0000000p-1, -0x1.c2e4d80000000p-3, -0x1.66536a0000000p-1, -0x1.ec1afc0000000p-2}
, {-0x1.df3f3e0000000p-3, -0x1.edc7c80000000p-2, -0x1.7a4ffe0000000p-1, -0x1.6ef5a60000000p-5, 0x1.4f8ab60000000p-1, 0x1.d1d90a0000000p-3, -0x1.b41dbe0000000p-1}
}
, {{-0x1.0c9c0a0000000p-4, -0x1.c9b26a0000000p-2, 0x1.1a314e0000000p-6, -0x1.6e9a040000000p-2, 0x1.ff49140000000p-3, -0x1.3ddfa00000000p-6, 0x1.52e1800000000p-5}
, {-0x1.d2cc300000000p-1, -0x1.bb29fa0000000p-2, -0x1.47d2320000000p-2, 0x1.26acd80000000p-1, -0x1.5c6c5e0000000p-6, 0x1.9e0dde0000000p-10, -0x1.050ca60000000p-1}
, {0x1.634eb20000000p-5, -0x1.20f86e0000000p-7, -0x1.c5de860000000p-3, -0x1.9789fc0000000p-2, -0x1.53b0780000000p-2, 0x1.6d22a00000000p-5, 0x1.95229a0000000p-5}
, {0x1.7820de0000000p-2, 0x1.6039800000000p-4, 0x1.47ad140000000p-4, 0x1.905bb80000000p-3, 0x1.2c49ae0000000p-2, 0x1.012b2e0000000p-4, 0x1.134cf80000000p-6}
, {-0x1.56e2880000000p-2, -0x1.6260e60000000p-3, -0x1.b56f1a0000000p-2, 0x1.7d62060000000p-3, 0x1.59c82a0000000p-4, 0x1.5a11660000000p-7, 0x1.2737aa0000000p-3}
, {-0x1.ba0bdc0000000p-2, -0x1.3986a20000000p-2, -0x1.a1f9440000000p-2, -0x1.ddf07e0000000p-4, 0x1.14680e0000000p-4, 0x1.2ed7f40000000p-6, -0x1.c343960000000p-2}
, {0x1.76f4180000000p-1, 0x1.f7f4cc0000000p-2, 0x1.73e2ea0000000p-3, 0x1.358c000000000p-2, -0x1.ff3cce0000000p-3, -0x1.8020d20000000p-3, -0x1.88dcfa0000000p-2}
, {-0x1.1ba8ac0000000p-1, -0x1.3fce5a0000000p-2, -0x1.6c34100000000p-3, -0x1.973d580000000p-1, -0x1.21dc9a0000000p-5, -0x1.2a19880000000p-2, -0x1.4dd9440000000p-8}
}
, {{0x1.de44ce0000000p-3, 0x1.24de260000000p-3, 0x1.38a9800000000p-3, 0x1.b915a20000000p-4, -0x1.5afc640000000p-3, 0x1.023bae0000000p-2, -0x1.3332400000000p-3}
, {-0x1.f0577e0000000p-1, -0x1.72c4320000000p-2, 0x1.956d700000000p-1, 0x1.1117a80000000p-1, 0x1.82adf80000000p-2, -0x1.890a300000000p-5, 0x1.8eb9a40000000p-2}
, {0x1.467db40000000p-3, 0x1.4d42240000000p-4, -0x1.e969440000000p-3, -0x1.28337a0000000p-4, -0x1.7472a80000000p-2, -0x1.dcbb420000000p-2, -0x1.8f76e20000000p-3}
, {0x1.efe4c60000000p-4, -0x1.963c980000000p-4, 0x1.e90dbe0000000p-6, -0x1.de620e0000000p-8, -0x1.9fa5f20000000p-2, -0x1.394cce0000000p-4, -0x1.8737960000000p-5}
, {0x1.3f9f9e0000000p-3, -0x1.1a372c0000000p-2, -0x1.176d000000000p-4, -0x1.20334c0000000p-2, -0x1.030e6a0000000p-1, -0x1.f594d20000000p-2, -0x1.347fda0000000p-1}
, {-0x1.7922a00000000p-1, 0x1.a455900000000p-7, -0x1.6679780000000p-2, -0x1.9e01020000000p-4, -0x1.3d743a0000000p-2, 0x1.a80e560000000p-3, 0x1.5e987a0000000p-1}
, {0x1.695cf40000000p-2, 0x1.d38d480000000p-3, -0x1.a2019c0000000p-9, 0x1.71f2b00000000p-2, -0x1.1d759e0000000p-5, -0x1.6fd8440000000p-3, -0x1.9cca740000000p-6}
, {-0x1.4d96ac0000000p-3, -0x1.424c3a0000000p-2, -0x1.5cf2e00000000p-2, -0x1.93a44c0000000p-1, -0x1.fd31780000000p-1, -0x1.20b12a0000000p+0, -0x1.74a6d40000000p-1}
}
, {{-0x1.05517c0000000p-6, 0x1.2ae2420000000p-4, -0x1.e92f1a0000000p-4, -0x1.1407e60000000p-2, 0x1.49be360000000p-2, -0x1.0c7d6c0000000p-1, -0x1.b1640c0000000p-2}
, {-0x1.34160a0000000p-1, -0x1.cb51b20000000p-2, 0x1.e5fe360000000p-2, 0x1.66b7b80000000p-1, 0x1.7e867a0000000p-1, 0x1.12e22a0000000p-2, 0x1.5adc540000000p-2}
, {-0x1.b977100000000p-3, 0x1.c2e4e80000000p-3, -0x1.67d3080000000p-9, 0x1.97e6ac0000000p-4, -0x1.7f2a0e0000000p-4, -0x1.44201e0000000p-4, 0x1.4bfa240000000p-5}
, {0x1.b42c7e0000000p-2, -0x1.085c860000000p-5, 0x1.32cc340000000p-3, -0x1.021fbe0000000p-1, -0x1.2f940a0000000p-3, 0x1.1d5b560000000p-4, -0x1.fef6e60000000p-2}
, {0x1.6748760000000p-7, 0x1.df4a8a0000000p-4, -0x1.e83e600000000p-3, -0x1.6bdac60000000p-4, -0x1.06f6560000000p-2, -0x1.9144340000000p-3, -0x1.c362ce0000000p-2}
, {-0x1.7cc5160000000p-2, 0x1.6981880000000p-2, 0x1.0b10ca0000000p-3, 0x1.bd74aa0000000p-2, -0x1.5dfacc0000000p-3, 0x1.f7db800000000p-6, -0x1.0e0c3a0000000p+0}
, {0x1.863e720000000p-1, 0x1.5e49900000000p-2, -0x1.3021f80000000p-4, -0x1.2096360000000p-2, -0x1.71f67e0000000p-4, 0x1.5c6df80000000p-4, -0x1.ea93420000000p-3}
, {-0x1.23181c0000000p-1, -0x1.2a82e60000000p-2, -0x1.5b9be80000000p-1, -0x1.60fd920000000p-2, 0x1.ae7f100000000p-3, 0x1.9a82ba0000000p-4, 0x1.73e2400000000p-2}
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


const float dense_1_bias[FC_UNITS] = {0x1.549e0c0000000p-3, -0x1.1a900a0000000p-3, -0x1.92a2700000000p-2, 0x1.518eca0000000p-3, 0x1.07c2600000000p-7, 0x1.4e0bd00000000p-2}
;

const float dense_1_kernel[FC_UNITS][INPUT_SAMPLES] = {{-0x1.5483c00000000p-1, -0x1.8619440000000p-2, 0x1.c475a20000000p-2, -0x1.80c2fc0000000p-3, -0x1.7f1f4a0000000p-4, -0x1.6097640000000p-3, -0x1.0bb4e20000000p-2, -0x1.1e3b3e0000000p-2, 0x1.a5c5b40000000p-3, 0x1.00bbaa0000000p-5, 0x1.35a66e0000000p-3, -0x1.4967a60000000p-5, 0x1.d48bb80000000p-2, 0x1.66ca320000000p-3, -0x1.8340760000000p-6, -0x1.aa82720000000p-5, 0x1.13aec00000000p-2, 0x1.fcfa700000000p-3, 0x1.51314a0000000p-3, 0x1.afbc8e0000000p-5, -0x1.6757040000000p-2, 0x1.1180c40000000p-6, -0x1.2623f40000000p-5, -0x1.7a90b20000000p-1, -0x1.5f85260000000p-1, -0x1.ddc2500000000p-5, -0x1.5f264c0000000p-5, -0x1.28d4e80000000p-7, -0x1.2ba7820000000p-3, -0x1.b87fca0000000p-2, -0x1.36ce980000000p-2, -0x1.4898ee0000000p-1, -0x1.b920b20000000p-2, -0x1.6865060000000p-4, -0x1.ae7fb40000000p-6, 0x1.0b6c9a0000000p-4, -0x1.cd22740000000p-3, -0x1.3b23680000000p-5, 0x1.0484420000000p-2, -0x1.688f7a0000000p-1, 0x1.30324c0000000p-2, -0x1.64d5ba0000000p-6, 0x1.e954f20000000p-3, 0x1.14e9700000000p-3, -0x1.fdffc00000000p-5, -0x1.04300c0000000p-3, 0x1.18b9f00000000p-6, -0x1.9453e20000000p-4, -0x1.d85e800000000p-2, -0x1.cd85340000000p-2, -0x1.a6be540000000p-3, -0x1.4282f40000000p-3, -0x1.b6645e0000000p-4, -0x1.4b6d8a0000000p-2, 0x1.20e1f60000000p-7, 0x1.5d48380000000p-2, 0x1.cdfb8e0000000p-2, -0x1.75cf000000000p-4, 0x1.ec47f40000000p-6, 0x1.4a7fc80000000p-3, 0x1.69f2420000000p-5, -0x1.898e820000000p-3, 0x1.55e63a0000000p-4, -0x1.114cc20000000p-3, -0x1.5462240000000p-3, -0x1.19dde20000000p-2, -0x1.8b948e0000000p-4, 0x1.f6a1a80000000p-4, 0x1.4dcbb60000000p-3, -0x1.6a71220000000p-1, -0x1.94198e0000000p-1, 0x1.915f560000000p-5, 0x1.b31f580000000p-4, 0x1.4c73b40000000p-3, -0x1.0de3700000000p-3, -0x1.1b9ef40000000p-4, 0x1.7959880000000p-5, -0x1.35cd860000000p-3, -0x1.b81a5c0000000p-4, 0x1.0160e60000000p-2, 0x1.c5b7540000000p-4, -0x1.5dadcc0000000p-2, -0x1.3e0b2c0000000p-3, -0x1.5417680000000p-2, -0x1.04d07e0000000p-3, 0x1.518df80000000p-4, -0x1.8442080000000p-3, -0x1.5e3c620000000p-3, -0x1.511e480000000p-2, -0x1.a9f2220000000p-3, -0x1.5a3e5e0000000p-2, 0x1.7bb83e0000000p-4, -0x1.9c76de0000000p-1, -0x1.1802700000000p+0, -0x1.442fd40000000p-2, -0x1.239e520000000p-1, -0x1.1a3d060000000p-1, 0x1.7cbbb40000000p-4, 0x1.4ffa660000000p-5, 0x1.370a1c0000000p-3, -0x1.6c2cb80000000p-4, -0x1.6a1e0e0000000p-3, 0x1.31ef880000000p-3, -0x1.e180b40000000p-5, -0x1.906f240000000p-7, 0x1.a177920000000p-4, -0x1.03e07e0000000p+0, -0x1.d138320000000p-1, -0x1.f709860000000p-3, 0x1.7504b40000000p-4, -0x1.0133960000000p-5, -0x1.0d1ed20000000p-2, -0x1.2fe7200000000p-4, -0x1.acff600000000p-3, -0x1.58b2fa0000000p-2, -0x1.ef02e00000000p-1, -0x1.06bd460000000p-1, -0x1.91137c0000000p-3, -0x1.3d50a80000000p-2, 0x1.ba8d3a0000000p-5, -0x1.ade4960000000p-4, -0x1.cf1e740000000p-2, -0x1.14707e0000000p-3, 0x1.53976e0000000p-3, 0x1.f4e5940000000p-3, -0x1.8944d60000000p-7, -0x1.da75ce0000000p-5, 0x1.32f3100000000p-7, 0x1.3a65d80000000p-3, 0x1.4130c60000000p-3, 0x1.2bab4e0000000p-2, 0x1.1ab0160000000p-3, 0x1.46cdc00000000p-2, 0x1.e679240000000p-3, 0x1.47459a0000000p-4, -0x1.38fe440000000p-2, -0x1.3dc9b80000000p-4, -0x1.dbf9e00000000p-2, -0x1.0ac7c00000000p-1, -0x1.b2ac500000000p-3, 0x1.6dfef40000000p-4, -0x1.6f8a920000000p-3, -0x1.3d1bba0000000p-2, 0x1.37c3860000000p-6, 0x1.725e500000000p-2, 0x1.87f5420000000p-4, 0x1.836e220000000p-3, 0x1.cc4d2e0000000p-6, 0x1.bdc4480000000p-2, 0x1.4ecc020000000p-2, 0x1.27a2bc0000000p-3, 0x1.79dcac0000000p-4, 0x1.35e8860000000p-2, 0x1.0052da0000000p-2, -0x1.f0b95c0000000p-3, 0x1.c1ad920000000p-4, 0x1.68bf320000000p-5, 0x1.c6caf00000000p-4, 0x1.01fdb00000000p-4, 0x1.41c0200000000p-3, -0x1.44532c0000000p-5, -0x1.53a61c0000000p-2, -0x1.1dbd5e0000000p-1, -0x1.bc07580000000p-1, 0x1.f62a320000000p-3, 0x1.4c39340000000p-1, 0x1.c97dc60000000p-2, 0x1.025dae0000000p-6, -0x1.9703a20000000p-3, 0x1.3fa7680000000p-2, -0x1.6846660000000p-5, -0x1.6532e40000000p-4, 0x1.b2cb7c0000000p-2, -0x1.6c7cc00000000p-2, 0x1.bc210c0000000p-4, -0x1.fa172c0000000p-5, 0x1.e3bbf80000000p-3, -0x1.00927c0000000p-2, 0x1.ad2e9c0000000p-8, 0x1.1ea1200000000p-2, 0x1.2693a80000000p-3, -0x1.1008760000000p-9, -0x1.8a9f720000000p-3, 0x1.c756120000000p-6}
, {-0x1.25efa80000000p-2, -0x1.d2bc320000000p-3, 0x1.d250b80000000p-3, 0x1.6619580000000p-3, 0x1.7198d40000000p-6, -0x1.28a5640000000p-4, 0x1.8dfb040000000p-3, 0x1.346bb60000000p-2, 0x1.3bcec00000000p-3, 0x1.9e3dae0000000p-4, 0x1.374b220000000p-9, -0x1.ff5da00000000p-5, -0x1.3628960000000p-4, 0x1.f633120000000p-4, 0x1.31d8d20000000p-3, 0x1.a5911c0000000p-3, 0x1.31a2800000000p-5, -0x1.c8bc1c0000000p-5, -0x1.34c45e0000000p-3, -0x1.442bb60000000p-4, -0x1.73911c0000000p-3, -0x1.1910600000000p-2, -0x1.0276a20000000p-1, 0x1.9285ac0000000p-3, 0x1.1f59720000000p-1, 0x1.729cb60000000p-2, -0x1.18c41c0000000p-4, 0x1.7b48de0000000p-3, 0x1.ef13f80000000p-5, 0x1.abfdde0000000p-6, -0x1.0f29d80000000p-3, -0x1.b927440000000p-3, -0x1.8465300000000p-2, -0x1.fa20900000000p-5, -0x1.3b10a60000000p-3, -0x1.0fcecc0000000p-4, 0x1.c21e460000000p-6, 0x1.1296c40000000p-1, -0x1.213a140000000p-6, 0x1.07d5d00000000p-3, -0x1.44d2460000000p-1, 0x1.9b15480000000p-2, 0x1.5ea5e40000000p-2, 0x1.96a02e0000000p-3, 0x1.19c0080000000p-2, 0x1.1afda00000000p-1, -0x1.2206100000000p-1, -0x1.6d6b7c0000000p-1, -0x1.6758800000000p-2, -0x1.217c8a0000000p-3, -0x1.e888040000000p-4, -0x1.50ed840000000p-6, -0x1.19271a0000000p-6, -0x1.aec8060000000p-4, -0x1.bee9b80000000p-3, -0x1.305d260000000p-2, -0x1.dea46a0000000p-2, -0x1.057a760000000p-2, -0x1.a1e61c0000000p-3, 0x1.2b69da0000000p-4, 0x1.cf86480000000p-6, -0x1.458b000000000p-6, -0x1.4ccdb00000000p-3, 0x1.184d1e0000000p-8, -0x1.f854640000000p-5, -0x1.7173ea0000000p-3, -0x1.d955180000000p-3, -0x1.0665320000000p-2, -0x1.05b0400000000p-3, -0x1.e3079c0000000p-2, -0x1.d172f20000000p-3, -0x1.039f700000000p-4, -0x1.01a55c0000000p-7, -0x1.874e8e0000000p-5, 0x1.33eae00000000p-4, 0x1.45860a0000000p-4, 0x1.e2c6c80000000p-4, 0x1.2f8d140000000p-6, 0x1.f02c460000000p-2, -0x1.883f5e0000000p-7, 0x1.21e46e0000000p-3, 0x1.94be120000000p-3, 0x1.76851c0000000p-3, 0x1.033d040000000p-6, 0x1.6193460000000p-3, -0x1.09c4a60000000p-4, -0x1.64a5fa0000000p-2, -0x1.3817040000000p-3, 0x1.3c54420000000p-8, -0x1.4f05200000000p-7, -0x1.9e843e0000000p-4, 0x1.051ad00000000p-3, -0x1.e935120000000p-2, -0x1.f333020000000p-1, -0x1.f0cb100000000p-2, -0x1.a164240000000p-4, 0x1.459bc40000000p-2, 0x1.c9f4900000000p-4, 0x1.5a34120000000p-5, -0x1.df98c80000000p-6, -0x1.ed6a220000000p-3, -0x1.163b720000000p-3, -0x1.e763dc0000000p-3, -0x1.25c3820000000p-1, -0x1.22adae0000000p-4, 0x1.ced9340000000p-3, 0x1.2af1a40000000p-2, -0x1.2136a40000000p+0, -0x1.7cd2580000000p-1, 0x1.ea19b80000000p-5, -0x1.e23e3a0000000p-5, -0x1.82131e0000000p-3, 0x1.29995c0000000p-2, 0x1.3c88a20000000p-6, -0x1.42713e0000000p-4, 0x1.7060dc0000000p-3, 0x1.b3a79c0000000p-2, 0x1.2b5ae40000000p-2, 0x1.e801020000000p-2, 0x1.e6d5020000000p-3, 0x1.9be5640000000p-4, 0x1.0434f00000000p-2, -0x1.a857f00000000p-3, -0x1.b0d4a20000000p-3, -0x1.14ab380000000p-4, 0x1.48683c0000000p-3, -0x1.22dc160000000p-4, 0x1.53bea40000000p-4, -0x1.3787be0000000p-3, -0x1.ad37c40000000p-8, -0x1.fb25280000000p-2, -0x1.2e794c0000000p-4, -0x1.963f960000000p-3, 0x1.a35ea40000000p-6, -0x1.5fb5a60000000p-3, -0x1.07b41a0000000p-4, -0x1.d5f6900000000p-4, 0x1.abc2d00000000p-7, 0x1.fa01840000000p-3, 0x1.67e3e00000000p-3, -0x1.05ab900000000p-2, 0x1.90086e0000000p-2, 0x1.00cb7a0000000p-2, 0x1.696c4a0000000p-5, 0x1.21c0a00000000p-5, -0x1.f6ee360000000p-4, 0x1.1118b40000000p-3, -0x1.4ab4920000000p-4, 0x1.9de51e0000000p-5, -0x1.3918f20000000p-4, -0x1.c264860000000p-3, -0x1.dd9be20000000p-6, -0x1.766ad20000000p-2, -0x1.0f9d060000000p-2, -0x1.02afba0000000p-2, 0x1.08227c0000000p-3, -0x1.0802f80000000p-4, 0x1.b32d0c0000000p-3, 0x1.0e7e660000000p-5, -0x1.8ee5d20000000p-6, 0x1.f6e3040000000p-3, -0x1.3933060000000p-3, 0x1.1159060000000p-6, 0x1.7b46d80000000p-3, 0x1.f0d77e0000000p-2, 0x1.bea2be0000000p-6, 0x1.bdf4d20000000p-5, 0x1.3bbcf20000000p-5, 0x1.2318140000000p-5, 0x1.6155120000000p-5, 0x1.07500a0000000p-2, 0x1.b462980000000p-3, 0x1.2e48020000000p-4, 0x1.efe25a0000000p-3, -0x1.fd804e0000000p-3, -0x1.1c865c0000000p-7, -0x1.82a7ba0000000p-3, -0x1.524bbe0000000p-7, 0x1.13e34e0000000p-2, -0x1.fb1e880000000p-3, -0x1.044dd20000000p-2, -0x1.dd44ea0000000p-5, -0x1.36357e0000000p-2, 0x1.6ac57e0000000p-3}
, {0x1.7368a40000000p-1, 0x1.ca05800000000p-2, -0x1.b1221a0000000p-3, 0x1.4d115a0000000p-3, 0x1.f6a5820000000p-2, 0x1.f08f7c0000000p-3, 0x1.8cc93c0000000p-3, -0x1.103a160000000p-5, -0x1.10b4fa0000000p-4, -0x1.94f0c00000000p-4, 0x1.48fff00000000p-5, 0x1.fe93820000000p-4, 0x1.3c78280000000p-3, 0x1.21398a0000000p-3, 0x1.7d6fb00000000p-6, -0x1.ddc1c80000000p-6, -0x1.6d92000000000p-3, 0x1.8e53600000000p-5, 0x1.76dae60000000p-3, -0x1.3658800000000p-7, 0x1.3016860000000p-3, 0x1.98e4080000000p-6, -0x1.9e67c00000000p-6, 0x1.14004e0000000p-4, -0x1.132b580000000p-1, -0x1.74d78e0000000p-7, 0x1.252fc40000000p-2, 0x1.d1417c0000000p-7, 0x1.ab1f1a0000000p-3, 0x1.6755080000000p-2, 0x1.cbce0e0000000p-3, 0x1.7e425e0000000p-1, 0x1.7159d60000000p-1, 0x1.9ec3300000000p-3, -0x1.9134d60000000p-4, 0x1.67ce5a0000000p-5, -0x1.28217e0000000p-3, -0x1.bca5f80000000p-2, -0x1.d3a23c0000000p-2, -0x1.f6bb6c0000000p-2, 0x1.78c8000000000p-2, -0x1.2888440000000p-1, -0x1.4241fe0000000p-2, -0x1.5fb6180000000p-2, -0x1.a978700000000p-5, -0x1.3199000000000p-1, -0x1.274d7c0000000p-1, -0x1.837b5e0000000p-2, 0x1.60e88a0000000p-2, 0x1.a042460000000p-4, 0x1.905f0a0000000p-3, 0x1.b1c7940000000p-3, 0x1.bde37c0000000p-4, 0x1.731f0e0000000p-5, 0x1.906a500000000p-3, 0x1.a1afd40000000p-7, -0x1.2e20a00000000p-2, 0x1.453e420000000p-4, 0x1.979fa80000000p-3, -0x1.8fbd6c0000000p-2, -0x1.0608d80000000p-3, -0x1.14719e0000000p-4, -0x1.3231ea0000000p-3, -0x1.2fe66e0000000p-3, -0x1.272e740000000p-2, -0x1.65bc0a0000000p-3, -0x1.6c4bca0000000p-1, -0x1.c71bf80000000p-1, -0x1.ecd9fe0000000p-1, -0x1.db446a0000000p-1, -0x1.708b7e0000000p-1, -0x1.d7e8a40000000p-2, -0x1.646cb20000000p-3, 0x1.5b0a780000000p-3, 0x1.69ff280000000p-3, 0x1.5f155c0000000p-2, 0x1.dda8f40000000p-3, 0x1.5305da0000000p-3, -0x1.bbf55a0000000p-3, -0x1.681bc60000000p-3, -0x1.2bb53c0000000p-2, -0x1.460b0c0000000p-2, -0x1.1263120000000p-1, -0x1.7332c00000000p-3, -0x1.e240480000000p-3, -0x1.52a2920000000p-3, -0x1.c28db40000000p-6, -0x1.7e098c0000000p-2, -0x1.373e140000000p-1, -0x1.2a6d180000000p-1, -0x1.0c60060000000p-1, -0x1.f2448c0000000p-1, 0x1.6c21940000000p-4, -0x1.df20760000000p-3, -0x1.67c9cc0000000p-1, -0x1.2ccc780000000p-3, 0x1.193ca00000000p-2, 0x1.8baa220000000p-5, -0x1.30bf620000000p-5, -0x1.4d8d620000000p-2, -0x1.6b40c60000000p-4, -0x1.2ce4120000000p-2, 0x1.4dabf20000000p-4, -0x1.39ea020000000p-5, -0x1.4982f20000000p-4, -0x1.5c855c0000000p-5, -0x1.2296760000000p-2, -0x1.b116180000000p-1, -0x1.3ab57e0000000p-2, 0x1.b286140000000p-4, -0x1.4426460000000p-1, -0x1.2604540000000p-1, -0x1.32e2120000000p-2, -0x1.5c98dc0000000p-4, -0x1.ac67280000000p-2, 0x1.d5639c0000000p-2, 0x1.34a0260000000p-2, 0x1.3249ee0000000p-3, -0x1.061b720000000p-5, -0x1.c45ef60000000p-5, 0x1.b58c920000000p-5, 0x1.9eb0b80000000p-3, -0x1.37bf1e0000000p-5, -0x1.2366220000000p-6, -0x1.05c6b20000000p-2, -0x1.16bfb40000000p-4, 0x1.4601b80000000p-5, -0x1.9bc0a00000000p-4, 0x1.357ace0000000p-3, 0x1.2fa2380000000p-6, -0x1.e0967a0000000p-8, 0x1.03bdcc0000000p-3, -0x1.d2b2e20000000p-3, -0x1.04a24c0000000p-3, -0x1.42f81a0000000p-3, -0x1.1aa30a0000000p-3, 0x1.0ef20c0000000p-2, 0x1.ba7f9c0000000p-4, 0x1.f53e020000000p-2, 0x1.8cb56c0000000p-2, 0x1.f6e7720000000p-3, -0x1.a4ec740000000p-10, 0x1.519e620000000p-2, -0x1.84f5f40000000p-4, -0x1.6e295c0000000p-2, -0x1.20b49a0000000p-7, -0x1.4f2ca80000000p-3, -0x1.3d69080000000p-5, 0x1.8d4d8a0000000p-4, -0x1.e1832c0000000p-7, 0x1.1df3520000000p-2, 0x1.38c1320000000p-3, 0x1.7f713a0000000p-3, 0x1.62aa160000000p-6, 0x1.aa5ccc0000000p-3, 0x1.3dfb300000000p-2, 0x1.e7949c0000000p-5, 0x1.1557c20000000p-3, 0x1.31d4e00000000p-4, -0x1.03ee680000000p-7, -0x1.203b680000000p-3, 0x1.c270f20000000p-2, 0x1.3cdf920000000p-1, 0x1.c6d3f60000000p-3, -0x1.abec280000000p-2, -0x1.092aca0000000p-1, -0x1.78ad520000000p-2, -0x1.30ee320000000p-2, 0x1.5f2fe00000000p-5, -0x1.38cf9a0000000p-3, 0x1.4880be0000000p-7, 0x1.1f75480000000p-4, 0x1.095bc80000000p-8, -0x1.0abcea0000000p-3, 0x1.4073400000000p-10, -0x1.05638c0000000p-8, -0x1.1d61ac0000000p-3, 0x1.14a07a0000000p-2, -0x1.d6ad280000000p-3, -0x1.1f63da0000000p-5, -0x1.8fb91c0000000p-8, -0x1.33d1520000000p-3, 0x1.b39b940000000p-4, 0x1.a46e3a0000000p-4}
, {-0x1.ec524e0000000p-1, -0x1.2d2b760000000p-1, -0x1.7406960000000p-1, -0x1.d84d160000000p-2, -0x1.8d0e780000000p-1, -0x1.5fbbbe0000000p-5, -0x1.bddc500000000p-2, -0x1.44cc7e0000000p-2, -0x1.6342400000000p-7, 0x1.556f060000000p-2, -0x1.9245e00000000p-2, -0x1.c8ac000000000p-2, -0x1.d968220000000p-5, -0x1.467ce40000000p-3, -0x1.396bd00000000p-5, -0x1.2391b20000000p-4, 0x1.4516740000000p-3, 0x1.1723a80000000p-3, -0x1.8e35920000000p-5, 0x1.36edfe0000000p-5, 0x1.556d520000000p-2, 0x1.f8445e0000000p-2, 0x1.c1f3660000000p-1, -0x1.1036ee0000000p-3, 0x1.6b76f00000000p-3, -0x1.5c54160000000p-1, -0x1.40b65e0000000p-2, 0x1.574ba60000000p-1, -0x1.2e1a940000000p-6, -0x1.9862e80000000p-4, -0x1.8599200000000p-2, 0x1.99c05c0000000p-2, -0x1.83443c0000000p-2, 0x1.43401e0000000p-2, 0x1.b74a260000000p-5, -0x1.9aef140000000p-5, -0x1.b478a60000000p-3, -0x1.876fc00000000p-3, -0x1.38fa460000000p-1, -0x1.8378a20000000p-1, -0x1.544a6e0000000p-1, 0x1.a3517c0000000p-5, -0x1.6c186c0000000p-3, -0x1.67933a0000000p-3, 0x1.bd607e0000000p-6, 0x1.35e6440000000p-3, 0x1.16f47a0000000p-1, 0x1.f6cd0a0000000p-3, -0x1.cfc7d40000000p-6, -0x1.0b765e0000000p-4, -0x1.5a56140000000p-3, -0x1.964edc0000000p-4, -0x1.2087b00000000p-1, -0x1.3b958c0000000p+0, -0x1.3f76640000000p-1, 0x1.441b040000000p-4, 0x1.5cad520000000p-1, 0x1.e058720000000p-1, 0x1.1473760000000p-1, -0x1.0102160000000p-1, 0x1.f8e17a0000000p-6, -0x1.b736e00000000p-5, 0x1.3e7f0a0000000p-3, 0x1.3e25c60000000p-4, 0x1.c179ee0000000p-3, -0x1.d9ebb00000000p-3, -0x1.d314160000000p-2, -0x1.4367780000000p-1, -0x1.0b89aa0000000p-1, -0x1.8407bc0000000p-4, 0x1.23d6b80000000p-5, -0x1.0491ca0000000p-2, -0x1.7410ca0000000p-2, -0x1.7a549a0000000p-2, -0x1.03e1d60000000p-2, -0x1.610f900000000p-2, -0x1.d8b9860000000p-2, -0x1.de36c00000000p-4, -0x1.1f85bc0000000p-2, 0x1.a4d7c20000000p-4, 0x1.ad52680000000p-2, 0x1.29da2c0000000p-2, 0x1.6ffd960000000p-2, 0x1.d435ec0000000p-4, 0x1.ccee2a0000000p-5, 0x1.ae944a0000000p-5, 0x1.9ee0520000000p-4, 0x1.5edce60000000p-4, -0x1.5cbcf40000000p-4, -0x1.4e196c0000000p-3, -0x1.a4b25e0000000p-1, 0x1.0ddc860000000p-3, -0x1.ac851a0000000p-2, -0x1.d6a9940000000p-2, -0x1.2201e80000000p-2, 0x1.e5e3e20000000p-3, -0x1.8e959e0000000p-2, -0x1.4fa7080000000p-6, -0x1.5ee9720000000p-1, -0x1.fd6f000000000p-3, -0x1.36df660000000p-9, 0x1.d7edc60000000p-2, 0x1.23cfd40000000p-1, 0x1.4d041c0000000p-1, 0x1.bf33a40000000p-2, -0x1.6f58de0000000p-3, 0x1.6a8afc0000000p-12, 0x1.1975320000000p-2, 0x1.111c6c0000000p-3, 0x1.064df40000000p-4, -0x1.c687c80000000p-1, -0x1.298bf00000000p+0, 0x1.df0d0c0000000p-3, 0x1.673cd20000000p-2, -0x1.0821a80000000p-1, -0x1.5975420000000p+0, -0x1.4ba9500000000p+0, -0x1.c9628e0000000p-1, -0x1.3e7eac0000000p+0, -0x1.5123a60000000p-2, -0x1.01576e0000000p-1, -0x1.fa86120000000p-2, -0x1.6629980000000p-1, -0x1.6e835c0000000p-1, -0x1.a3069e0000000p-4, -0x1.5420fe0000000p-4, 0x1.e9111c0000000p-5, -0x1.01f7540000000p-1, -0x1.33bc0c0000000p-1, -0x1.09070e0000000p+0, -0x1.36b3140000000p-2, -0x1.3e734e0000000p-9, 0x1.0da2b00000000p-5, -0x1.56d68c0000000p-4, 0x1.1be7a20000000p-3, -0x1.bdc7da0000000p-3, -0x1.4a082c0000000p-2, -0x1.a2d12e0000000p-4, -0x1.e71a320000000p-1, -0x1.cba7b20000000p-1, -0x1.9404180000000p-2, 0x1.442aca0000000p-3, -0x1.e9fbe60000000p-6, 0x1.2e82e20000000p-5, 0x1.71f4ae0000000p-3, -0x1.77e9880000000p-3, 0x1.e2efee0000000p-4, -0x1.5c563a0000000p-2, -0x1.2da34a0000000p-3, -0x1.767f0e0000000p-1, -0x1.13a6080000000p-2, -0x1.c7c20a0000000p-3, -0x1.b505820000000p-3, -0x1.4ae70e0000000p-6, -0x1.165f960000000p-4, 0x1.ae2c760000000p-4, 0x1.ea25a40000000p-6, 0x1.6e82fa0000000p-4, -0x1.d6005e0000000p-4, 0x1.8427000000000p-3, -0x1.1bc9800000000p-4, -0x1.bdceb60000000p-1, -0x1.c15eb80000000p-3, -0x1.a4ba680000000p-5, -0x1.2236440000000p-2, 0x1.6328fa0000000p-4, 0x1.3c4e140000000p-3, 0x1.326cbc0000000p-5, -0x1.0e127a0000000p-3, 0x1.4e89380000000p-3, -0x1.7a85ee0000000p-4, -0x1.6eb2ce0000000p-1, -0x1.4bbf0c0000000p-1, -0x1.b31ad20000000p-1, -0x1.6fa8c20000000p-2, 0x1.23808c0000000p-2, -0x1.45cca20000000p-3, -0x1.10a6400000000p-1, -0x1.86b6b40000000p-3, -0x1.c3151a0000000p-3, -0x1.38fc3e0000000p-5, -0x1.a064040000000p-4, 0x1.20a2420000000p-3, 0x1.07728e0000000p-2}
, {-0x1.27ed1c0000000p-1, -0x1.8eda900000000p-1, -0x1.f4aaf00000000p-2, -0x1.64be3c0000000p-2, -0x1.03fdac0000000p-2, -0x1.3a8f420000000p-2, -0x1.41ba1c0000000p-3, 0x1.a5ae880000000p-3, -0x1.7162700000000p-4, -0x1.8e54200000000p-3, 0x1.f763860000000p-5, -0x1.e5b61c0000000p-4, -0x1.9979c60000000p-3, -0x1.23613c0000000p-3, -0x1.c7f70c0000000p-3, 0x1.128d580000000p-4, 0x1.2797420000000p-4, 0x1.926bea0000000p-4, -0x1.1a9b100000000p-9, 0x1.7ac76e0000000p-6, -0x1.b7f6d20000000p-3, -0x1.54e19a0000000p-2, -0x1.5310380000000p+0, -0x1.76ab9c0000000p-4, 0x1.cc4efc0000000p-5, 0x1.05729c0000000p-3, 0x1.7f06660000000p-4, -0x1.50832e0000000p-1, -0x1.1685820000000p-2, -0x1.4506d40000000p-4, -0x1.66a90e0000000p-3, -0x1.9175e80000000p-2, -0x1.e76ad40000000p-3, -0x1.0855e40000000p-4, 0x1.84f3da0000000p-4, 0x1.e2d2d40000000p-2, -0x1.83943c0000000p-2, -0x1.2c1b200000000p-3, -0x1.f745ee0000000p-4, -0x1.3f5f520000000p-3, -0x1.9af2560000000p-2, -0x1.04c67e0000000p-2, 0x1.810fea0000000p-3, 0x1.324a280000000p-2, -0x1.1919620000000p-3, -0x1.0a9ed60000000p-2, 0x1.cfc37e0000000p-3, 0x1.1907c40000000p-2, 0x1.e0900a0000000p-4, 0x1.81ac000000000p-5, 0x1.a1473a0000000p-4, 0x1.8aaac00000000p-5, 0x1.0fadf40000000p-4, 0x1.c2cd720000000p-1, 0x1.5295ee0000000p-3, 0x1.4b666e0000000p-3, -0x1.e481c20000000p-1, -0x1.3189c00000000p+0, -0x1.c684c00000000p-1, -0x1.0bf36c0000000p-3, -0x1.fb1cda0000000p-4, 0x1.61f8cc0000000p-3, 0x1.a15a7e0000000p-4, 0x1.ecd45c0000000p-3, 0x1.c019720000000p-3, 0x1.645e6c0000000p-9, -0x1.20b59e0000000p-2, -0x1.15b5f20000000p-3, -0x1.0925180000000p-1, 0x1.f603300000000p-1, 0x1.52cdba0000000p-1, 0x1.a751d20000000p-2, 0x1.8025600000000p-2, 0x1.02a9ac0000000p-2, 0x1.2e5d160000000p-6, -0x1.6a78220000000p-2, -0x1.3fca860000000p-2, -0x1.6b89da0000000p-1, -0x1.2e1aba0000000p-2, -0x1.601c880000000p-4, -0x1.2796920000000p-3, -0x1.a0a5fa0000000p-2, 0x1.426a9e0000000p-3, 0x1.1b54b20000000p-3, -0x1.831f220000000p-5, 0x1.565d820000000p-4, 0x1.9195860000000p-3, 0x1.8631900000000p-4, 0x1.f069560000000p-4, -0x1.3df04e0000000p-3, 0x1.1eee240000000p-2, -0x1.7200ee0000000p-1, 0x1.67c8900000000p-2, 0x1.0c97160000000p+0, 0x1.10509e0000000p-1, -0x1.3ff6780000000p-2, 0x1.ba7c160000000p-6, -0x1.e1575c0000000p-2, 0x1.39c8b60000000p-4, 0x1.c3cb1e0000000p-5, 0x1.1dbe8c0000000p-2, -0x1.916d160000000p-3, -0x1.088a320000000p-1, -0x1.a5a2980000000p-1, -0x1.dec8900000000p-2, 0x1.78b9260000000p-6, -0x1.79fb120000000p-7, 0x1.aaef440000000p-3, 0x1.8253e80000000p-3, 0x1.f93d800000000p-6, -0x1.c3a2c80000000p-2, -0x1.3315880000000p-3, 0x1.3477060000000p-6, 0x1.c5c1e60000000p-5, -0x1.71f63c0000000p+0, -0x1.24c7a40000000p+0, -0x1.0b65400000000p+0, -0x1.19b2880000000p+0, -0x1.a81ab00000000p-1, -0x1.77c30a0000000p-1, -0x1.c9e4d00000000p-1, -0x1.cf5bcc0000000p-2, -0x1.6a3f260000000p-2, -0x1.d13eb20000000p-6, -0x1.ebd9a00000000p-4, 0x1.1630740000000p-2, -0x1.d9a1620000000p-2, -0x1.9980280000000p-2, -0x1.680ce00000000p-1, -0x1.0eab300000000p-3, -0x1.31193a0000000p-3, -0x1.9d2d900000000p-5, -0x1.7a8bb40000000p-3, 0x1.9635500000000p-3, 0x1.21fff80000000p-5, -0x1.2e6afe0000000p-3, -0x1.6f2ce40000000p-4, -0x1.a494c40000000p-3, -0x1.f1429c0000000p-1, -0x1.f64c260000000p-1, -0x1.e68c5c0000000p-1, -0x1.1e840c0000000p+0, -0x1.208f100000000p-2, -0x1.bb70c80000000p-3, -0x1.ec507e0000000p-2, 0x1.52386a0000000p-5, -0x1.466a140000000p-3, 0x1.e1ffa20000000p-3, 0x1.6745380000000p-4, 0x1.0cd5760000000p-3, -0x1.a65e020000000p-3, -0x1.ff0ca40000000p-2, -0x1.fdc86a0000000p-2, -0x1.7642bc0000000p-4, -0x1.6ae4c80000000p-6, -0x1.33bcd00000000p-2, -0x1.5886a40000000p-3, 0x1.bfb2e80000000p-4, 0x1.b00b4a0000000p-3, 0x1.bf2b940000000p-3, -0x1.6155da0000000p-3, -0x1.173d840000000p-1, -0x1.7d88da0000000p-1, -0x1.208daa0000000p+0, -0x1.7b84a60000000p-1, 0x1.efd60c0000000p-4, -0x1.ef51160000000p-2, -0x1.c54aa00000000p-1, -0x1.f440e60000000p-2, -0x1.db23fe0000000p-2, -0x1.2309c40000000p-2, 0x1.e8049a0000000p-3, 0x1.6dfc440000000p-3, 0x1.5894000000000p-2, -0x1.de87580000000p-2, 0x1.0c7c260000000p-3, 0x1.7ec0be0000000p-2, -0x1.5e77260000000p-3, -0x1.3968120000000p-2, 0x1.1fdb660000000p-4, -0x1.0ab78e0000000p-5, -0x1.a1a3c00000000p-5, -0x1.e949340000000p-5, 0x1.cb4d360000000p-3}
, {-0x1.686fc20000000p-3, 0x1.470fb00000000p-7, 0x1.05808c0000000p-2, 0x1.eb10820000000p-3, 0x1.8d498a0000000p-2, 0x1.008ca00000000p-2, 0x1.f859f00000000p-4, 0x1.64082c0000000p-3, -0x1.e9fcec0000000p-4, -0x1.26e5360000000p-3, 0x1.3f6e480000000p-6, 0x1.92fdb60000000p-3, 0x1.d08bec0000000p-6, -0x1.8054e40000000p-5, -0x1.0bfbd80000000p-1, -0x1.324a8c0000000p-1, -0x1.7b2c220000000p-1, -0x1.4f0b3e0000000p-1, 0x1.5abcb00000000p-3, 0x1.7a07820000000p-2, 0x1.fd355a0000000p-3, 0x1.a8c19c0000000p-2, 0x1.811bac0000000p-1, 0x1.a4ac420000000p-2, -0x1.5f51820000000p-3, -0x1.24a30c0000000p-1, -0x1.cdec7e0000000p-3, 0x1.0cbcdc0000000p-3, 0x1.37ea320000000p-2, 0x1.b740d20000000p-3, -0x1.2c28320000000p-2, 0x1.422cf60000000p-3, 0x1.22218c0000000p-2, 0x1.03eaa00000000p-7, -0x1.0d52100000000p-3, 0x1.a72be80000000p-4, -0x1.0b3b440000000p-2, 0x1.5ac2c40000000p-3, 0x1.7b9d7c0000000p-1, 0x1.4dfd960000000p-1, 0x1.b4c70c0000000p-2, 0x1.6692f60000000p-3, 0x1.3a0e8c0000000p-4, -0x1.4492de0000000p+0, -0x1.7ec4380000000p-2, -0x1.3651a40000000p-5, 0x1.241e700000000p-7, 0x1.928fb40000000p-8, -0x1.8ded8e0000000p-4, 0x1.b5c3340000000p-4, -0x1.b373840000000p-5, 0x1.690f280000000p-3, 0x1.2dbc6c0000000p-2, 0x1.81fbe40000000p-4, 0x1.84b0aa0000000p-2, -0x1.9b8f7e0000000p-4, -0x1.19556a0000000p-2, -0x1.52d5420000000p-3, 0x1.c6e02a0000000p-5, 0x1.d0b5a00000000p-3, 0x1.0340ee0000000p-3, -0x1.81ac0a0000000p-3, -0x1.2265780000000p-2, -0x1.9a03de0000000p-2, -0x1.3d10c00000000p-2, 0x1.c3c5420000000p-3, 0x1.f84d160000000p-3, 0x1.52a0920000000p-1, 0x1.96a7380000000p-2, 0x1.15705a0000000p-1, 0x1.f035720000000p-2, 0x1.ed96c40000000p-4, -0x1.91d8ba0000000p-3, 0x1.dc1cd60000000p-4, 0x1.760fe20000000p-3, 0x1.11ffba0000000p-2, 0x1.c43f080000000p-5, 0x1.511cf80000000p-7, -0x1.82042c0000000p-4, -0x1.6e67820000000p-3, -0x1.0399420000000p-3, -0x1.75c26c0000000p-2, -0x1.6c8ac80000000p-1, -0x1.b802ca0000000p-1, -0x1.29c16c0000000p-1, -0x1.7ba7b00000000p-4, 0x1.0683a20000000p-2, 0x1.7a61360000000p-9, 0x1.5f2d280000000p-5, 0x1.0f14080000000p+0, 0x1.1ad8f00000000p+0, 0x1.90ffb40000000p-1, -0x1.0e45100000000p-3, 0x1.62c0f40000000p-2, 0x1.b1c2280000000p-4, -0x1.4a77380000000p-5, 0x1.9027a20000000p-2, -0x1.04f2080000000p-2, 0x1.dcf4600000000p-4, -0x1.6db0ce0000000p-6, -0x1.12ed7c0000000p-4, -0x1.5d714e0000000p-5, -0x1.887d420000000p-8, -0x1.ac50860000000p-4, -0x1.344a480000000p-5, 0x1.fd90a40000000p-4, -0x1.7d34900000000p-4, -0x1.d2017a0000000p-1, -0x1.d6c3540000000p-1, -0x1.4758c60000000p-1, 0x1.0c65c60000000p+0, 0x1.88bcdc0000000p+0, 0x1.1fb67e0000000p-2, -0x1.70fc300000000p-2, 0x1.b5a7580000000p+0, -0x1.abc72e0000000p-1, -0x1.5ca7160000000p-1, -0x1.7e05e00000000p-3, -0x1.0c088c0000000p-2, -0x1.ff6fea0000000p-3, -0x1.dffdfa0000000p-5, -0x1.aac7de0000000p-2, 0x1.40debe0000000p-4, -0x1.eddb380000000p-6, 0x1.01bf740000000p-2, 0x1.7ad0260000000p-5, 0x1.a4906c0000000p-6, 0x1.86258a0000000p-3, -0x1.688d0a0000000p-2, -0x1.a5c4a20000000p-5, -0x1.6e5ac60000000p-3, -0x1.4ce5c00000000p-1, -0x1.3909f20000000p-2, -0x1.96ace40000000p-2, -0x1.b9c09a0000000p-4, 0x1.43a97a0000000p-4, 0x1.d2c0000000000p-3, 0x1.a9cd920000000p-2, -0x1.8a3a2c0000000p-1, -0x1.519b4c0000000p-3, -0x1.1c66320000000p-5, 0x1.301de60000000p-4, 0x1.187f660000000p-5, -0x1.2ae7e60000000p-3, -0x1.1eeba40000000p-3, -0x1.3acae80000000p-4, 0x1.bf7ab60000000p-3, -0x1.a68e4c0000000p-4, -0x1.0a1eec0000000p-3, 0x1.a347720000000p-5, -0x1.70d0540000000p-2, 0x1.cb27380000000p-5, 0x1.bb35880000000p-3, 0x1.5fa3660000000p-3, -0x1.13762a0000000p-6, -0x1.b1ef640000000p-4, -0x1.b22d4c0000000p-3, -0x1.3d6aac0000000p-3, -0x1.7968e60000000p-3, -0x1.ef3c020000000p-3, -0x1.cc18a20000000p-3, -0x1.05dc4e0000000p-1, -0x1.964c9c0000000p-3, -0x1.17f97e0000000p-3, -0x1.20ac3e0000000p-3, -0x1.a51b2a0000000p-2, -0x1.13f0340000000p-2, -0x1.9d752e0000000p-3, -0x1.f75a700000000p-3, 0x1.417f0a0000000p-8, 0x1.1cb4140000000p-2, -0x1.d5f34c0000000p-4, 0x1.ef45680000000p-4, -0x1.a401480000000p-4, -0x1.26c2f40000000p-3, -0x1.7762280000000p-2, -0x1.ce837a0000000p-3, -0x1.2e85040000000p-2, -0x1.475a740000000p-7, -0x1.27d07c0000000p-4, -0x1.500ca20000000p-4, -0x1.ebfcdc0000000p-2, -0x1.dcd4060000000p-2, -0x1.3a16d40000000p-2}
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
