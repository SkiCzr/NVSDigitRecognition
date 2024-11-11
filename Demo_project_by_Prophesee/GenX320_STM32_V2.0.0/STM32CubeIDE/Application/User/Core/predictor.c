#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "Predictor.h"
#include <string.h>
#include "weights.h"


/* Arrays for storing the outputs of every layer */
float Conv1out[24 * 24 * 3] ;
float MP1out[12 * 12 * 3] ;
float Conv2out[8 * 8 * 6] ;
float MP2out[4 * 4 * 6] ;
float FC1out[52] ;
float FC2out[10];

void print_NN_Input(int8_t buffer[28][28]){
	for (int i = 0; i < 28; i++){
				for (int j = 0; j < 28; j++){
					printf("%u", buffer[i][j]);
					printf (" ");
				}
			}
	printf("\n");

}

float dequantize_weight(int16_t quantized_weight, float scaling_factor) {
    // Reverse the scaling by dividing by the scaling factor
    return (float)(quantized_weight) / scaling_factor;
}

/* Calculates the input index for a 1D array, given inputs of a 3D array */
int input_index(int row, int col, int h, int height_in, int width_in) {
    return h*width_in*height_in+row*width_in+col;
}

/* Calculates the input index for a 1D array, given inputs of a 2D array */
int input_index2D(int row, int col, int num_in) {
    return row*num_in+col;
}

/* Implementation of a convolution layer with given parameters. It includes by default ReLu activation */
void Conv(int kernel_size, int depth, int previous_depth, int in_rows, int in_cols, float *matrix, float *output, int16_t *kernels) {
    int out_rows = in_rows - kernel_size + 1;
    int out_cols = in_cols - kernel_size + 1;
    int index = 0;
    int filter = 0;
    float sum = 0;

    for (int block_i = 0; block_i < out_rows; block_i += 1) {
        for (int block_j = 0; block_j < out_cols; block_j += 1) {
            for(int filter_i = 0; filter_i < depth; filter_i += 1) {
                filter = filter_i * previous_depth * kernel_size * kernel_size;
                for(int k = 0; k < previous_depth; k++) {
                    for (int i = 0; i < kernel_size; i++) {
                        for (int j = 0; j < kernel_size; j++) {

                            sum += dequantize_weight(kernels[filter], SCALE_FACTOR) * matrix[input_index((block_i + i),
                                (block_j + j), k, in_rows, in_cols)];
                            filter++;
                        }
                    }
                }

                // ReLu
                if (sum < 0) sum = 0;

                output[index] = sum;
                sum = 0;
                index++;
            }
        }
    }
}

float sigmoid(float x) {
        float z = expf(x);
        return 1.0f / (1.0f + z);
}

// Function for a fully connected layer, includes by default sigmoid activation
void fullyConnectedLayer(int inputSize, int outputSize, float* input, float* output, int16_t* weights) {
    int size = inputSize * outputSize;
    float temp;
    int k = 0;
    for (int i = 0; i < size;) {
        temp = 0;
        for (int j = 0; j < inputSize; j++) {
            temp += dequantize_weight(weights[i], SCALE_FACTOR) * input[j];
            ++i;
        }

        // sigmoid activation function
        output[k] = sigmoid(temp);
        k++;
    }
}

// Function to get the index of the maximum value (used for the final prediction)
int getMaxIndex(float* array, int size) {
    int maxIndex = 0;
    for (int i = 1; i < size; i++) {
        if (array[i] > array[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

// Function implementing Max pooling for pool_size = 2 and stride = 2
// depth - depth of the input layer
// size - width or height of the input layer

void max_pooling(int depth, int size, float *input, float *output) {
    int index = 0;
    float max = 0;

    for (int d = 0; d < depth; d++) {
        for (int block_i = 0; block_i < size; block_i += 2) {
            for (int block_j = 0; block_j < size; block_j += 2) {
                int ind_bi = block_i * depth * size;
                int ind_bj = block_j * depth;
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        int ind_i = i * depth;
                        int ind_j = j * depth;
                        if(input[d + (ind_bi + ind_i * size + ind_bj + ind_j)] > max) {
                            max = input[d + (ind_bi + ind_i * size + ind_bj + ind_j)];
                        }
                    }
                }
                output[index] = max;

                index++;
                max = 0 ;
            }
        }
    }
}

int predict(float output[]) {
        int max_index = 0;
        float max_value = output[0];
        for (int i = 1; i < 10; ++i) {
           // printf(" %f ", output[i]);
            if (output[i] > max_value) {
                max_value = output[i];
                max_index = i;
            }
        }
        return max_index;
    }

int calculate_the_weight_of_a_9by9_block(uint8_t* buffer, int row, int col){
	int result = 0;

	for (int r = 0; r < 9; r++){
		for (int c = 0; c < 9; c++){
			if (buffer[( 320 * (row + r) + (col + c))] != 0) {
				result += 1;
			}
		}
	}

	return result;
}

/* Function that performs forward propagation on a 28x28 array input */
int predict_main(float *img) {

	int the_prediction;

//    int input[28][28] = {
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,},
//           {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,},
//           {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,},
//           {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,},
//           {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,},
//           {0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,},
//           {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
//
//    };
//



    Conv(5, 3, 1, 28, 28, img, Conv1out, weights_conv1);

	max_pooling(3, 24, Conv1out,MP1out);

    Conv(5, 6, 3, 12, 12, MP1out, Conv2out, weights_conv2);

    max_pooling(6, 8, Conv2out,MP2out);

    fullyConnectedLayer(96, 52, MP2out, FC1out, weights_FC1);

    fullyConnectedLayer(52, 10, FC1out, FC2out, weights_FC2);


    the_prediction = predict(FC2out);

    //printf("PREDICTED NR = %d \n", the_prediction);

    return the_prediction;
}
