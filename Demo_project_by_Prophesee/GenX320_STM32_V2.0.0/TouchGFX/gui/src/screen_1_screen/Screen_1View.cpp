/* TouchGFX Includes */
#include <gui/screen_1_screen/Screen_1View.hpp>
#include <touchgfx/Color.hpp>

/* Standard Includes */
#include <vector>
#include <stdio.h>

/* User Includes */
#include "main.h"
#include "genx320.h"
#include "tasks.h"
#include "app_state.h"
#include "task_frame_buffer.h"

/* Imported Variables */
extern uint32_t nbev_rate;
//extern int prediction;

Screen_1View::Screen_1View()
{

}

void Screen_1View::setupScreen()
{
    Screen_1ViewBase::setupScreen();
}

void Screen_1View::tearDownScreen()
{
    Screen_1ViewBase::tearDownScreen();
}

/**
 * @brief Function to handle the screen swipe.
 */
void Screen_1View::handleClickEvent(const ClickEvent& event) {

	if (event.getType() == ClickEvent::PRESSED)
	{
		/* Record the initial touch position */
		startTouchX = event.getX();
		startTouchY = event.getY();
	}
	else if (event.getType() == ClickEvent::RELEASED)
	{
		/* Calculate the distance moved */
		int32_t deltaX = event.getX() - startTouchX;
		int32_t deltaY = event.getY() - startTouchY;

		/* Check if it's a left swipe */
		if (deltaX < -swipeThreshold)
		{
			Screen_1View::changeScreenRight();
		}
		/* Check if it's a up swipe */
		else if (deltaY < -swipeThreshold)
		{
			Screen_1View::changeScreenDown();
		}
	}
}

/**
 * @brief Function to update the selected screen.
 */
void Screen_1View::updateScreen()
{
	enter_view();
}

/**
 * @brief Function to initialize the screen.
 */
void Screen_1View::screenInit() {

	statusChecked = false;

}

/**
 * @brief Function to check the status of the IC2 driver.
 */
void Screen_1View::checkI2CStatus() {

	if(statusChecked != true ) {

		/* Check if the Lock is available before suspending tasks */
		UBaseType_t lockState = get_lock_sem_count(drv_i2c);
		if(lockState == 1) {
			Screen_1View::updateScreen();
			statusChecked = true;
		}
	}
}

/**
 * @brief Function to update the event rate.
 */
void Screen_1View::updateEvents()
{
	//printf("%d", get_prediction());
	/* Update the Event rate */
	Unicode::snprintf(EV_TextBuffer, EV_TEXT_SIZE, "%d", get_prediction());
	EV_Text.invalidate();

	/* Update the Power consumption progress bar */
	PC_Progress.setValue(pc_interpolate(nbev_rate, MIN_MEVTS, MAX_MEVTS, MIN_PC, MAX_PC));

}
