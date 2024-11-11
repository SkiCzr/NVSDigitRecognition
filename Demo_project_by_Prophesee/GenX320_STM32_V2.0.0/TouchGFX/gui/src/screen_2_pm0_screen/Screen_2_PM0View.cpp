/* TouchGFX Includes */
#include <gui/screen_2_pm0_screen/Screen_2_PM0View.hpp>

/* User Includes */
#include "main.h"
#include "genx320.h"
#include "tasks.h"
#include "app_state.h"

/* Imported Variables */
extern const struct issd *current_issd;
extern uint32_t ev_processed;
extern uint32_t calib_done;

Screen_2_PM0View::Screen_2_PM0View()
{

}

void Screen_2_PM0View::setupScreen()
{
    Screen_2_PM0ViewBase::setupScreen();
}

void Screen_2_PM0View::tearDownScreen()
{
    Screen_2_PM0ViewBase::tearDownScreen();
}

/**
 * @brief Function to initialize the screen.
 */
void Screen_2_PM0View::screenInit() {

	statusChecked = false;

}

/**
 * @brief Function to check the status of the IC2 driver.
 */
void Screen_2_PM0View::checkI2CStatus() {

	if(statusChecked != true ) {

		/* Check if the Lock is available before suspending tasks */
		UBaseType_t lockState = get_lock_sem_count(drv_i2c);
		if(lockState == 1) {
			Screen_2_PM0View::updateScreen();
			statusChecked = true;
		}
	}
}

/**
 * @brief Function to update the selected screen.
 */
void Screen_2_PM0View::updateScreen()
{
	enter_PM0_view();

	/* Execute the power down sequence */
	ULP_standby_sequence();

	/* Update the ULP_State */
	pm0_enabled = true;

}

/**
 * @brief Function to read and process the output of the ULP pins.
 */
void Screen_2_PM0View::readGPIO()
{

	/* Check if PM0 has been enabled*/
	if(pm0_enabled) {

		/* Ignore the first wakeup signal and add a delay of 1 second to filter false wake up */
		if((HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_9) == GPIO_PIN_SET) && wakeup_ignored == 0) {

			/* Wakeup signal received, send acknowledgment using ULP_RSTN */
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);

			wakeup_ignored = true;

		}

		/* Read the state of ULP_Wakeup pin */
		if(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_9) == GPIO_PIN_SET) {

			/* Wakeup signal received, send acknowledgment using ULP_RSTN */
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
			psee_sleep_us_imp(Tulp_rstn_low);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);

			/*Execute the power up sequence */
			ULP_wakeup_sequence();

			/* Update the ULP_State */
			pm0_enabled = false;

			/* Reset Calibration */
			ev_processed = 0;
			calib_done = 0;

			/* Change Screen */
			Screen_2_PM0ViewBase::PM0Wakeup();
		}
	}
}



