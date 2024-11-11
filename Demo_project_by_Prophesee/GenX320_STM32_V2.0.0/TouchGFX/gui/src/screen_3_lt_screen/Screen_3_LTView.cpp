/* TouchGFX Includes */
#include <gui/screen_3_lt_screen/Screen_3_LTView.hpp>

/* User Includes */
#include "main.h"
#include "genx320.h"
#include "tasks.h"
#include "app_state.h"

Screen_3_LTView::Screen_3_LTView()
{

}

void Screen_3_LTView::setupScreen()
{
    Screen_3_LTViewBase::setupScreen();
}

void Screen_3_LTView::tearDownScreen()
{
    Screen_3_LTViewBase::tearDownScreen();
}

/**
 * @brief Function to initialize the screen.
 */
void Screen_3_LTView::screenInit() {

	statusChecked = false;

}

/**
 * @brief Function to check the status of the IC2 driver.
 */
void Screen_3_LTView::checkI2CStatus() {

	if(statusChecked != true ) {

		/* Check if the Lock is available before suspending tasks */
		UBaseType_t lockState = get_lock_sem_count(drv_i2c);
		if(lockState == 1) {
			Screen_3_LTView::updateScreen();
			statusChecked = true;
		}
	}
}

/**
 * @brief Function to handle the screen swipe.
 */
void Screen_3_LTView::handleClickEvent(const ClickEvent& event) {

	if (event.getType() == ClickEvent::PRESSED)
	{
		/* Record the initial touch position */
		startTouchX = event.getX();
	}
	else if (event.getType() == ClickEvent::RELEASED)
	{
		/* Calculate the distance moved */
		int32_t deltaX = event.getX() - startTouchX;

		/* Check if it's a right swipe */
		if (deltaX > swipeThreshold)
		{
			Screen_3_LTView::changeScreenLeft();
		}
	}
}

/**
 * @brief Function to update the selected screen.
 */
void Screen_3_LTView::updateScreen() {
	enter_LT_view();
}

/**
 * @brief Function to read and process the output of the LT algo.
 */
void Screen_3_LTView::updateCoordinates() {

	/* Indicate the framework that this entire Drawable needs to be redrawn */
	verticalLine.invalidate();
	horizontalLine.invalidate();

	/* Update the X coordinate of the LED blob */
	verticalLine.setStart((abs((int)(led_coordinates[0] - guiDifferenceConst))), xStart);
	verticalLine.setEnd((abs((int)(led_coordinates[0] - guiDifferenceConst))), xEnd);

	/* Update the Y coordinate of the LED blob */
	horizontalLine.setStart(yStart, (int)led_coordinates[1]);
	horizontalLine.setEnd(yEnd, (int)led_coordinates[1]);

	/* Indicate the framework that this entire Drawable needs to be redrawn */
	verticalLine.invalidate();
	horizontalLine.invalidate();

}
