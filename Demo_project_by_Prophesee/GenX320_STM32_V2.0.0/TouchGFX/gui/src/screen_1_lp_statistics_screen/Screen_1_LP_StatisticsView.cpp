/* TouchGFX Includes */
#include <gui/screen_1_lp_statistics_screen/Screen_1_LP_StatisticsView.hpp>
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

Screen_1_LP_StatisticsView::Screen_1_LP_StatisticsView()
{
	Screen_1_LP_StatisticsView::initCounters();
}

void Screen_1_LP_StatisticsView::setupScreen()
{
	Screen_1_LP_StatisticsViewBase::setupScreen();
}

void Screen_1_LP_StatisticsView::tearDownScreen()
{
	Screen_1_LP_StatisticsViewBase::tearDownScreen();
}

/**
 * @brief Function to handle the screen swipe.
 */
void Screen_1_LP_StatisticsView::handleClickEvent(const ClickEvent& event){

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
			Screen_1_LP_StatisticsView::changeScreenLeft();
		}
	}
}

/**
 * @brief Function to initialize the screen.
 */
void Screen_1_LP_StatisticsView::screenInit() {

	statusChecked = false;

}

/**
 * @brief Function to check the status of the IC2 driver.
 */
void Screen_1_LP_StatisticsView::checkI2CStatus() {

	if(statusChecked != true ) {

		/* Check if the Lock is available before suspending tasks */
		UBaseType_t lockState = get_lock_sem_count(drv_i2c);
		if(lockState == 1) {
			Screen_1_LP_StatisticsView::updateScreen();
			statusChecked = true;
		}
	}
}

/**
 * @brief Function to update the selected screen.
 */
void Screen_1_LP_StatisticsView::updateScreen()
{
	enter_LP_statistics_view();
}

/**
 * @brief Function to read the low power counters.
 */
void Screen_1_LP_StatisticsView::readLpCounters()
{

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
