#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h" // Swish funkcia a pomocne funkcie -- NEMENIT !!!
#include "data.h" // Makra, vahy a bias hodnoty -- NEMENIT !!!
#define MAX_SIZE 100


double findMax(double *output) {        //to find max index
    int max_index = 0;
    double max_value = output[0];
    for (int i = 1; i < NUMBER_OF_NEURONS; i++) {
        if (output[i] > max_value) {
            max_value = output[i];
            max_index = i;
        }
    }
    return max_index;
}
void softMax(double *z, int size){        //size is number of neurons
    double max_z = z[0];

    for (int i = 1 ; i < size ; i++) { //searching for max value
        if (z[i] > max_z) {
            max_z = z[i];
        }
    }
    double sum_exp = 0.0;
    //denominator
    for (int j = 0; j < size; j++) {
        sum_exp += exp(z[j] - max_z);
    }

    //softmax values
    for (int i = 0; i < size; i++) {
        z[i] = exp(z[i] - max_z) / sum_exp;
    }
    findMax(z);
}
void calculateWeightedSum(double input[IMAGE_SIZE], double weights[NUMBER_OF_NEURONS * IMAGE_SIZE], double bias[NUMBER_OF_NEURONS], double z[NUMBER_OF_NEURONS], int mode){
    //double z[NUMBER_OF_NEURONS];        //to store linear combinations

    for (int i = 0; i < NUMBER_OF_NEURONS; i++) {
        double sum = bias[i];
        // searching for needed pixel
        for (int j = 0; j < IMAGE_SIZE; j++) {
            // index in 1d
            int weight_index = i * IMAGE_SIZE + j;
            sum += weights[weight_index] * input[j];
        }
        z[i] = sum;
        if (mode != 3) {
            z[i] = swish(z[i]); //activating neurons
        }
    }
}
double rezim_2(int index_neuronu, int riadok, int stlpec, double weights[NUMBER_OF_WEIGHTS]) {
    int pixel_index = riadok * 28 + stlpec;
    int weight_index = index_neuronu * IMAGE_SIZE + pixel_index;
    printf("%.2f", weights[weight_index]);
    return 0;
}
double rezim_3(double input[IMAGE_SIZE], int mode) {
    double z[NUMBER_OF_NEURONS];

    calculateWeightedSum(input, weights, bias, z, mode);
    for (int i = 0; i < 10; i++) {
        printf("%8u%8.2f\n", i, z[i]);
    }
    return 0;
}

double rezim_4() {
    double numbers[10]; //to store values

    for (int i = 0; i < 10; i++) {
        scanf("%lf", &numbers[i]);
    }
    softMax(numbers,NUMBER_OF_NEURONS); //activate softmax
    for (int i = 0; i < 10; i++) {      //printing out
        printf("%.2lf ", numbers[i]);
    }
    return 0;
}
double rezim_5(int mode) {
    double numbers[10]; //values to send in findmax

    for (int i = 0; i < 10; i++) {
        scanf("%lf", &numbers[i]);
    }
    int max_index = findMax(numbers);
    printf("%d\n", max_index);
    return 0;
}
double rezim_6(double img[IMAGE_SIZE], int mode) {
    double values[10];       //storing values
    calculateWeightedSum(img, weights, bias, values, mode); //calculate values
    softMax(values, 10); //send to softmax
    int vysledok = findMax(values);     //find max_index
    printf("%d\n", vysledok);
    return 0;
}
double rezim_7(int mode) {
    int K; //parameter K
    int menovky[MAX_SIZE]; // postupnosť správnych menoviek
    double img[MAX_SIZE][IMAGE_SIZE];
    //double *saved_images[MAX_SIZE];
    scanf("%d", &K); //enter parameter K

    for (int i = 0; i < K; i++) { //enter labels
        scanf("%d", &menovky[i]);
    }

    for (int i = 0; i < K; i++) { //load images
        load_image(img[i], IMAGE_SIZE);
    }

    int count = 0; //to calculate correct classifications
    int predictions[10]; //to store predictions
    for (int i = 0; i < K; i++) {
        double z[10]; //to store linear combinations

        calculateWeightedSum(img[i], weights, bias, z, mode);
        softMax(z, NUMBER_OF_NEURONS);
        int prediction = findMax(z); //making a prediction
        predictions[i] = prediction;

        if (prediction == menovky[i]) {
            count++;
        }
    }
    double success_rate = (double) count/K * 100.0;
    printf("%.2lf ", success_rate);

    for (int j = 0; j < K; j++) {
        printf("%d-%d-%d ", j, menovky[j], predictions[j]);
    }

    return 0;
}
int main(void) {
    int mode;
    scanf("%d", &mode);
    double img[IMAGE_SIZE];

    if (mode == 1) {    //rezim 1
            load_image(img, IMAGE_SIZE);
    print_image(img, 28, 28);
}
    else if (mode == 2) {
        int index_neuronu, riadok, stlpec;
        weights[NUMBER_OF_WEIGHTS];
        scanf("%d %d %d", &index_neuronu, &riadok, &stlpec);
        rezim_2(index_neuronu, riadok, stlpec, weights);
    }
    else if (mode == 3) {
        load_image(img, IMAGE_SIZE);
        rezim_3(img, mode);
    }
    else if (mode == 4) {
        rezim_4();
    }
    else if (mode == 5) {
        rezim_5(mode);
    }
    else if (mode == 6) {
        load_image(img, IMAGE_SIZE);
        rezim_6(img, mode);
    }
    else if (mode == 7) {
        rezim_7(mode);
    }
    return 0;
}