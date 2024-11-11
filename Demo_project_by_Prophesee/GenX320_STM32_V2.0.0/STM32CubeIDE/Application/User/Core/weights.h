/*
 * weights.h
 *
 *  Created on: Oct 23, 2024
 */

#ifndef APPLICATION_USER_CORE_WEIGHTS_H_
#define APPLICATION_USER_CORE_WEIGHTS_H_

#include <stdint.h>

//extern double weights_conv1[125] __attribute__((section(".sdram_data")));
//extern double weights_conv2[1250] __attribute__((section(".sdram_data")));
//extern double weights_FC1[13440] __attribute__((section(".sdram_data")));
//extern double weights_FC2[840] __attribute__((section(".sdram_data")));

extern int16_t weights_conv1[125];
extern int16_t weights_conv2[1250];
extern int16_t weights_FC1[13440];
extern int16_t weights_FC2[840];

#endif /* APPLICATION_USER_CORE_WEIGHTS_H_ */
