/**
  ******************************************************************************
  * @file    weights/conv.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#define INPUT_CHANNELS    64
#define CONV_FILTERS      32
#define CONV_KERNEL_SIZE  3


const int16_t conv1d_4_bias[CONV_FILTERS] = {3, 3, 13, 27, 3, 41, -7, -6, -5, -24, -1, 8, 7, -30, 0, -16, 17, -1, -1, -58, -2, 10, 16, -6, -22, -11, -28, -2, -4, 6, 10, 29}
;

const int16_t conv1d_4_kernel[CONV_FILTERS][INPUT_CHANNELS][CONV_KERNEL_SIZE] = {{{57, -34, -38}
, {-13, -64, 29}
, {12, 67, 33}
, {-13, -25, -61}
, {-78, -43, -60}
, {-54, -77, 56}
, {-5, 0, -69}
, {30, 31, -73}
, {78, -6, -54}
, {51, 28, -62}
, {-6, -39, 33}
, {86, 17, 48}
, {10, 13, -18}
, {-3, 27, 15}
, {0, 56, -16}
, {12, 69, -16}
, {13, 5, -16}
, {71, -44, -44}
, {-58, -60, 81}
, {-4, -25, 69}
, {-77, -18, 50}
, {42, -34, 63}
, {21, -43, -70}
, {-12, 27, 34}
, {-64, -71, 24}
, {-6, 56, -22}
, {-9, -21, 26}
, {20, 68, 65}
, {37, -65, 51}
, {72, -19, -37}
, {-17, -45, 49}
, {-26, -65, -88}
, {28, 93, 87}
, {-49, -26, -6}
, {-72, -61, -37}
, {40, 18, 43}
, {18, 4, -55}
, {15, 14, -20}
, {-45, -41, -54}
, {-28, 65, -34}
, {-52, -18, 59}
, {108, 42, -46}
, {-70, -46, -71}
, {-5, 2, 4}
, {-33, 47, 8}
, {-6, -1, -54}
, {-65, -53, 45}
, {-37, 69, 84}
, {8, 34, 35}
, {1, 15, -30}
, {68, 89, 7}
, {19, 74, 60}
, {72, -19, 89}
, {67, 0, 17}
, {52, 63, -14}
, {-52, 35, 46}
, {3, 79, 3}
, {102, 81, 62}
, {-64, 78, 84}
, {-10, 26, -33}
, {44, 71, -16}
, {-6, -76, 16}
, {45, 20, 30}
, {-22, 98, 95}
}
, {{-56, -2, -35}
, {-55, 75, -1}
, {-30, 35, 1}
, {-6, -100, -109}
, {-37, 71, -17}
, {-113, -39, -4}
, {6, -49, 5}
, {36, 29, -85}
, {-56, 60, 66}
, {-48, 26, -65}
, {6, 55, 57}
, {4, 83, -44}
, {0, 35, 33}
, {48, -17, -73}
, {59, 60, 62}
, {13, -50, 49}
, {-71, 9, 0}
, {-48, -13, -88}
, {72, 14, 2}
, {-72, 13, 28}
, {-71, -18, 16}
, {-17, -68, -49}
, {32, 47, 70}
, {-20, -14, -23}
, {-18, -30, -88}
, {-42, 38, 53}
, {64, -31, -43}
, {45, -26, 39}
, {-34, -13, 2}
, {-45, -40, 72}
, {-57, -39, 24}
, {54, 47, -77}
, {1, 3, 83}
, {64, 48, 40}
, {18, -76, -4}
, {52, -12, -70}
, {-11, -16, -56}
, {34, -65, -23}
, {-51, -45, 54}
, {-4, -20, -20}
, {-14, -6, 52}
, {-71, -11, -9}
, {-22, -76, -17}
, {65, -36, 4}
, {49, 60, 1}
, {-3, 29, 43}
, {-9, 9, 56}
, {-8, 73, -67}
, {0, -49, 59}
, {6, -62, 27}
, {-85, -16, 53}
, {58, 57, -36}
, {-28, 42, 7}
, {15, 69, -14}
, {56, 0, 79}
, {-36, -75, -24}
, {68, 9, -44}
, {82, 39, -46}
, {37, 55, 11}
, {-45, 15, -19}
, {32, 51, 61}
, {-16, -23, 17}
, {-71, -6, -47}
, {2, -26, -22}
}
, {{-73, -58, -13}
, {-65, -31, 24}
, {-73, 47, 61}
, {0, 14, 41}
, {11, -54, -6}
, {-71, -48, -23}
, {9, 64, -18}
, {-11, -47, -94}
, {39, -56, -31}
, {-65, -8, -34}
, {34, -78, 35}
, {-64, -91, -6}
, {44, -39, 51}
, {-58, -62, -30}
, {-14, 75, 58}
, {-39, -14, 26}
, {65, -15, 43}
, {-7, -45, 58}
, {-90, -3, -90}
, {3, -59, 63}
, {0, 33, 49}
, {-6, -35, 19}
, {-19, 40, 23}
, {-48, -61, 21}
, {-23, -75, 21}
, {9, -24, 2}
, {-32, -3, 5}
, {-67, 0, -10}
, {-86, -55, -68}
, {-59, -38, -10}
, {-39, -58, -46}
, {-57, -78, -19}
, {20, 62, -7}
, {83, 38, -1}
, {20, -19, 61}
, {3, -57, -24}
, {-58, -21, -85}
, {-81, 42, -31}
, {-19, -114, -93}
, {46, 31, -51}
, {-46, 58, 76}
, {38, -45, 48}
, {-38, 6, -25}
, {29, -46, 30}
, {-93, -81, -28}
, {51, 43, 41}
, {-83, 0, -38}
, {-30, -80, -10}
, {-53, -54, 42}
, {49, 13, 53}
, {91, 78, 72}
, {21, -74, 3}
, {-55, -31, 49}
, {-87, 29, -75}
, {-61, -100, 28}
, {-60, 2, -45}
, {-82, 0, -57}
, {-66, -47, 13}
, {-53, 1, -37}
, {-37, 54, -27}
, {-25, -56, -83}
, {48, -65, -72}
, {-34, 67, 68}
, {-71, -48, -44}
}
, {{-43, -72, 42}
, {61, 25, 58}
, {-9, -50, -44}
, {-32, -83, -60}
, {1, -47, -36}
, {-69, -37, -57}
, {0, -30, -49}
, {24, 2, -4}
, {-31, 46, 67}
, {39, -53, -24}
, {35, -9, 16}
, {47, 30, -47}
, {77, -39, 18}
, {-30, -71, -8}
, {45, 15, 55}
, {-22, -22, -50}
, {-78, -4, -62}
, {41, -53, -53}
, {82, 12, 22}
, {6, -50, -8}
, {13, 56, 6}
, {-8, -51, -80}
, {0, 8, 36}
, {36, 56, 16}
, {59, 24, -48}
, {-57, -84, -37}
, {-4, 54, -55}
, {-57, 2, -31}
, {70, 79, 48}
, {48, 23, 82}
, {-44, -71, 40}
, {21, 22, -46}
, {40, 11, -24}
, {48, 23, 70}
, {47, 39, 26}
, {16, 16, -16}
, {17, -30, -50}
, {15, -19, -77}
, {61, 34, 66}
, {-45, 35, 11}
, {40, -5, -40}
, {37, 51, 28}
, {50, 37, -43}
, {46, 11, 40}
, {-14, 34, -43}
, {46, 42, -66}
, {-63, 6, -24}
, {46, -17, -25}
, {-26, -14, -66}
, {16, -42, -23}
, {-30, -63, -56}
, {7, 6, 41}
, {-23, -23, 40}
, {55, -63, -2}
, {-48, -30, 75}
, {-31, -47, -7}
, {-3, -7, -37}
, {-43, 17, 43}
, {-26, 75, 68}
, {-14, -35, 62}
, {-39, -54, -66}
, {8, -32, -36}
, {6, 44, -53}
, {0, -11, 68}
}
, {{-7, -9, 5}
, {21, -25, -24}
, {-28, -79, 30}
, {-44, 62, 38}
, {-55, -18, -69}
, {-47, -16, 31}
, {-37, 15, -5}
, {-38, 19, -77}
, {-69, -62, 52}
, {20, 53, 26}
, {-44, 14, -41}
, {17, -23, -7}
, {-97, -82, 6}
, {-34, 16, 22}
, {-80, -6, -26}
, {-26, 50, 25}
, {6, -60, -46}
, {-15, 56, 20}
, {11, 68, -22}
, {-30, 6, -42}
, {28, -53, -24}
, {86, -35, -62}
, {-21, 45, 48}
, {36, 58, 20}
, {22, 11, -19}
, {-60, 60, -34}
, {25, 67, 37}
, {-47, 5, -61}
, {62, 0, -28}
, {-87, -69, 35}
, {-30, 30, 53}
, {-52, 17, 27}
, {52, 33, -33}
, {-63, 0, -13}
, {-8, 6, 3}
, {44, -66, 45}
, {-49, 64, -46}
, {-35, 39, -45}
, {-29, 14, 21}
, {32, 62, 38}
, {-80, 13, -16}
, {-35, -25, -35}
, {-21, 15, 56}
, {0, 55, -14}
, {-13, 24, 47}
, {13, 48, -44}
, {-17, 43, 21}
, {-57, -63, -43}
, {23, 60, -5}
, {23, -50, 42}
, {75, 85, -18}
, {-9, -95, -49}
, {-2, -73, 19}
, {0, -43, 38}
, {21, 44, 63}
, {-4, -7, 16}
, {-50, -51, -32}
, {-60, 20, -96}
, {32, -77, 12}
, {-5, -70, -22}
, {-32, 59, -72}
, {11, -42, -7}
, {33, -29, -74}
, {-70, 42, 21}
}
, {{2, -67, -90}
, {0, 73, 79}
, {63, -24, 53}
, {-41, 41, 33}
, {-18, -47, 52}
, {-90, -91, -112}
, {22, -17, -26}
, {-55, -14, 48}
, {38, -86, -26}
, {-16, -104, 31}
, {-81, 45, -47}
, {-52, 47, -78}
, {39, 40, -34}
, {-108, 10, -6}
, {-17, 60, 17}
, {28, 9, 65}
, {-45, -48, -69}
, {-83, -101, -79}
, {32, -29, 17}
, {-55, 59, 23}
, {-106, -48, -104}
, {-58, 19, -61}
, {-43, -24, -19}
, {-22, 31, 83}
, {28, -29, -56}
, {-71, 2, -10}
, {60, 41, -28}
, {-2, -24, -15}
, {-18, -7, 17}
, {-38, -3, -45}
, {0, -52, -11}
, {-80, -18, -41}
, {0, -58, 26}
, {-17, 43, -3}
, {50, -1, 69}
, {62, 79, -20}
, {-73, -83, -33}
, {-65, -62, -9}
, {-3, 2, -45}
, {12, -56, -32}
, {71, 39, -3}
, {11, 32, 22}
, {49, -39, -13}
, {-24, -51, 62}
, {0, -34, -11}
, {-36, -21, -65}
, {-7, -26, 54}
, {15, -22, 16}
, {-13, -6, 1}
, {-42, 7, -14}
, {-28, -1, -76}
, {18, 31, -21}
, {-19, 58, -27}
, {20, 10, -41}
, {19, 68, 52}
, {-49, 31, 1}
, {37, 50, 19}
, {-13, -40, 45}
, {15, 37, -87}
, {18, 74, 70}
, {-74, -88, -49}
, {-38, -80, -85}
, {-32, 44, 40}
, {13, 64, 9}
}
, {{-58, -54, -31}
, {37, -6, -53}
, {-40, 28, -40}
, {-58, -84, -35}
, {-19, -6, 60}
, {56, -69, -40}
, {-28, 0, -40}
, {17, 25, 19}
, {9, 17, -13}
, {28, -60, 15}
, {-48, 20, 62}
, {43, -49, -19}
, {-22, 3, -65}
, {4, -19, 47}
, {-22, -93, -71}
, {-35, -40, 45}
, {-63, -92, 19}
, {19, -46, 66}
, {-20, -66, 0}
, {20, -19, 44}
, {-59, -82, -85}
, {55, -10, 23}
, {48, 25, -46}
, {-20, 53, 81}
, {45, -33, 4}
, {-35, -90, -44}
, {48, -42, -65}
, {-76, 56, -11}
, {6, 10, 18}
, {-20, -25, 11}
, {-18, 0, -15}
, {-26, -18, 16}
, {55, -23, -21}
, {-59, -24, -68}
, {-59, 22, 54}
, {31, 33, 7}
, {-45, -74, -10}
, {-58, 42, -39}
, {-51, 19, 0}
, {-66, 25, -75}
, {-15, 47, -38}
, {19, 47, 17}
, {36, 1, -46}
, {21, 65, -29}
, {3, 0, -46}
, {-93, 8, 23}
, {-10, 33, 13}
, {11, -43, -43}
, {-20, -12, -76}
, {-22, -78, -68}
, {23, -29, -26}
, {7, 33, 29}
, {71, -27, 27}
, {-52, -12, 40}
, {-52, 11, -8}
, {58, -52, 35}
, {-70, -58, 2}
, {-48, -68, -13}
, {-53, -17, 29}
, {42, 60, 61}
, {18, -34, 9}
, {-16, 56, 30}
, {1, 4, -57}
, {24, -34, -80}
}
, {{-1, 61, 16}
, {70, -60, 34}
, {-37, 38, -60}
, {-13, -56, -43}
, {-57, 37, -73}
, {-46, -49, 46}
, {25, -69, -64}
, {7, -24, -45}
, {23, -24, 12}
, {34, -60, 3}
, {0, -4, -22}
, {-60, -62, 37}
, {54, -74, -29}
, {57, 68, -62}
, {17, 51, 5}
, {-47, -78, -45}
, {7, -44, 37}
, {36, 20, -31}
, {22, -19, -43}
, {30, 55, -72}
, {-34, -27, 9}
, {22, 37, -19}
, {57, -75, 8}
, {-56, -8, 24}
, {20, -54, 21}
, {6, -53, -21}
, {47, 13, -39}
, {-2, -63, 30}
, {-48, 25, -44}
, {45, 5, -9}
, {58, -29, 1}
, {-8, -55, -49}
, {10, 48, -47}
, {-63, -47, -67}
, {-21, -45, -18}
, {-15, -10, 3}
, {-49, -9, -16}
, {-37, -31, -69}
, {11, -3, -23}
, {-25, 50, -50}
, {-62, 57, -24}
, {4, 20, -18}
, {14, 11, -57}
, {44, 5, -34}
, {-34, 7, -38}
, {-79, 57, 8}
, {-10, 15, -53}
, {17, -7, -48}
, {24, -51, -30}
, {-2, -15, -10}
, {-56, 55, 36}
, {-2, -11, -5}
, {-35, -69, 56}
, {-44, 12, 60}
, {-57, 4, 22}
, {-31, -25, 50}
, {-62, -24, -10}
, {-52, -60, -30}
, {0, 14, 12}
, {-10, -49, 33}
, {57, 43, 47}
, {-15, 29, 38}
, {-53, -12, 1}
, {64, 17, 0}
}
, {{-32, -17, 10}
, {12, 76, -43}
, {64, -68, 10}
, {-38, 49, -55}
, {-28, 78, -26}
, {-55, 60, -37}
, {8, -60, -43}
, {63, -32, -26}
, {86, -51, -2}
, {-69, 44, 18}
, {71, 35, 90}
, {-49, 77, -43}
, {86, 20, 46}
, {53, 35, 32}
, {-41, -2, 69}
, {-24, -3, 30}
, {-17, 51, 22}
, {32, 72, 5}
, {-30, -41, 39}
, {42, -28, 11}
, {62, -49, -47}
, {-12, -50, 37}
, {-65, -88, -46}
, {48, -10, -62}
, {58, -24, -21}
, {-8, 41, -42}
, {62, 20, -45}
, {-58, 60, 82}
, {-32, -24, -34}
, {23, -43, 80}
, {0, 9, 38}
, {34, 65, 10}
, {-30, -4, 65}
, {-24, -2, 69}
, {-80, -69, 36}
, {-52, 71, 47}
, {37, 0, -31}
, {-49, 45, 6}
, {-47, 54, -10}
, {45, -63, -54}
, {36, -31, 63}
, {-48, 57, 15}
, {-83, -94, 48}
, {66, 13, 99}
, {69, -49, 78}
, {-41, -3, -20}
, {-63, 60, 78}
, {6, 16, 97}
, {-68, 24, -46}
, {43, -9, -29}
, {-70, -31, -25}
, {71, -44, 63}
, {38, -19, -13}
, {66, 69, 8}
, {-27, 83, 82}
, {-2, 14, 42}
, {79, 64, 70}
, {32, -83, -8}
, {-50, -49, 55}
, {111, 23, 56}
, {35, 14, 16}
, {27, -76, 34}
, {-34, -67, 38}
, {5, 5, 12}
}
, {{-11, -68, -19}
, {-52, -70, -70}
, {-42, 40, 50}
, {-70, -38, 18}
, {-2, 43, 0}
, {2, 52, -15}
, {25, 5, 14}
, {7, 15, 33}
, {12, 53, 27}
, {56, 72, 61}
, {44, -38, 43}
, {-63, 12, -7}
, {-48, 56, -65}
, {18, 15, 32}
, {13, 57, 61}
, {-22, -2, -43}
, {35, -24, 18}
, {-22, -53, -9}
, {-79, -72, -25}
, {0, -59, -35}
, {68, 18, 0}
, {-39, -60, -39}
, {-45, 62, 24}
, {-42, 25, -23}
, {-51, -42, 45}
, {20, -16, 43}
, {2, 19, -38}
, {-60, -13, 0}
, {-18, -4, -25}
, {-56, -70, 23}
, {29, 64, -56}
, {0, -13, -51}
, {-16, 5, -48}
, {-22, -12, -12}
, {-72, 24, -37}
, {2, 39, -57}
, {-78, -41, -35}
, {-53, 14, 60}
, {-14, -2, 58}
, {-8, 6, 3}
, {-46, -15, -69}
, {-60, 43, 2}
, {19, 50, -33}
, {-13, -67, -67}
, {53, -11, 21}
, {10, -42, -69}
, {-74, 54, -59}
, {-10, -33, -45}
, {-36, -61, 54}
, {21, -73, 51}
, {74, 60, -14}
, {-71, -60, 21}
, {-9, -53, -14}
, {51, 22, -27}
, {54, -34, 52}
, {50, -11, -35}
, {35, -28, 61}
, {7, -28, -87}
, {7, 14, -35}
, {0, -69, -81}
, {30, -19, 0}
, {12, 12, -66}
, {35, -36, -27}
, {-50, -18, 34}
}
, {{48, 43, 19}
, {36, -61, -1}
, {85, 54, 46}
, {-40, 39, 12}
, {29, -9, 3}
, {-1, -22, 11}
, {39, -24, 28}
, {66, 83, -63}
, {2, 50, -20}
, {-53, -8, 37}
, {92, -32, 66}
, {49, 43, 91}
, {68, 35, 18}
, {0, -49, -64}
, {17, 41, 59}
, {34, 80, -13}
, {50, 25, -76}
, {23, 15, -17}
, {10, 53, -74}
, {18, -64, -63}
, {70, 14, 50}
, {-83, -85, -70}
, {-55, 24, -1}
, {80, 98, -12}
, {-88, -41, -25}
, {18, -26, 14}
, {-56, 70, -9}
, {49, -22, 84}
, {-21, -35, -35}
, {0, 63, 58}
, {28, 13, 49}
, {-6, 3, 22}
, {84, 47, 72}
, {-60, 53, -32}
, {-58, 3, -47}
, {-26, 30, 15}
, {14, 13, 38}
, {64, 13, -50}
, {-49, -49, -10}
, {62, 79, -31}
, {-45, 19, 51}
, {-54, -30, -36}
, {-62, -93, -82}
, {-44, 75, 93}
, {91, 68, -43}
, {-45, -29, 30}
, {-58, 29, 89}
, {16, 94, 61}
, {-39, -35, -23}
, {-59, 80, 48}
, {-85, -82, -58}
, {-8, 80, 54}
, {55, -48, -29}
, {-28, 0, -43}
, {67, 90, -46}
, {-29, -11, -29}
, {41, -3, -1}
, {-54, -8, 91}
, {14, -14, 66}
, {-27, 104, -19}
, {29, -57, 55}
, {-59, 33, -10}
, {-5, -48, -32}
, {30, 99, 16}
}
, {{-41, 42, -28}
, {-62, 10, 28}
, {-6, 19, -22}
, {-1, -6, 30}
, {-26, 26, 18}
, {26, 36, 7}
, {33, -20, 16}
, {0, 36, -35}
, {-73, 54, -19}
, {65, -2, -16}
, {68, 25, 30}
, {72, 46, -66}
, {23, -9, 30}
, {-28, -47, -14}
, {-14, 52, -8}
, {74, 27, 0}
, {-52, 31, 46}
, {37, -20, -5}
, {-64, -72, -37}
, {-25, 41, -11}
, {-67, -2, 0}
, {5, -27, -18}
, {-63, 9, -77}
, {-50, -80, -85}
, {0, -50, -52}
, {17, 40, 52}
, {-7, 22, -44}
, {32, 29, -15}
, {48, 57, 20}
, {-42, 22, -1}
, {-11, -52, -45}
, {-73, -13, -73}
, {-41, 5, 59}
, {-45, -39, -42}
, {61, 29, 16}
, {6, 49, 42}
, {-63, -67, -16}
, {-9, 71, 0}
, {-58, -52, 68}
, {27, -40, -28}
, {55, -35, -15}
, {42, 10, -50}
, {36, 2, -13}
, {-30, -61, -56}
, {-69, 43, -32}
, {52, -48, 63}
, {-24, -45, 67}
, {13, 10, -58}
, {-9, 50, -12}
, {-14, 58, 51}
, {94, 61, 52}
, {-64, -102, 2}
, {-14, -36, 8}
, {56, -50, -44}
, {14, -46, 38}
, {-14, -61, 17}
, {61, 43, -30}
, {8, 38, -12}
, {59, 15, -64}
, {-71, 14, -72}
, {43, 53, -3}
, {77, 49, 67}
, {18, 73, -44}
, {46, -44, 40}
}
, {{-23, 15, 11}
, {48, -49, -25}
, {-7, 58, 35}
, {27, -54, 21}
, {-26, -17, 43}
, {30, -31, 8}
, {-36, -3, -21}
, {54, 19, -47}
, {-74, -72, 38}
, {55, -27, 21}
, {21, 0, -6}
, {-8, 47, -22}
, {-5, 66, 20}
, {-5, -3, -16}
, {-11, -10, 45}
, {10, 32, -66}
, {-45, -17, 13}
, {-34, -57, 11}
, {-58, -60, -56}
, {18, 27, -19}
, {-52, -74, 44}
, {0, 16, -7}
, {-13, 38, 17}
, {-10, -71, -86}
, {53, -31, -25}
, {-23, 51, -26}
, {8, 28, -26}
, {23, -38, 72}
, {-7, -4, 5}
, {3, 10, -64}
, {-63, 40, -74}
, {18, 22, -70}
, {-71, -71, -2}
, {65, -8, 48}
, {48, 39, 45}
, {28, -14, 35}
, {-67, 27, 32}
, {-61, -44, -33}
, {-80, -10, -40}
, {-26, -74, 39}
, {19, -66, -2}
, {-38, 48, -72}
, {-56, -2, 63}
, {-23, -72, -78}
, {-62, -39, -38}
, {31, 43, 36}
, {-73, 47, 30}
, {23, -41, 54}
, {-70, 69, 56}
, {-15, -53, 32}
, {-50, 56, 25}
, {49, -63, 60}
, {35, 37, 49}
, {29, 14, -53}
, {-28, -27, 16}
, {-74, -56, -82}
, {-19, -8, -24}
, {-53, -49, 66}
, {-51, -36, 79}
, {-32, -79, 58}
, {24, 24, -48}
, {74, -29, -33}
, {28, 72, 17}
, {-28, -46, -42}
}
, {{-32, -28, 61}
, {70, -76, 48}
, {82, 13, -47}
, {-11, -29, 17}
, {-11, 3, 5}
, {0, -46, 47}
, {-21, -43, -24}
, {51, -72, 41}
, {-64, -6, -7}
, {-72, 47, -62}
, {51, 52, -12}
, {23, 33, -33}
, {-31, -2, 34}
, {-60, 11, 2}
, {60, 54, -15}
, {-22, -41, -35}
, {0, -23, 43}
, {-58, 2, 9}
, {27, -3, 9}
, {13, -31, 69}
, {16, -26, 29}
, {-88, -55, -49}
, {63, -28, 38}
, {-36, -59, -23}
, {27, -5, -29}
, {69, -49, 41}
, {12, -25, 33}
, {27, -33, 67}
, {-76, 36, -82}
, {76, -54, 45}
, {61, -73, -21}
, {61, 26, 48}
, {-36, -50, -39}
, {-6, 39, 50}
, {-68, 0, 21}
, {51, -46, 9}
, {18, 22, 69}
, {-9, -18, -52}
, {30, -68, -42}
, {-4, 4, 1}
, {14, -27, -15}
, {-35, -7, 54}
, {-64, 56, 14}
, {0, -80, 18}
, {-40, 48, 19}
, {-9, 48, 44}
, {49, -50, -46}
, {-3, -47, -9}
, {-89, 23, -9}
, {70, -30, 49}
, {-13, -74, -81}
, {61, 28, 38}
, {44, 60, -55}
, {16, -80, -58}
, {-50, 1, 38}
, {22, -18, 43}
, {68, -11, -12}
, {-15, -5, -32}
, {58, 0, 10}
, {-34, 21, 72}
, {4, -51, 36}
, {-60, -13, 64}
, {-5, -8, 57}
, {-32, 24, -10}
}
, {{-40, -49, 26}
, {2, -11, 75}
, {48, 16, 64}
, {-46, -64, -52}
, {8, -57, 29}
, {8, 8, -11}
, {18, 6, 4}
, {10, -18, 42}
, {12, -32, 19}
, {-23, -24, 86}
, {81, -49, -43}
, {44, 42, 77}
, {24, 44, -56}
, {29, 21, 17}
, {0, -44, -5}
, {-69, -1, -40}
, {31, 42, 52}
, {4, -15, -3}
, {57, -1, 42}
, {12, -52, -19}
, {16, 51, -4}
, {21, -78, -6}
, {21, -72, -22}
, {58, 79, 33}
, {-9, -1, 33}
, {41, 15, 11}
, {-12, -77, -87}
, {77, -61, 40}
, {-47, 1, -25}
, {-47, 43, 29}
, {-43, -17, -5}
, {41, 0, 58}
, {27, 28, 65}
, {54, 32, 10}
, {47, 28, -41}
, {41, -45, 5}
, {0, -63, -25}
, {72, 23, -40}
, {-6, -50, -43}
, {-54, -56, 13}
, {39, -22, 64}
, {5, 52, -53}
, {34, -75, 28}
, {72, 11, -5}
, {71, 66, 0}
, {-35, -69, 29}
, {-68, 8, -27}
, {16, 87, -5}
, {1, 34, 68}
, {56, -39, 26}
, {-19, 9, -25}
, {0, 52, 32}
, {43, 57, -42}
, {-2, -70, -3}
, {40, -68, -33}
, {-30, 12, 28}
, {-40, 9, 37}
, {-4, 25, 26}
, {5, -49, 33}
, {14, 101, 85}
, {5, 61, 11}
, {-54, 76, 62}
, {-16, 33, -9}
, {33, 5, 102}
}
, {{-47, 42, -1}
, {-41, -72, 51}
, {-65, 46, -51}
, {34, 19, 39}
, {-31, -50, 48}
, {33, 15, 8}
, {-48, -24, 12}
, {40, -22, 20}
, {18, 48, 27}
, {-74, 34, 42}
, {20, -52, 47}
, {4, -49, 13}
, {51, -20, -9}
, {-9, -22, -61}
, {-40, 31, -75}
, {73, -9, 1}
, {11, -23, -39}
, {-65, -69, 56}
, {14, -9, 67}
, {-10, 51, -79}
, {-19, -12, 2}
, {86, 77, -52}
, {-15, 54, -74}
, {-1, -73, 47}
, {-46, -14, -17}
, {-19, -21, -5}
, {14, -67, 16}
, {61, 2, 19}
, {-65, -70, 53}
, {31, -7, 48}
, {11, -55, 8}
, {-14, -46, -16}
, {18, -63, 6}
, {-11, -61, 17}
, {41, -62, 69}
, {47, 2, 3}
, {34, -45, -70}
, {-67, -1, -11}
, {65, 58, -12}
, {12, -67, 65}
, {-29, -30, -45}
, {-25, 6, -65}
, {-35, -52, -59}
, {-44, -68, 25}
, {-69, 20, -15}
, {0, -29, 60}
, {-40, 25, -62}
, {24, 37, -71}
, {80, -38, 7}
, {-12, -18, 60}
, {-12, -71, -21}
, {-22, 54, 9}
, {10, 61, 11}
, {-9, -56, -45}
, {-13, 27, 40}
, {36, 40, 40}
, {64, -79, 55}
, {60, 16, 19}
, {-52, 19, 40}
, {49, -68, -75}
, {-42, -23, -10}
, {-3, 0, -70}
, {-61, 32, -37}
, {-24, -67, 60}
}
, {{55, 46, -6}
, {-55, -73, -74}
, {-48, -41, 55}
, {50, 56, 23}
, {1, -7, -71}
, {-15, -48, 37}
, {-51, 44, -9}
, {72, 8, 45}
, {-29, 51, -27}
, {-26, -6, 38}
, {-8, -41, -38}
, {30, -50, -34}
, {79, 81, 6}
, {-30, -37, 47}
, {-59, 41, -49}
, {44, 56, 47}
, {-11, 0, 4}
, {-6, -36, 0}
, {-24, -41, 9}
, {38, -28, 61}
, {-44, -22, -54}
, {-3, 2, -98}
, {-62, -15, -76}
, {-75, -45, -48}
, {-74, -73, -3}
, {34, 33, -2}
, {123, 67, 76}
, {-32, -77, -31}
, {-110, -85, -38}
, {-44, -22, -65}
, {-39, -24, -23}
, {-61, 49, -63}
, {30, 44, -61}
, {-19, -66, -11}
, {-34, 19, -89}
, {-25, 39, 28}
, {-4, -5, 21}
, {-4, 9, 28}
, {27, 0, -25}
, {-12, -15, -40}
, {14, -27, -15}
, {-5, 4, 68}
, {-82, -82, 44}
, {17, -77, -70}
, {60, 40, 14}
, {2, -51, -31}
, {46, -54, -3}
, {-70, 22, -92}
, {57, -45, -54}
, {57, 14, -39}
, {29, 18, 96}
, {2, -1, -54}
, {70, -34, 64}
, {-33, 74, 80}
, {-50, -31, -39}
, {-23, -40, -21}
, {50, -79, -57}
, {-27, 21, -34}
, {4, -67, -4}
, {47, -53, 18}
, {43, 23, 0}
, {-28, 3, 13}
, {22, -15, 49}
, {0, -45, -23}
}
, {{-16, -44, 36}
, {74, 49, -38}
, {31, 84, 46}
, {-60, -1, 15}
, {-9, 22, 3}
, {-20, 69, 6}
, {17, 34, -61}
, {76, -35, 65}
, {-54, -53, -6}
, {-44, 61, 19}
, {-33, 8, 22}
, {20, -10, -31}
, {94, 73, 37}
, {-67, -15, 45}
, {-31, -57, 15}
, {-45, 41, 80}
, {55, 30, -65}
, {-75, -50, 64}
, {-74, 32, 65}
, {0, -64, 30}
, {-59, -12, 30}
, {-7, 69, 29}
, {-7, -68, -26}
, {83, 43, -52}
, {2, -64, -97}
, {-43, 29, 8}
, {-66, -8, 53}
, {11, 2, 65}
, {-65, -81, 62}
, {-41, -15, 81}
, {-58, -24, -6}
, {-64, 32, 35}
, {-35, 62, -37}
, {70, 37, 91}
, {-94, -53, -2}
, {39, -84, 6}
, {42, -75, 34}
, {6, 15, -14}
, {-2, -94, 38}
, {-33, 34, -39}
, {79, -26, 53}
, {-60, 7, -5}
, {-12, 6, -6}
, {-33, -41, 89}
, {45, 23, 61}
, {-73, 50, 10}
, {61, 23, -41}
, {-57, 20, -17}
, {-11, -91, -104}
, {65, -65, -4}
, {10, -72, -35}
, {31, -47, 65}
, {29, -49, -48}
, {-10, 15, 96}
, {82, -6, -69}
, {-9, -36, -50}
, {57, 11, 40}
, {26, -9, -60}
, {-47, 24, 3}
, {8, -4, -43}
, {-65, 25, -64}
, {70, 0, -7}
, {-31, 12, 47}
, {86, 36, 95}
}
, {{-34, 0, -35}
, {-41, -75, 46}
, {-35, -28, -83}
, {-7, 0, 5}
, {10, 19, -12}
, {-66, -16, 32}
, {-47, -69, -44}
, {40, -59, -28}
, {-41, -65, 0}
, {13, -51, -52}
, {-23, -61, 28}
, {-60, -45, 60}
, {62, 66, -22}
, {-82, 24, -25}
, {29, 0, -49}
, {-45, -10, -13}
, {-41, 53, 47}
, {40, -19, -61}
, {20, -5, 20}
, {36, 19, -7}
, {-14, -14, -77}
, {40, 8, 9}
, {-54, -53, -18}
, {-46, -31, -47}
, {46, 7, 26}
, {-43, -69, 0}
, {-56, -17, 27}
, {19, -61, -34}
, {64, 74, 35}
, {-55, 64, -37}
, {-61, 51, -40}
, {-66, -42, -7}
, {-39, -76, -60}
, {30, 30, 0}
, {30, 32, 53}
, {24, 64, 42}
, {-46, 3, 33}
, {15, -52, 12}
, {50, -53, -8}
, {61, -67, 32}
, {-57, 64, -50}
, {10, 46, 49}
, {-10, -13, 27}
, {9, 50, -53}
, {52, -64, 47}
, {-85, -63, 26}
, {-21, -55, 59}
, {23, -12, 23}
, {-6, -53, 31}
, {46, 6, 48}
, {-75, 14, -59}
, {-54, -30, 15}
, {-50, 49, -21}
, {57, 54, 12}
, {-8, 34, -56}
, {68, -1, 68}
, {-38, -87, -37}
, {-69, -79, -25}
, {49, 54, -30}
, {-20, 42, -12}
, {8, -69, -60}
, {6, -58, -12}
, {-82, 22, 28}
, {-72, -88, -36}
}
, {{66, -9, -62}
, {32, -59, -3}
, {-65, 23, -80}
, {-32, 4, 59}
, {-27, 21, -48}
, {-46, -38, 30}
, {2, 35, 12}
, {59, -40, -7}
, {42, -44, -40}
, {-70, 12, -76}
, {35, -22, -19}
, {-39, -57, 22}
, {17, -60, 57}
, {-6, 26, -19}
, {47, -1, 9}
, {-5, 51, 43}
, {-43, -44, -32}
, {21, 65, 37}
, {-68, 11, -67}
, {58, 4, 16}
, {-31, -20, -9}
, {71, 24, -23}
, {11, 9, -18}
, {-33, -57, 59}
, {-19, 3, 79}
, {-7, -60, 63}
, {0, 52, 54}
, {-60, -29, 92}
, {64, -63, 51}
, {-30, 46, 27}
, {47, -27, 3}
, {-9, -8, -28}
, {61, 28, -13}
, {-63, 69, 42}
, {-37, 69, 3}
, {7, 17, 15}
, {-60, -54, 68}
, {62, -73, 65}
, {-54, 16, -16}
, {-24, 57, 53}
, {17, -15, -39}
, {-45, 59, -76}
, {8, 51, 16}
, {-16, -46, 38}
, {29, 13, 11}
, {-56, 62, 0}
, {55, 2, -20}
, {50, 58, -48}
, {53, 38, -6}
, {73, -50, 64}
, {25, -48, 35}
, {-57, -63, -52}
, {-59, -55, -34}
, {-8, -43, 80}
, {-28, -73, 33}
, {-29, 21, -74}
, {19, 66, -59}
, {14, 5, -70}
, {-43, 6, -38}
, {-50, -20, 22}
, {62, -15, -48}
, {-65, 13, 4}
, {40, -49, 72}
, {72, 69, -56}
}
, {{27, -48, -11}
, {61, 12, 20}
, {63, -50, 78}
, {-12, 21, -8}
, {-62, -22, 58}
, {-28, 39, -48}
, {-17, 59, -19}
, {-4, 35, 66}
, {-31, 72, 52}
, {8, -58, -76}
, {25, -37, 13}
, {16, 19, -10}
, {26, -19, -11}
, {-28, 16, -55}
, {69, -77, 87}
, {57, 50, -22}
, {68, -15, -37}
, {-5, 21, -55}
, {-56, 49, -15}
, {47, 33, -17}
, {-80, 2, 15}
, {11, 54, 40}
, {48, -37, -41}
, {-61, -6, 47}
, {-28, 61, 27}
, {19, 17, -42}
, {-6, -11, -31}
, {36, -27, 21}
, {38, -60, -63}
, {88, 57, -12}
, {34, 16, 60}
, {57, -9, -66}
, {-27, -4, 43}
, {23, 51, 15}
, {44, 6, 9}
, {-44, -63, 16}
, {-42, -66, -54}
, {-20, 56, -55}
, {-46, -23, 54}
, {11, -48, 5}
, {4, -19, 13}
, {9, 69, 16}
, {0, -64, -21}
, {54, 23, 105}
, {48, 36, -67}
, {-32, -3, 53}
, {39, -36, 81}
, {44, 35, -43}
, {46, -51, -2}
, {-9, -10, 6}
, {-58, -29, -13}
, {1, 10, 50}
, {35, -8, -38}
, {41, -51, 56}
, {44, 4, -9}
, {8, -71, 55}
, {65, 54, -38}
, {91, 41, -59}
, {-16, -37, -36}
, {71, -53, -51}
, {-75, 42, -32}
, {-56, -71, -29}
, {59, -4, 0}
, {-6, 90, 24}
}
, {{-72, 67, 21}
, {-50, -20, -31}
, {67, 13, 93}
, {-6, 45, 12}
, {-14, 61, -46}
, {-45, -77, 19}
, {7, 2, 3}
, {18, 11, -67}
, {-15, -14, -19}
, {-35, -14, 77}
, {35, 72, -60}
, {9, -25, -15}
, {-37, 65, -35}
, {-34, 59, -14}
, {82, -54, -5}
, {-5, 1, -72}
, {1, 93, -43}
, {-71, 13, 73}
, {-15, -12, 28}
, {55, -33, 24}
, {-55, -54, -71}
, {49, -42, -45}
, {43, -25, -37}
, {44, -57, -43}
, {-29, 56, -31}
, {-14, 69, -4}
, {63, 31, -41}
, {75, 5, 11}
, {35, 3, -4}
, {-43, -4, 32}
, {48, 25, -74}
, {10, 12, -59}
, {66, -31, 81}
, {-65, -60, 59}
, {-28, 51, -15}
, {26, 81, 19}
, {21, 44, -43}
, {-35, 74, -34}
, {48, -23, -35}
, {-64, -9, 49}
, {91, -38, 90}
, {21, -10, -38}
, {-63, -29, 23}
, {54, 10, 32}
, {1, 47, 30}
, {-18, 46, -56}
, {11, 28, 77}
, {8, 60, 24}
, {53, 20, -22}
, {-11, 10, 94}
, {38, 53, -31}
, {-2, 48, 68}
, {16, 19, 63}
, {-62, -20, -18}
, {-63, -43, -58}
, {88, 5, -58}
, {-20, 86, -3}
, {48, -50, 48}
, {63, 51, -43}
, {60, 38, 49}
, {41, 14, -55}
, {-17, 24, -57}
, {-28, -27, -7}
, {-22, -18, 30}
}
, {{-35, -25, -39}
, {70, 22, 76}
, {7, 47, 3}
, {22, 42, 48}
, {-43, -40, 20}
, {-56, 68, -72}
, {-70, -27, 66}
, {-55, -12, -21}
, {-43, -65, 50}
, {-8, 17, 11}
, {-15, 37, 12}
, {25, 15, 49}
, {76, -4, 76}
, {-20, -15, -33}
, {45, 22, 48}
, {-10, 17, -72}
, {-63, -6, 24}
, {-43, 41, -19}
, {39, 71, -42}
, {61, 26, -61}
, {-53, -7, 43}
, {-40, 19, 39}
, {67, 38, 0}
, {-50, -26, -15}
, {-28, -15, -18}
, {-35, 54, -19}
, {-7, -60, 13}
, {-46, 38, 63}
, {-17, -82, 1}
, {33, 74, 40}
, {-49, -17, 38}
, {-75, -39, -16}
, {-8, -9, -14}
, {52, -60, -53}
, {-68, -44, -75}
, {-17, 31, 13}
, {62, -58, 72}
, {-13, -20, 51}
, {52, 20, -58}
, {-28, 58, -55}
, {12, -9, 36}
, {54, 62, -17}
, {34, 12, 62}
, {-46, 2, 0}
, {36, 32, 16}
, {-58, 57, -1}
, {37, 14, -24}
, {63, 34, -51}
, {-86, 44, 55}
, {66, 66, 44}
, {-53, -110, 10}
, {59, 52, -21}
, {5, -62, -34}
, {31, -79, 62}
, {-63, 65, 18}
, {14, -11, 48}
, {2, -8, 16}
, {48, 13, 41}
, {-15, -9, -6}
, {-34, 4, 67}
, {-58, -29, 65}
, {57, 58, -17}
, {71, -25, -56}
, {-26, 99, -16}
}
, {{-5, 17, 47}
, {-51, 28, -69}
, {-68, 54, 88}
, {11, -63, 63}
, {-29, 58, 16}
, {-20, -43, 53}
, {0, -57, 37}
, {-46, 24, -14}
, {67, 25, 32}
, {59, 50, -21}
, {9, 95, 5}
, {70, 2, 49}
, {10, -15, 34}
, {-42, 16, 74}
, {-43, 0, -27}
, {29, 22, 56}
, {-30, 41, -4}
, {0, 0, 60}
, {79, 16, 78}
, {-52, 27, 69}
, {-48, -43, 54}
, {37, 13, -77}
, {-55, -4, 49}
, {71, 43, 35}
, {-37, -46, -77}
, {12, -43, 42}
, {-31, 17, -79}
, {-31, 49, 48}
, {67, 40, 52}
, {78, 103, -63}
, {11, 3, -51}
, {25, -29, -52}
, {-51, 11, 46}
, {-22, -45, -45}
, {41, 14, -42}
, {-51, 42, -28}
, {23, -7, 56}
, {27, 36, 8}
, {38, 65, -40}
, {-25, -29, -33}
, {62, 40, -32}
, {51, 58, 63}
, {-57, -80, 47}
, {-38, 61, 12}
, {46, 47, -14}
, {-72, 1, -29}
, {16, 23, -3}
, {-36, 79, 28}
, {37, -64, 5}
, {-53, -13, -21}
, {-40, -75, 20}
, {8, 14, -21}
, {-35, 59, 54}
, {-12, 25, -28}
, {-13, 8, -7}
, {29, -18, 51}
, {27, 9, 46}
, {-55, -20, 83}
, {20, -10, 29}
, {3, 30, 17}
, {47, 3, -61}
, {-51, 3, 66}
, {30, 73, -69}
, {45, -39, 60}
}
, {{-2, 35, -21}
, {-25, -20, -64}
, {-34, -1, -46}
, {-68, -53, 15}
, {53, 40, -51}
, {-4, 0, -28}
, {-6, -53, 42}
, {-67, 70, 67}
, {91, -25, 87}
, {-28, 68, 21}
, {-28, 31, 45}
, {51, -25, -36}
, {-20, 38, 34}
, {-21, 21, 18}
, {-27, -40, -5}
, {-19, -1, -4}
, {-6, -40, -19}
, {19, 59, -47}
, {63, -22, 61}
, {37, 37, 74}
, {-63, -56, 39}
, {-4, -63, 10}
, {-68, -18, 41}
, {16, -49, -40}
, {-9, 70, 2}
, {-12, -8, -41}
, {39, 7, 67}
, {-54, -57, 41}
, {-20, 32, -38}
, {40, 86, 7}
, {49, 59, -24}
, {-29, 73, 28}
, {-20, -17, 45}
, {26, 16, -22}
, {-50, 76, 13}
, {36, 31, 56}
, {-20, -25, -8}
, {5, -27, 64}
, {-37, 52, 61}
, {-67, 20, 40}
, {-67, 7, -38}
, {6, -34, 58}
, {-37, 39, -32}
, {73, -11, 22}
, {85, 8, 48}
, {47, -61, -25}
, {-84, 24, 61}
, {15, 72, -50}
, {-18, -68, -53}
, {-16, -21, 64}
, {-15, 29, 86}
, {15, 36, 63}
, {57, -18, 47}
, {-45, -32, 63}
, {37, 7, 31}
, {-37, 69, 0}
, {-55, 16, -42}
, {-57, 21, -16}
, {21, 66, 44}
, {-6, 57, -44}
, {4, -41, -38}
, {53, 31, -24}
, {-72, -24, 21}
, {57, -12, 39}
}
, {{23, 30, -55}
, {-7, -46, -64}
, {-42, -66, -37}
, {24, 7, -39}
, {9, 10, -63}
, {-83, 31, -13}
, {-2, 37, 12}
, {-49, 23, 22}
, {-47, 10, 43}
, {12, -16, 21}
, {-68, 40, -52}
, {25, 53, -62}
, {-27, 31, 30}
, {-60, 5, 59}
, {39, 36, -27}
, {-38, -8, 56}
, {-31, -67, 25}
, {-78, 38, -64}
, {49, 33, -56}
, {-45, -80, -39}
, {43, 5, -52}
, {-23, -35, 17}
, {-43, -2, 1}
, {5, -18, 55}
, {-29, -49, 25}
, {-35, 63, -60}
, {39, -40, 15}
, {3, 32, 37}
, {-21, 16, 16}
, {62, 4, -16}
, {-5, 45, -51}
, {-69, -24, -30}
, {3, 0, -77}
, {-73, -11, -67}
, {13, -11, -8}
, {-21, -10, 30}
, {53, -77, 29}
, {32, 0, 30}
, {38, 46, -36}
, {-20, -78, 52}
, {-72, -5, 23}
, {65, 13, 25}
, {-79, 34, 0}
, {-49, 58, -57}
, {57, -35, -54}
, {1, 5, -11}
, {16, 46, 60}
, {-6, 16, 49}
, {-35, -3, 0}
, {52, -8, 52}
, {18, -44, -43}
, {40, -8, -41}
, {-39, 40, 60}
, {62, -13, -61}
, {60, 38, -80}
, {-70, 50, -78}
, {-48, 27, 58}
, {-52, 56, 49}
, {33, -19, -53}
, {48, 32, -21}
, {-19, 40, 4}
, {58, -3, 10}
, {37, -63, 3}
, {29, -48, 0}
}
, {{46, 20, 30}
, {8, -21, 62}
, {7, -5, 25}
, {-57, -28, 17}
, {4, -56, 58}
, {-20, 6, -7}
, {-38, -4, 63}
, {-20, 19, -52}
, {24, -66, -18}
, {-3, 8, -12}
, {-85, -54, -9}
, {-18, 46, -56}
, {-55, 14, -18}
, {-39, 24, 65}
, {-77, 25, -25}
, {64, 42, -27}
, {52, 32, 59}
, {-19, 5, -30}
, {-32, 35, -26}
, {67, 60, -39}
, {30, 31, -1}
, {6, -1, -70}
, {59, -21, 63}
, {39, 18, 7}
, {31, -42, -29}
, {-5, -30, 18}
, {2, -27, 62}
, {-39, -63, -28}
, {2, -14, -32}
, {-39, -57, 17}
, {-3, -19, -57}
, {67, 60, 61}
, {15, 29, 44}
, {42, -16, 27}
, {-9, -25, 24}
, {43, 3, 91}
, {31, 65, 38}
, {26, 29, -13}
, {-65, 30, 49}
, {0, -4, 9}
, {36, -49, 15}
, {-34, 6, 9}
, {-60, 44, 54}
, {-89, -21, -65}
, {-12, -43, -15}
, {31, 7, -39}
, {-15, -40, -74}
, {-37, -61, 27}
, {-61, -21, -57}
, {-11, -8, 37}
, {33, -39, -46}
, {-16, -44, -2}
, {-30, -14, 41}
, {-71, 56, 19}
, {2, -26, -17}
, {7, 41, -10}
, {-48, 49, -9}
, {-71, 19, 51}
, {-21, -13, -17}
, {-5, 17, -32}
, {-75, 44, -31}
, {64, 40, -32}
, {-17, -25, -37}
, {4, 37, 41}
}
, {{39, 49, 62}
, {79, -65, 54}
, {-31, 81, -68}
, {23, 11, -37}
, {32, -24, -8}
, {-72, 47, 66}
, {17, -24, 19}
, {73, 66, -18}
, {35, -66, 0}
, {-36, -23, -44}
, {-42, -61, 17}
, {59, 31, -53}
, {58, -52, 80}
, {-56, -28, -5}
, {50, -37, -34}
, {-18, -55, -12}
, {-75, 39, 27}
, {39, 0, 44}
, {45, -64, -52}
, {-23, 14, -35}
, {-66, -43, 22}
, {-31, 21, -44}
, {33, 44, -34}
, {36, -9, 74}
, {63, -18, 24}
, {-17, -50, 44}
, {-9, -77, -3}
, {-19, 70, -55}
, {-21, -43, -16}
, {-52, -52, 71}
, {-38, -42, -37}
, {-73, -25, -48}
, {71, 38, 45}
, {-48, -61, -36}
, {28, -14, 25}
, {30, -54, 80}
, {39, 35, 52}
, {33, -53, -29}
, {-21, 59, -43}
, {-43, -58, -43}
, {-66, -1, 85}
, {-43, -39, -36}
, {-76, 20, 16}
, {64, -1, 4}
, {76, 71, 19}
, {51, -60, -4}
, {69, -9, -68}
, {66, -3, 17}
, {-22, 21, 65}
, {58, 55, -5}
, {11, -8, 9}
, {-5, -55, 2}
, {11, -6, 78}
, {-70, -60, 38}
, {-66, 3, -56}
, {64, 78, -23}
, {-27, -32, 28}
, {-40, 33, 45}
, {33, -49, -72}
, {56, -22, 17}
, {-51, -43, 69}
, {53, -32, -36}
, {-34, 49, -29}
, {-21, -17, 57}
}
, {{-74, -1, -61}
, {50, -1, -54}
, {39, -23, 24}
, {-32, 23, 33}
, {-2, 37, -50}
, {-43, 0, -55}
, {-17, -47, 15}
, {-10, 12, 59}
, {75, 53, 17}
, {-9, -65, -62}
, {-15, 10, -8}
, {-6, 66, 26}
, {-8, 20, 24}
, {-30, -42, 43}
, {-3, -74, 50}
, {-62, 44, 29}
, {53, -66, 4}
, {-63, 51, -30}
, {68, -40, 50}
, {-35, -35, -42}
, {53, -80, 46}
, {10, 40, 32}
, {41, -85, 30}
, {-18, 15, -51}
, {45, -28, 39}
, {-60, -1, 42}
, {-40, -5, -36}
, {-62, -36, -61}
, {-34, 0, 4}
, {-59, 36, -42}
, {26, 2, -18}
, {-78, 2, 51}
, {64, 49, -37}
, {-18, -68, -57}
, {43, -22, 52}
, {-53, 9, 30}
, {64, -2, 22}
, {56, 53, 26}
, {-7, -18, -76}
, {54, 54, 41}
, {42, -24, -33}
, {-58, -13, 50}
, {57, -1, -6}
, {58, -23, 72}
, {7, 65, 45}
, {-25, 42, -34}
, {-42, 32, -66}
, {50, 25, 69}
, {30, -28, -47}
, {55, 12, -53}
, {56, -62, -35}
, {-46, -9, -2}
, {47, 14, 21}
, {-74, 33, -22}
, {-86, 44, -74}
, {81, 20, -54}
, {-43, 68, 5}
, {53, 33, -40}
, {-49, 2, -36}
, {42, -77, -4}
, {-71, 14, 36}
, {-44, -20, 40}
, {31, 54, -63}
, {-65, -25, 34}
}
, {{-61, 14, 74}
, {-11, -4, -48}
, {18, -52, -60}
, {15, 70, 17}
, {-36, -8, -64}
, {7, 33, -45}
, {-66, 41, -27}
, {-61, 20, -22}
, {51, -40, -60}
, {-50, -68, -8}
, {-15, 33, -53}
, {-9, 4, -73}
, {33, 32, 8}
, {-23, -12, 33}
, {0, -64, 31}
, {23, 5, 68}
, {6, 49, 17}
, {49, 35, 54}
, {-33, -39, -62}
, {0, 0, 30}
, {42, 61, -30}
, {63, -19, 73}
, {47, -19, 39}
, {7, -4, 29}
, {78, -37, 54}
, {70, 28, -40}
, {70, -67, 18}
, {-40, -23, -10}
, {-54, 44, 23}
, {-43, -85, -39}
, {76, 65, 57}
, {-54, 26, 50}
, {16, -34, -6}
, {-54, -58, 23}
, {-2, 65, 77}
, {-47, 33, 79}
, {64, 60, -5}
, {-37, 67, 75}
, {39, 7, -25}
, {-23, 17, -45}
, {51, -6, -63}
, {20, -30, 66}
, {-49, 3, 57}
, {-64, 50, 47}
, {-47, -94, -64}
, {31, 31, 71}
, {2, 65, -23}
, {-60, 3, -89}
, {1, -17, -52}
, {41, 20, 33}
, {27, -31, 32}
, {-11, -18, -68}
, {27, 73, -69}
, {11, -34, -37}
, {-31, 45, -33}
, {30, -7, 9}
, {-60, -25, 28}
, {-34, -17, -82}
, {-20, -33, -3}
, {-73, -71, -12}
, {3, 29, -36}
, {67, 31, 58}
, {-26, 65, 28}
, {-72, -25, -48}
}
, {{-60, -81, -6}
, {23, -4, -10}
, {-65, -88, -90}
, {-11, 41, 37}
, {-34, 20, 50}
, {-5, -52, -18}
, {-57, -51, -63}
, {34, -48, -45}
, {-24, -69, 4}
, {1, 36, -64}
, {-85, -88, -65}
, {16, -71, -86}
, {-11, -90, -56}
, {19, 7, 3}
, {37, 16, -52}
, {25, -48, -40}
, {3, 23, 26}
, {37, 11, -36}
, {-43, -28, 17}
, {-12, 3, -39}
, {20, 0, -74}
, {52, 43, -17}
, {57, -3, 47}
, {-23, 57, 52}
, {10, 8, 60}
, {-57, 4, -2}
, {16, 61, -55}
, {4, 3, -36}
, {6, 4, -3}
, {8, -30, -13}
, {-18, 53, -67}
, {39, 15, -3}
, {-73, 14, -12}
, {-64, -27, 59}
, {63, 32, 37}
, {12, 33, -4}
, {26, 61, 28}
, {-3, 21, -10}
, {-66, -35, -51}
, {-20, -14, -25}
, {6, 31, 6}
, {-1, -16, -27}
, {48, -18, 50}
, {43, -68, 51}
, {-66, -12, 19}
, {-84, -52, -70}
, {-54, -12, 49}
, {35, 25, 4}
, {-69, -42, 27}
, {-64, 51, -14}
, {-24, -57, -69}
, {-36, -83, 28}
, {-27, 6, -24}
, {36, 51, -9}
, {16, 62, 6}
, {-3, -76, 46}
, {39, -20, 38}
, {-24, -55, 29}
, {-53, -24, 14}
, {43, -49, 1}
, {-29, -37, 23}
, {59, -44, 21}
, {-86, -105, 10}
, {-63, -81, -63}
}
, {{-79, 28, -18}
, {-34, -23, -30}
, {47, -26, 32}
, {-67, 6, -58}
, {-41, -21, 60}
, {-50, -5, 15}
, {17, -46, 25}
, {-15, 63, 60}
, {59, -79, -65}
, {8, -22, -20}
, {33, 19, -78}
, {-92, -62, 1}
, {0, -85, 23}
, {-84, -31, 30}
, {-19, -60, -17}
, {-16, 37, 35}
, {33, -47, -42}
, {40, -44, 0}
, {13, 37, -59}
, {9, 10, -28}
, {-61, -50, -52}
, {-68, -74, -15}
, {0, 31, 56}
, {-4, 38, 20}
, {83, -20, -1}
, {-48, -22, -49}
, {-52, -40, 30}
, {-31, -33, -80}
, {10, 50, -30}
, {3, 55, 0}
, {17, 45, -22}
, {22, -94, -74}
, {-36, -8, -68}
, {5, 15, -12}
, {6, -39, 9}
, {-46, -16, 50}
, {-32, 39, -32}
, {-21, -67, -63}
, {19, 52, 47}
, {24, -16, 26}
, {16, 56, -16}
, {-77, -43, -76}
, {0, 62, 50}
, {-15, -54, -59}
, {-68, 37, -24}
, {45, 56, -29}
, {43, 15, 7}
, {3, -69, 52}
, {-15, 11, 57}
, {0, 17, 37}
, {-22, -25, -30}
, {32, 11, 28}
, {-50, -59, -69}
, {-35, -1, 24}
, {68, -35, 1}
, {-56, 1, -53}
, {47, -36, -73}
, {-17, 19, -3}
, {3, 51, 8}
, {-4, 14, -8}
, {-55, -18, -73}
, {54, 48, -54}
, {20, -39, -6}
, {-81, -16, -61}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE