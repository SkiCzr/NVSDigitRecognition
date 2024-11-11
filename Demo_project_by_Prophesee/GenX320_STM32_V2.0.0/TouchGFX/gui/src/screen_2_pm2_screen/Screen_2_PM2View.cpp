/* TouchGFX Includes */
#include <gui/screen_2_pm2_screen/Screen_2_PM2View.hpp>
#include <touchgfx/Color.hpp>

/* Standard Includes */
#include <vector>
#include <stdio.h>

/* User Includes */
#include "main.h"
#include "genx320.h"
#include "tasks.h"
#include "app_state.h"

/* Color_interpolate Variables */
static Display_Color offColor = {193, 207, 221}; // #C1CFDD
static Display_Color onColor = {64, 126, 201}; // #407EC9
static std::vector<Display_Color> interpolatedColors(16);

Screen_2_PM2View::Screen_2_PM2View()
{
	Screen_2_PM2View::initCounters();

}

void Screen_2_PM2View::setupScreen()
{
    Screen_2_PM2ViewBase::setupScreen();
}

void Screen_2_PM2View::tearDownScreen()
{
    Screen_2_PM2ViewBase::tearDownScreen();
}

/**
 * @brief Function to initialize the screen.
 */
void Screen_2_PM2View::screenInit() {

	statusChecked = false;
	screenUpdated = false;

}

/**
 * @brief Function to check the status of the IC2 driver.
 */
void Screen_2_PM2View::checkI2CStatus() {

	if(statusChecked != true ) {

		/* Check if the Lock is available before suspending tasks */
		UBaseType_t lockState = get_lock_sem_count(drv_i2c);
		if(lockState == 1) {
			Screen_2_PM2View::updateScreen();
			statusChecked = true;
		}
	}
}

/**
 * @brief Function to update the selected screen.
 */
void Screen_2_PM2View::updateScreen()
{
	enter_PM2_view();
	screenUpdated = true;
}

/**
 * @brief Function to read and process the output of the LPW algo.
 */
void Screen_2_PM2View::readLPW()
{

	if(screenUpdated == true) {
		if(lockState == UNLOCKED) {
			if(me_direction == TRUE_WAKE_UP) {

				/* Update the GUI state */
				Outer_Box.setBorderColor(touchgfx::Color::getColorFromRGB(0, 255, 00));
				Outer_Box.invalidate();
				Awake.setAlpha(255);
				Awake.invalidate();
				Inst.setAlpha(0);
				Inst.invalidate();

				/* Lock and update the device state */
				lockState = LOCKED;
				deviceState = TRUE_WAKE_UP;

			}
			else if(me_direction == FALSE_WAKE_UP) {

				/* Update the GUI state */
				Outer_Box.setBorderColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
				Outer_Box.invalidate();
				Awake.setAlpha(0);
				Awake.invalidate();
				Inst.setAlpha(0);
				Inst.invalidate();

				/* Lock and update the device state */
				lockState = LOCKED;
				deviceState = FALSE_WAKE_UP;

			}
			else if(me_direction == SLEEP_STATE) {

				/* Update the GUI state */
				Outer_Box.setBorderColor(touchgfx::Color::getColorFromRGB(193, 207, 221));
				Outer_Box.invalidate();
				Awake.setAlpha(0);
				Awake.invalidate();
				Inst.setAlpha(255);
				Inst.invalidate();
			}
		}
		else if(lockState == LOCKED) {
			if (deviceState == TRUE_WAKE_UP) {
				timerCount++;

				if(timerCount == 50) {
					/* Unlock the State and reset the timer */
					lockState = UNLOCKED;
					timerCount = 0;

					/* Switch back to screen selection */
					Screen_2_PM2ViewBase::changeScreenToModeSelection();
				}
			}
			else if (deviceState == FALSE_WAKE_UP) {
				timerCount++;

				if(timerCount == 25) {
					/* Unlock the State and reset the timer */
					lockState = UNLOCKED;
					timerCount = 0;
				}
			}
		}
	}
}

/**
 * @brief Function to read the low power counters.
 */
void Screen_2_PM2View::lowPowerMode() {

	/* Compute the interpolated color value for each counter */
    for (int i = 0; i < 16; i++) {
        interpolatedColors[i] = color_interpolate(lp_counter[i], MIN_EVTS, MAX_EVTS, offColor, onColor);
    }

    /* Update all counter values */
	for (int i = 0; i < totalCounters; i++) {
		lpCounters[i]->setColor(touchgfx::Color::getColorFromRGB(interpolatedColors[i].red, interpolatedColors[i].green, interpolatedColors[i].blue));
		lpCounters[i]->invalidate();
	}

}
