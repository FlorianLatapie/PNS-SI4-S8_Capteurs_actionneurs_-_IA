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