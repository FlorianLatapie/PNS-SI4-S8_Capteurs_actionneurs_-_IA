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