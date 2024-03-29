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