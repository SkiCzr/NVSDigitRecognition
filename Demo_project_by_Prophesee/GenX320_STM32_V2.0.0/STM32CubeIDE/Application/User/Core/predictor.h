/*
 * predictor.h
 *
 *  Created on: Oct 17, 2024
 *      Author: Sefu
 */

#ifndef APPLICATION_USER_CORE_PREDICTOR_H_
#define APPLICATION_USER_CORE_PREDICTOR_H_

#include <stdint.h>
#include "weights.h"

#define SCALE_FACTOR 10000.0

// External declarations for global variables

// Function prototypes

int calculate_the_weight_of_a_9by9_block(uint8_t* buffer, int row, int col);
int input_index(int row, int col, int h, int height, int width);
int input_index2D(int i, int j, int num_in);

void Conv(int kernel_size, int depth, int previous_depth, int in_rows, int in_cols, float *matrix, float *output, int16_t *kernels);

void loadWeightsFromFile(const char* filename);

float sigmoid(float x);

void fullyConnectedLayer(int inputSize, int outputSize, float* input, float* output, int16_t* weights);

int getMaxIndex(float* array, int size);

void max_pooling(int depth, int size, float *input, float *output);
void max_pooling2(int depth, int size, float *input, float *output);

int predict(float output[]);
int predict_main(float *img);
#endif /* APPLICATION_USER_CORE_PREDICTOR_H_ */
