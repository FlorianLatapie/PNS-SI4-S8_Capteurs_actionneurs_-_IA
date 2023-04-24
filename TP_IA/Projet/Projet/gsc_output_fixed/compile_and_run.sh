g++ -Wall -Wextra -pedantic -Ofast -o gsc_fixed -Igsc_output_fixed/ gsc_output_fixed/model.c ../main.cpp
./gsc_fixed x_test.csv y_test.csv