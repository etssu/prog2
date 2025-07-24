#include <stdio.h>
#include <ctype.h>
#include <math.h>

int binarna_skokova(double x) {
    if (x < 0) {
        return 0;
    }
    else {
        return 1;
    }
}
double reLU(double x) {
    if (x <= 0) {
        return 0;
    }
    else {
        return x;
    }
}
double squareplus(double x) {
    int b = 2;
    return 0.5 * (x + sqrt(x*x + b));
}
double gaussova_f(double x) {
    return exp(-(x*x));
}
double hyperbolicky_tan(double x) {
    return (exp(x)- exp(-x))/(exp(x)+exp(-x));
}
double swiss(double x) {
    double b = 0.5;
    return x / (1.0 +exp(-b * x));
}
double auto_mod(double x_start, double x_stop, double step, int precision, int width) {
    printf("%*s%*s%*s%*s%*s%*s%*s\n", width, "x", width, "1", width, "2", width, //headline
            "3", width, "4", width, "5", width, "6");

    for (double x = x_start; x <= x_stop; x += step) {
        double f1 = binarna_skokova(x);        //assigning values
        double f2 = reLU(x);
        double f3 = squareplus(x);
        double f4 = gaussova_f(x);
        double f5 = hyperbolicky_tan(x);
        double f6 = swiss(x);

        //formatting the answer
        char format[32];        //limit
        sprintf(format, "%%%d.%df", width, precision);

        printf("%*.*f%*.*f%*.*f%*.*f%*.*f%*.*f%*.*f\n", width, precision, x, width, precision, f1,
            width, precision, f2, width, precision, f3, width, precision, f4, width, precision, f5, width, precision, f6);
    }
    return 0;
}

double manual_mod(double x_start, double x_stop, double step, int precision, int width,
    int extra_count[64], int extra_index) {
    int i = 0;
    int all_invalid = 1;  //to check valid

    // if at least 1 is valid
    for (i = 0; i < extra_index; i++) {
        if (extra_count[i] >= 1 && extra_count[i] <= 6) {
            all_invalid = 0;  //if 1 is valid
            break;
        }
    }

    // if all invalid - finish
    if (all_invalid) {
        return 0;
    }

    // headline
    printf("%*s", width, "x");
    for (double x = x_start; x <= x_stop; x += step) {
        printf("%*.*f", width, precision, x);
    }
    printf("\n");

    while (i < extra_index) { // while i < amount of nums
        if (extra_count[i] < 1 || extra_count[i] > 6) {
            i++;  // if it doesn't fit
            continue;  // skip
        }
        printf("%*d", width, extra_count[i]); // print the name of a function

        for (double x = x_start; x <= x_stop; x += step) {
            double value = 0;

            if (extra_count[i] == 1) {        //assigning valeus
                value = binarna_skokova(x);
            } else if (extra_count[i] == 2) {
                value = reLU(x);
            } else if (extra_count[i] == 3) {
                value = squareplus(x);
            } else if (extra_count[i] == 4) {
                value = gaussova_f(x);
            } else if (extra_count[i] == 5) {
                value = hyperbolicky_tan(x);
            } else if (extra_count[i] == 6) {
                value = swiss(x);
            } else if (extra_count[i] > 6) {
                continue;
            }
            printf("%*.*f", width, precision, value);  // print values
        }
        printf("\n");        //to continue on a new line with a new value
        i++;
    }
    return 0;
}

int main(void) {
        double x_start, x_stop, step,cislo;
        int precision, width;
        char end_symb;
        int count = 0;        //to count inputs
        int extra_count[64];    //to hold extra numbers
        int extra_index = 0;    //index for extras

    while (scanf("%lf", &cislo) == 1) {
        count++;
        if (count > 5) {
            extra_count[extra_index] = cislo;
            extra_index++;
        }
        else if (count <= 5) {    //assigning a value
            if (count == 1) {
                x_start = cislo;
            }
            else if (count == 2) {
                x_stop = cislo;
            }
            else if (count == 3) {
                step = cislo;
            }
            else if (count == 4) {
                precision = cislo;
            }
            else if (count == 5) {
                width = cislo;
            }
        }
    }

    //Error 1
    if (count < 5) {
        printf("E1\n");
        return 0;
    }
    if (scanf(" %c", &end_symb) == 1) {
        if (!isalpha(end_symb)) {      //if the end symbol is digit
            printf("E1\n");
            return 0;
        }
    } else {        //if we don't have anything after the nums
        printf("E1\n");
        return 0;
    }

    //Error 2
    if (x_start >= x_stop) {
        printf("E2\n");
        return 0;
    }

        //auto mode
        if (count == 5) {
            auto_mod(x_start, x_stop, step, precision, width);
        }
        //manual mode
        else if (count > 5) {
            manual_mod(x_start, x_stop, step, precision, width, extra_count, extra_index);
        }
        return 0;
}