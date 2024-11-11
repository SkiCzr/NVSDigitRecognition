/* TouchGFX Includes */
#include <gui/screen_2_screen/Screen_2View.hpp>

/* User Includes */
#include "main.h"
#include "genx320.h"
#include "tasks.h"
#include "app_state.h"

Screen_2View::Screen_2View()
{

}

void Screen_2View::setupScreen()
{
    Screen_2ViewBase::setupScreen();
}

void Screen_2View::tearDownScreen()
{
    Screen_2ViewBase::tearDownScreen();
}

/**
 * @brief Function to initialize the screen.
 */
void Screen_2View::screenInit() {

	statusChecked = false;

}

/**
 * @brief Function to check the status of the IC2 driver.
 */
void Screen_2View::checkI2CStatus() {

	if(statusChecked != true ) {

		/* Check if the Lock is available before suspending tasks */
		UBaseType_t lockState = get_lock_sem_count(drv_i2c);
		if(lockState == 1) {
			Screen_2View::updateScreen();
			statusChecked = true;
		}
	}
}

/**
 * @brief Function to update the selected screen.
 */
void Screen_2View::updateScreen()
{
	enter_view2();
}

/**
 * @brief Function to handle the screen swipe.
 */
void Screen_2View::handleClickEvent(const ClickEvent& event){

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
			Screen_2View::changeScreenLeft();
		}
		/* Check if it's a left swipe */
		else if (deltaX < -swipeThreshold)
		{
			Screen_2View::changeScreenRight();
		}
		/* Check if it is PM2 button press */
		else if (((event.getX() >= PM2_Button.getX()) && (event.getX() <= (PM2_Button.getX() + 136))) && ((event.getY() >= PM2_Button.getY()) && (event.getY() <= (PM2_Button.getY() + 40)))) {

			Screen_2ViewBase::PM2_Screen();
		}
		/* Check if it is PM0 button press */
		else if (((event.getX() >= PM0_Button.getX()) && (event.getX() <= (PM0_Button.getX() + 136))) && ((event.getY() >= PM0_Button.getY()) && (event.getY() <= (PM0_Button.getY() + 40)))) {

			Screen_2ViewBase::PM0_Screen();
		}
	}
}
